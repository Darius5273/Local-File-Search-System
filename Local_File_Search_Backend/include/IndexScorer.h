#ifndef INDEX_SCORER_H
#define INDEX_SCORER_H

#include <string>
#include <unordered_map>
#include <unordered_set>

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
