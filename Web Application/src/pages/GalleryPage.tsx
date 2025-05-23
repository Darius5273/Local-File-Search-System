import React, { useEffect, useState } from 'react';
import { useLocation, useNavigate } from 'react-router-dom';
import { fetchImage } from '../api/ImagesApi';
import { SearchResult } from '../interfaces/SearchResult';


const GalleryPage: React.FC = () => {
  const location = useLocation();
  const navigate = useNavigate();
  const results: SearchResult[] = location.state?.results ?? [];

  const [images, setImages] = useState<string[]>([]);

  useEffect(() => {
    const loadImages = async () => {
      const imageResults = results.filter((r) => r.is_image);
      const fetchedImages = await Promise.all(
        imageResults.map((r) => fetchImage(r.path))
      );
      setImages(fetchedImages);
    };

    loadImages();
  }, [results]);

  if (!results.length) {
    return (
      <div style={{ padding: '2rem', textAlign: 'center' }}>
        <p>No images to display. Please run a search first.</p>
        <button onClick={() => navigate(-1)}>Go Back</button>
      </div>
    );
  }

 

  return (
    <div style={{ padding: '2rem' }}>
      <h2>Gallery View</h2>
      <div style={{ display: 'flex', gap: '20px', flexWrap: 'wrap' }}>
        {images.map((imgSrc: string , index: number) => (
          <img
            key={index}
            src={imgSrc}
            alt={`Image ${index + 1}`}
            style={{
              width: '200px',
              height: 'auto',
              border: '1px solid #ccc',
              borderRadius: '4px',
            }}
          />
        ))}
      </div>
    </div>
  );
};

export default GalleryPage;
