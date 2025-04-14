import { useNavigate } from 'react-router-dom';
import { useEffect, useState } from 'react';
import SearchBar from '../components/SearchBar';
import SearchResults from '../components/SearchResults';
import { searchFiles } from '../api/SearchApi.tsx';
import '../styles/SearchPage.css';
import { SearchResponse } from '../interfaces/SearchResponse';
import { SearchResult } from '../interfaces/SearchResult';
import { fetchImage } from '../api/ImagesApi.tsx';

const SearchPage: React.FC = () => {
  const navigate = useNavigate();
  const [query, setQuery] = useState('');
  const [suggestions, setSuggestions] = useState<string[]>([]);
  const [results, setResults] = useState<SearchResult[]>([]);
  const [error, setError] = useState<string | null>(null);
  const [imageMap, setImageMap] = useState<Record<string, string>>({});
  const [loadingImages, setLoadingImages] = useState(false);

  const handleSearch = async () => {
    try {
      setError(null);
      setLoadingImages(true);
      setImageMap({});
      const response: SearchResponse = await searchFiles(query);
      setSuggestions(response.suggestions || []);
      setResults(response.rankingResults || []);
      const newImageMap: Record<string, string> = {};
      await Promise.all(
        response.rankingResults.map(async (result) => {
          if (result.is_image) {
            try {
              const imageUrl = await fetchImage(result.path);
              newImageMap[result.path] = imageUrl;
            } catch (imgError) {
              console.error(`Failed to load image: ${result.path}`, imgError);
            }
          }
        })
      );
      setImageMap(newImageMap);
      setLoadingImages(false);
    } catch (err: any) {
      setError(err.message);
    }
  };

  return (
    <div className="search-page full-screen">
      <div className="gradient-box">
        <button
          className="file-crawler-btn"
          onClick={() => navigate('/crawler')}
        >
          File Crawler
        </button>
        <h1>Local File Search Engine</h1>
      </div>

      <div className="search-bar-wrapper">
        <SearchBar value={query} onChange={setQuery} onSearch={handleSearch} />
      </div>

      {error && <p className="error-message">{error}</p>}

      <div className="main-content">
        <div className="results-wrapper">
          <SearchResults results={results} imageMap={imageMap}/>
        </div>

        <div className="suggestions-box">
          <h2>Suggestions</h2>
          <ul>
            {suggestions.map((s, i) => (
              <li key={i}>{s}</li>
            ))}
          </ul>
        </div>
      </div>
    </div>
  );
};

export default SearchPage;
function setImageMap(arg0: {}) {
  throw new Error('Function not implemented.');
}

