#ifndef LOGUTILS_H
#define LOGUTILS_H

#include <string>

class LogUtils {
public:
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
};

#endif // LOGUTILS_H 