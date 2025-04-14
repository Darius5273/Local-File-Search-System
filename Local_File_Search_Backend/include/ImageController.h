#ifndef IMAGE_CONTROLLER_H
#define IMAGE_CONTROLLER_H
#include "FileMetadataExtractor.h"
#include "httplib.h"

class ImageController {
public:
    void registerRoutes(httplib::Server& server);
private:
    FileMetadataExtractor fileMetadataExtractor;
};

#endif // IMAGE_CONTROLLER_H
