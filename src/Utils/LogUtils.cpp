#include "LogUtils.h"
#include <iostream>

void LogUtils::info(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

void LogUtils::warning(const std::string& message) {
    std::cout << "[WARNING] " << message << std::endl;
}

void LogUtils::error(const std::string& message) {
    std::cout << "[ERROR] " << message << std::endl;
} 