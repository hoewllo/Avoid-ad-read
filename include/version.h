// version.h - EPUB Ad Cleaner Version Information
// This file contains version information for the project

#ifndef EPUB_CLEANER_VERSION_H
#define EPUB_CLEANER_VERSION_H

#include <string>

namespace epub_cleaner {

// Version information
struct VersionInfo {
    // Version from VERSION file
    static constexpr const char* VERSION = "1.0.5";
    static constexpr const char* PROJECT_NAME = "epub_cleaner";
    static constexpr const char* PROJECT_DESCRIPTION = "EPUB Ad Cleaner - C++ EPUB ad content cleaning tool";
    static constexpr const char* COPYRIGHT = "Copyright © 2024 Jianyin Li. All rights reserved.";
    
    // Get formatted version string
    static std::string getVersionString() {
        return std::string(PROJECT_NAME) + " v" + VERSION;
    }
    
    // Get full project information
    static std::string getFullInfo() {
        return getVersionString() + "\n" + PROJECT_DESCRIPTION + "\n" + COPYRIGHT;
    }
};

} // namespace epub_cleaner

#endif // EPUB_CLEANER_VERSION_H