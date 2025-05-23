import { KeywordWidget } from "./KeywordWidget";
import { MetadataBucket } from "./MetadataBucket";
import { SearchResult } from "./SearchResult";


export interface SearchResponse {
    suggestions: string[];
    rankingResults: SearchResult[];
    correctedQuery: string;
    fileTypeCategories: MetadataBucket[];
    extensionCounts: MetadataBucket[];
    yearCounts: MetadataBucket[];
    sizeBuckets: MetadataBucket[];
    languages?: MetadataBucket[];
    contextWidgets?: string;
    keywordWidgets?: KeywordWidget[];
  }
  