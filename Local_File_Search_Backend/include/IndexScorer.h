#ifndef INDEX_SCORER_H
#define INDEX_SCORER_H

#include <string>
#include <unordered_map>
#include <unordered_set>

const long long KB = 1024;
const long long MB = 1024 * KB;
const long long GB = 1024 * MB;
std::unordered_set<std::string> textExtensions = {".txt", ".md", ".py", ".c", ".cpp", ".java"};
std::unordered_map<std::string, double> mimeTypeScores = {
        {"application/pdf", 0.9},
        {"application/msword", 0.9},
        {"application/vnd.openxmlformats-officedocument.wordprocessingml.document", 0.9},
        {"application/vnd.ms-excel", 0.7},
        {"application/octet-stream", 0.1},
        {"image/png", 0.5},
        {"image/jpeg", 0.5},
        {"text/plain", 0.8},
        {"text/x-c", 0.7},
        {"text/x-c++", 0.7},
        {"text/x-python", 0.7}
};

class IndexScorer {
public:

    static double scoreFile(const std::string& path, const std::string& extension,
                            const std::string& mime_type, const std::string& modified_time,
                            long long size, bool is_text);

private:
    static double pathDepthScore(const std::string& path);
    static double pathLengthScore(const std::string& path);
    static double mimeTypeScore(const std::string& mime_type, const std::string& extension);
    static double sizeScore(long long size);
    static double recencyScore(const std::string& modified_time);
    static double textScore(bool is_text);

    static int getPathDepth(const std::string& path);
};

#endif // INDEX_SCORER_H
