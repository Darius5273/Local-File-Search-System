import React from 'react';
import { useNavigate } from 'react-router-dom';
import { SearchResult } from '../interfaces/SearchResult';
import { KeywordWidget } from '../interfaces/KeywordWidget';
import * as FaIcons from 'react-icons/fa';

interface WidgetVisualizerProps {
  keywordWidgets?: KeywordWidget[];
  contextWidgets: string;
  results: SearchResult[];
}


  const WidgetVisualizer: React.FC<WidgetVisualizerProps> = ({ keywordWidgets, contextWidgets, results }) => {

  const navigate = useNavigate()
  const textWidgets = contextWidgets ? contextWidgets.split('//') : [];
  const hasIcons = keywordWidgets && keywordWidgets.some(w => w.icon?.trim().length > 0);
  const shouldShowTitle = (textWidgets.length > 0) || hasIcons;



  return (
    <div className="widget-indicator" style={{ marginTop: '1rem',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center', 
        justifyContent: 'center', }}>
      {shouldShowTitle &&<p style={{ fontWeight: 'bold' }}>Widget Activated</p>}
      <div style={{ display: 'flex', alignItems: 'center', gap: '12px', flexWrap: 'wrap' }}>
        {keywordWidgets?.map((widget, index) => {
          const Icon = (FaIcons as any)[widget.icon];
          if (Icon)
          return <Icon key={index} size={32} color="gray" />
        })}


        {textWidgets && textWidgets.map((text, i) => {
          const isGallery = text.toLowerCase().includes("gallery");
          return isGallery ? (
            <span
              key={i}
              onClick={() => navigate('/gallery', { state: { results } })}
              style={{ textDecoration: 'underline', color: 'blue', cursor: 'pointer' }}
            >
              {text}
            </span>
          ) : (
            <span
              key={i}
              style={{ textDecoration: 'underline', color: 'black' }}
            >
              {text}
            </span>
          );
        })}
      </div>
    </div>
  );
};

export default WidgetVisualizer;
