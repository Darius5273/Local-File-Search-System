#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/ImageController.h"

void ImageController::registerRoutes(httplib::Server& server) {
    server.Get("/image", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("path")) {
            res.status = 400;
            res.set_content("{\"error\": \"Missing image path parameter\"}", "application/json");
            return;
        }

        std::string path = req.get_param_value("path");
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            res.status = 404;
            res.set_content("{\"error\": \"File not found\"}", "application/json");
            return;
        }

        std::string mimeType = fileMetadataExtractor.getMimeType(path);
        res.set_header("Content-Type", mimeType);

        std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        res.set_content(data, mimeType);
    });
}
