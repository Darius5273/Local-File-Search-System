import { useNavigate } from 'react-router-dom';
import { useEffect, useState } from 'react';
import SearchBar from '../components/SearchBar';
import SearchResults from '../components/SearchResults';
import { searchFiles } from '../api/SearchApi.tsx';
import '../styles/SearchPage.css';
import { SearchResponse } from '../interfaces/SearchResponse';
import { fetchImage } from '../api/ImagesApi.tsx';
import { setSpellStrategy } from '../api/SpellApi.tsx';
import { SearchResult } from '../interfaces/SearchResult.tsx';
import MetadataAnalyzer from '../components/MetadataSummary.tsx';
import WidgetVisualizer from '../components/WidgetVisualizer.tsx';

const SearchPage: React.FC = () => {
  const navigate = useNavigate();
  const [query, setQuery] = useState('');
  const [suggestions, setSuggestions] = useState<string[]>([]);
  const [results, setResults] = useState<SearchResult[]>([]);
  const [spellStrategy, setSpellStrategyState] = useState('noSpellCheck');
  const [error, setError] = useState<string | null>(null);
  const [imageMap, setImageMap] = useState<Record<string, string>>({});
  const [loadingImages, setLoadingImages] = useState(false);
  const [correctedQuery, setCorrectedQuery] = useState("");
  const [showCorrectedQuery, setShowCorrectedQuery] = useState(false);
  const [response, setResponse] = useState<SearchResponse>();

  const handleSearch = async () => {
    try {
      setError(null);
      setLoadingImages(true);
      setImageMap({});
      const response: SearchResponse = await searchFiles(query);
      setSuggestions(response.suggestions || []);
      setResults(response.rankingResults || []);
      setCorrectedQuery(response.correctedQuery || "");
      setResponse(response);
      const newImageMap: Record<string, string> = {};
      if (response.rankingResults) {
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
    }
      setImageMap(newImageMap);
      setLoadingImages(false);
    } catch (err: any) {
      setError(err.message);
    }
  };

  const handleStrategyChange = async (e: React.ChangeEvent<HTMLSelectElement>) => {
    const strategy = e.target.value;
    setSpellStrategyState(strategy);
    if (strategy == "none")
      setShowCorrectedQuery(false);
    else
      setShowCorrectedQuery(true);
    try {
      await setSpellStrategy(strategy); 
    } catch (err) {
      setError('Failed to update spell strategy');
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
      <div style={{ marginBottom: '1rem' }}>
        <label htmlFor="strategy-select">Spell Correction Strategy: </label>
        <select
          id="strategy-select"
          value={spellStrategy}
          onChange={handleStrategyChange}
        >
          <option value="none">No Correction</option>
          <option value="englishSpellCheck">English Spell Check</option>
        </select>
        {showCorrectedQuery && (
        <>
          <p><strong>Results shown for: </strong> {correctedQuery}</p>
        </>)
        }
        {(response?.contextWidgets && response.contextWidgets.length > 0 || response?.keywordWidgets) && (
        <WidgetVisualizer keywordWidgets={response.keywordWidgets} contextWidgets={response.contextWidgets || ""} results = {response.rankingResults} />
        )}

      </div>
      
      
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
         {response && <MetadataAnalyzer metadata={response} />}
      </div>
    </div>
  );
};

export default SearchPage;
function setImageMap(arg0: {}) {
  throw new Error('Function not implemented.');
}

