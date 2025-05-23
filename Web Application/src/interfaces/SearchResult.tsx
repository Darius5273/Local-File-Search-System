export interface SearchResult {
  path: string;
  score: number;
  preview?: string;
  is_image: boolean;
  correctedQuery: string;
}