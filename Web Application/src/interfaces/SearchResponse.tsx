import { SearchResult } from "./SearchResult";


export interface SearchResponse {
    suggestions: string[];
    rankingResults: SearchResult[];
  }
  