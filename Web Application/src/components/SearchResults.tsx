import { SearchResult } from "../interfaces/SearchResult";

interface Props {
  results: SearchResult[];
  imageMap: Record<string, string>;
}

export default function SearchResults({ results, imageMap }: Props) {
  return (
    <div className="space-y-6">
      {results.map((result, index) => (
        <div key={index} className="border p-4 rounded shadow bg-white">
          <div className="font-bold text-lg mb-1 text-gray-800" style={{ fontWeight: 'bold' }}>
            {index + 1}. {result.path}
          </div>
          {result.is_image ? (
            <img
            src={imageMap[result.path]}
            alt="Preview"
              className="max-h-64 object-contain rounded"
            />
          ) : (
            <div
              className="text-sm text-gray-600"
              dangerouslySetInnerHTML={{ __html: result.preview as string }}
            />
          )}
        </div>

      ))}
    </div>
  );
}
