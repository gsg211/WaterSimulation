#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

struct Material {
    std::string diffuseTexture; // map_Kd
};

inline std::map<std::string, Material> loadMTL(const std::string& path) {
    std::map<std::string, Material> materials;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open MTL: " << path << std::endl;
        return materials;
    }

    std::string line, currentMat;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "newmtl") {
            ss >> currentMat;
            materials[currentMat] = Material{};
        }
        else if (token == "map_Kd" && !currentMat.empty()) {
            ss >> materials[currentMat].diffuseTexture;
        }
    }
    return materials;
}