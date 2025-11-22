#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

static std::string buildFullPath(std::string path, int levelsUp = 0) {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path targetPath = currentPath;
    
    for (int i = 0; i < levelsUp; i++) {
        targetPath = targetPath.parent_path();
    }
    
    targetPath /= path;
    return targetPath.string();
}