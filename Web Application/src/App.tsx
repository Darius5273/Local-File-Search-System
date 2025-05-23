import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import SearchPage from './pages/SearchPage';
import CrawlerPage from './pages/CrawlerPage';
import GalleryPage from "./pages/GalleryPage";

function App() {
  return (
  <Router>
    <Routes>
      <Route path="/" element={<SearchPage />} />
      <Route path="/crawler" element={<CrawlerPage />} />
      <Route path="/gallery" element={<GalleryPage />} />
    </Routes>
  </Router>);
}

export default App;
