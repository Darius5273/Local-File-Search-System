import { SearchResult } from "./SearchResult";


export interface SearchResponse {
    suggestions: string[];
    rankingResults: SearchResult[];
    correctedQuery: string;
    metadataSummary: {
    totalFiles: number;
    totalSizeBytes: number;
    fileTypeCategories: MetadataBucket[];
    extensionCounts: MetadataBucket[];
    yearCounts: MetadataBucket[];
    sizeBuckets: MetadataBucket[];
    languages?: MetadataBucket[];
  };
  }
  