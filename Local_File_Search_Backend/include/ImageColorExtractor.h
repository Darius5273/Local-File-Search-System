#ifndef IMAGE_COLOR_EXTRACTOR_H
#define IMAGE_COLOR_EXTRACTOR_H

#include <string>
#include <opencv2/opencv.hpp>

class ImageColorExtractor {
public:
    static std::string GetDominantColor(const std::string& imagePath);
};

#endif // IMAGE_COLOR_EXTRACTOR_H
