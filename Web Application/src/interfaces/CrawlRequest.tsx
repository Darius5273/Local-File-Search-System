export interface CrawlRequest {
    rootPath: string;
    ignoreDirs: string[];
    ignoreFiles: string[];
    ignorePatterns: string[];
    reportFormat: 'markdown' | 'text';
  }