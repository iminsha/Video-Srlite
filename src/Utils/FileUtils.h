#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>

class FileUtils {
public:
    static bool exists(const std::string& path);
    static std::string getExtension(const std::string& path);
    static std::string getBaseName(const std::string& path);
};

#endif // FILEUTILS_H 