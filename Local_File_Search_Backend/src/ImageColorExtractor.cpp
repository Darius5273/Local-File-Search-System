#include "../include/ImageColorExtractor.h"
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <iomanip>

struct Vec3bHash {
    std::size_t operator()(const cv::Vec3b& v) const {
        return (static_cast<size_t>(v[0]) << 16) | (static_cast<size_t>(v[1]) << 8) | v[2];
    }
};

struct Vec3bEqual {
    bool operator()(const cv::Vec3b& a, const cv::Vec3b& b) const {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
    }
};

static const std::unordered_map<std::string, std::string> namedColors = {
        {"black", "#000000"}, {"white", "#FFFFFF"}, {"red", "#FF0000"}, {"green", "#00FF00"},
        {"blue", "#0000FF"}, {"yellow", "#FFFF00"}, {"cyan", "#00FFFF"}, {"magenta", "#FF00FF"},
        {"orange", "#FFA500"}, {"purple", "#800080"}, {"pink", "#FFC0CB"}, {"gray", "#808080"},
        {"brown", "#A52A2A"}, {"navy", "#000080"}, {"teal", "#008080"}, {"lime", "#00FF00"},
        {"indigo", "#4B0082"}, {"maroon", "#800000"}, {"olive", "#808000"}
};

static cv::Vec3b hexToRGB(const std::string& hex) {
    int r = std::stoi(hex.substr(1, 2), nullptr, 16);
    int g = std::stoi(hex.substr(3, 2), nullptr, 16);
    int b = std::stoi(hex.substr(5, 2), nullptr, 16);
    return cv::Vec3b(b, g, r);
}

static std::string closestNamedColor(const cv::Vec3b& dominantColor) {
    double minDist = std::numeric_limits<double>::max();
    std::string closestName;

    for (const auto& [name, hex] : namedColors) {
        cv::Vec3b ref = hexToRGB(hex);
        double dist = std::sqrt(
                std::pow(dominantColor[0] - ref[0], 2) +
                std::pow(dominantColor[1] - ref[1], 2) +
                std::pow(dominantColor[2] - ref[2], 2)
        );
        if (dist < minDist) {
            minDist = dist;
            closestName = name;
        }
    }
    return closestName;
}

std::string ImageColorExtractor::GetDominantColor(const std::string& imagePath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    std::unordered_map<cv::Vec3b, int, Vec3bHash, Vec3bEqual> colorCount;

    for (int r = 0; r < image.rows; ++r) {
        for (int c = 0; c < image.cols; ++c) {
            cv::Vec3b color = image.at<cv::Vec3b>(r, c);
            colorCount[color]++;
        }
    }

    int maxCount = 0;
    cv::Vec3b dominantColor;
    for (const auto& [color, count] : colorCount) {
        if (count > maxCount) {
            maxCount = count;
            dominantColor = color;
        }
    }

    return closestNamedColor(dominantColor);
}
