import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import SearchPage from './pages/SearchPage';
import CrawlerPage from './pages/CrawlerPage';

function App() {
  return (
  <Router>
    <Routes>
      <Route path="/" element={<SearchPage />} />
      <Route path="/crawler" element={<CrawlerPage />} />
    </Routes>
  </Router>);
}

export default App;
