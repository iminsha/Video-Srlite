#include "FileUtils.h"
#include <iostream>

bool FileUtils::exists(const std::string& path) {
    std::cout << "FileUtils::exists called for: " << path << std::endl;
    return true;  // 简化实现
}

std::string FileUtils::getExtension(const std::string& path) {
    std::cout << "FileUtils::getExtension called for: " << path << std::endl;
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return "";
}

std::string FileUtils::getBaseName(const std::string& path) {
    std::cout << "FileUtils::getBaseName called for: " << path << std::endl;
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return path;
} 