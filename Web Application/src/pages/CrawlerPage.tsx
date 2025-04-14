import { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import '../styles/CrawlerPage.css';
import { crawlDirectory } from '../api/CrawlerApi';
import { CrawlRequest } from '../interfaces/CrawlRequest';

const CrawlerPage: React.FC = () => {
  const navigate = useNavigate();

  const [rootPath, setRootPath] = useState(localStorage.getItem('rootPath') || '');
  const [ignoreDirs, setIgnoreDirs] = useState(localStorage.getItem('ignoreDirs') || '');
  const [ignoreFiles, setIgnoreFiles] = useState(localStorage.getItem('ignoreFiles') || '');
  const [ignorePatterns, setIgnorePatterns] = useState(localStorage.getItem('ignorePatterns') || '');
  const [reportFormat, setReportFormat] = useState(localStorage.getItem('reportFormat') || 'text');
  const [status, setStatus] = useState<string | null>(null);

  useEffect(() => {
    localStorage.setItem('rootPath', rootPath);
    localStorage.setItem('ignoreDirs', ignoreDirs);
    localStorage.setItem('ignoreFiles', ignoreFiles);
    localStorage.setItem('ignorePatterns', ignorePatterns);
    localStorage.setItem('reportFormat', reportFormat);
  }, [rootPath, ignoreDirs, ignoreFiles, ignorePatterns, reportFormat]);

  const handleCrawl = async () => {
    setStatus('Crawling...');
    try {
      const config: CrawlRequest = {
        rootPath,
        ignoreDirs: ignoreDirs.split(';').map(s => s.trim()).filter(Boolean),
        ignoreFiles: ignoreFiles.split(';').map(s => s.trim()).filter(Boolean),
        ignorePatterns: ignorePatterns.split(';').map(s => s.trim()).filter(Boolean),
        reportFormat: reportFormat as 'text' | 'markdown',
      };
      const res = await crawlDirectory(config);
      setStatus(res.message);
    } catch (err: any) {
      setStatus(err.message);
    }
  };

  return (
    <div className="crawler-page-container">
      <div className="crawler-form">
        <h1 className="text-2xl font-bold mb-4 text-blue-700">File Crawler Configuration</h1>

        <div className="space-y-4">
          <input className="w-full p-3 border rounded" placeholder="Root Path" value={rootPath} onChange={(e) => setRootPath(e.target.value)} />
          <input className="w-full p-3 border rounded" placeholder="Ignore Dirs (semicolon separated)" value={ignoreDirs} onChange={(e) => setIgnoreDirs(e.target.value)} />
          <input className="w-full p-3 border rounded" placeholder="Ignore Files (semicolon separated)" value={ignoreFiles} onChange={(e) => setIgnoreFiles(e.target.value)} />
          <input className="w-full p-3 border rounded" placeholder="Ignore Patterns (semicolon separated)" value={ignorePatterns} onChange={(e) => setIgnorePatterns(e.target.value)} />

          <select className="w-full p-3 border rounded" value={reportFormat} onChange={(e) => setReportFormat(e.target.value)}>
            <option value="text">Text</option>
            <option value="markdown">Markdown</option>
          </select>

          <div className="flex-buttons mt-6">
            <button onClick={handleCrawl} className="bg-blue-600 text-white px-4 py-2 rounded hover:bg-blue-700">
              Start Crawling
            </button>
            <button onClick={() => navigate('/')} className="text-blue-600 underline hover:text-blue-800">
              Back to Search
            </button>
          </div>

          {status && <p className="mt-4 text-gray-700">{status}</p>}
        </div>
      </div>
    </div>
  );
};

export default CrawlerPage;
