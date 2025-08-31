#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

struct vec3 {
    float x;
    float y;
    float z;
};

struct model {
    std::vector<vec3> vertices;
    std::vector<int> faces;

    bool load_model(const std::string& file_path) noexcept {
        std::ifstream file(file_path);
        if (!file) {
            std::cerr << "Can't load model " << file_path << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);

            if (line.rfind("v ", 0) == 0) {
                char v;
                float x, y, z;
                iss >> v >> x >> y >> z;

                vec3 vertex = {
                    x,
                    y,
                    z
                };
                vertices.emplace_back(vertex);

            } else if (line.rfind("f ", 0) == 0) {
                char f;
                std::string v1, v2, v3;
                iss >> f >> v1 >> v2 >> v3;

                auto parse_index = [](const std::string& token) {
                    return std::stoi(token.substr(0, token.find('/')));
                };

                faces.push_back(parse_index(v1) - 1);
                faces.push_back(parse_index(v2) - 1);
                faces.push_back(parse_index(v3) - 1);
            }
        }
        return true;
    }

private:
    inline std::vector<std::string> split_string_by_token(const std::string& line, char token) {
        std::vector<std::string> tokens;
        const char* begin = line.c_str();
        const char* end = begin + line.size();
        const char* word_start = nullptr;

        for (const char* p = begin; p <= end; ++p) {
            if (*p == token) {
                if (word_start) {
                    tokens.emplace_back(word_start, p - word_start);
                    word_start = nullptr;
                }
            } else if (!word_start) {
                word_start = p;
            }
        }

        return tokens;
    }
};