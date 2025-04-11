#include "../include/IndexScorer.h"
#include <ctime>
#include <sstream>
#include <algorithm>
#include <iomanip>

double IndexScorer::scoreFile(const std::string& path, const std::string& extension,
                         const std::string& mime_type, const std::string& modified_time,
                         long long size, bool is_text)
{
    double score = 0.0;

    score += 1.2 * pathDepthScore(path);
    score += 0.6 * pathLengthScore(path);
    score += 1.0 * mimeTypeScore(mime_type, extension);
    score += 0.8 * sizeScore(size);
    score += 1.2 * recencyScore(modified_time);
    score += 0.8 * textScore(is_text);

    return score;
}


double IndexScorer::pathDepthScore(const std::string& path) {
    int depth = getPathDepth(path);
    if (depth < 3) return 1.0;
    if (depth < 6) return 0.6;
    if (depth < 10) return 0.3;
    return 0.1;
}

double IndexScorer::pathLengthScore(const std::string& path) {
    int len = static_cast<int>(path.length());
    if (len < 50) return 1.0;
    if (len < 100) return 0.7;
    if (len < 200) return 0.4;
    return 0.1;
}

double IndexScorer::mimeTypeScore(const std::string& mime_type, const std::string& extension) {

    if (mimeTypeScores.find(mime_type) != mimeTypeScores.end())
        return mimeTypeScores[mime_type];

    if (textExtensions.find(extension) != textExtensions.end())
        return 0.6;

    return 0.1;
}

double IndexScorer::sizeScore(long long size) {

    if (size < 500 * KB) return 0.8;
    if (size < 1 * MB) return 0.4;
    if (size < 1 * GB) return 0.2;
    return 0.1;
}

double IndexScorer::recencyScore(const std::string& modified_time) {
    std::tm tm = {};
    std::istringstream ss(modified_time);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    std::time_t file_time = std::mktime(&tm);
    std::time_t now = std::time(nullptr);

    double days_diff = difftime(now, file_time) / (60 * 60 * 24);
    if (days_diff < 3) return 1.2;
    if (days_diff < 7) return 1.0;
    if (days_diff < 30) return 0.8;
    if (days_diff < 180) return 0.5;
    if (days_diff < 365) return 0.3;
    return 0.1;
}

double IndexScorer::textScore(bool is_text) {
    return is_text ? 1.0 : 0.6;
}

int IndexScorer::getPathDepth(const std::string& path) {
    return std::count(path.begin(), path.end(), '/');
}

