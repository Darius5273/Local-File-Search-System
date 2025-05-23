# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),  
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Gallery page and integration into the search page.
- Dynamic widget system using observer pattern.
- Code widgets and keyword-aware widget classes.
- Metadata display in search results (size, date, etc.).
- Spell checker with configurable strategies.
- Search result caching mechanism.
- Support for file type recognition and image color indexing.
- Text extraction from PDF, Word, and CSV files.
- Observer-based ranking using search history.
- TypeScript interfaces and React components for web UI.
- File ignore rules for improved indexing efficiency.

### Changed

- Search controller to include widgets and metadata in results.
- Indexer optimized to batch insert and report.
- Spell correction strategy can now be updated and retrieved.
- Refactored search engine and database connector for better modularity.
- Query parser enhanced with path/content support and validation.
- Performance improvements in content indexing and search.

### Fixed

- Issue with missing file extensions.
- Circular dependencies in C++ backend modules.
- UTF-8 encoding issues during database insert.
- Error handling in backend request flow.
- Display and logic bugs in search ranking and previews.

### Removed

- Unused comments and debug statements.
- Excessive database content insertion to optimize size.

## [1.0.0] - 2025-03-21

### Added

- Initial implementation of C++ backend and React frontend.
- PostgreSQL database integration using libpqxx.
- Core components: FileCrawler, Indexer, SearchEngine.
- Full-text file indexing and real-time query execution.
- REST API endpoints for search and file crawling.
- Architectural documentation following the C4 model.
- Support for MIME type detection and metadata extraction.
