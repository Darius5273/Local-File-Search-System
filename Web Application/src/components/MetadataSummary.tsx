import React from "react";
import { SearchResponse } from "../interfaces/SearchResponse";
import '../styles/SearchPage.css';
import { FaIcons } from "react-icons/fa";
interface Props {
  metadata: SearchResponse;
}

export default function MetadataAnalyzer({ metadata }: Props) {
  const sections = [
    { title: "File Type Categories", data: metadata.fileTypeCategories, label: "type" },
    { title: "Extension Counts", data: metadata.extensionCounts, label: "extension" },
    { title: "Year Counts", data: metadata.yearCounts, label: "year" },
    { title: "Size Buckets", data: metadata.sizeBuckets, label: "bucket" },
    { title: "Languages", data: metadata.languages, label: "language" },
  ];

  return (
    <div className="metadata-analyzer">
      <h2 className="text-lg">Metadata Analysis</h2>
      {sections.map(
        (section) =>
          section.data &&
          section.data.length > 0 && (
            <div key={section.title} className="mb-4">
              <h3 className="font-medium text-gray-700">{section.title}</h3>
              <ul className="ml-4 list-disc text-sm text-gray-800">
                {section.data.map((item, i) => (
                  <li key={i}>
                    {item[section.label]}: {item.count}
                  </li>
                ))}
              </ul>
            </div>
          )
      )}
    </div>
  );
}
