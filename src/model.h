#pragma once

#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "tgaimage.h"
#include "util.h"
#include "types.h"

struct Model {
    std::vector<vec3> vertices;
    std::vector<int> faces;

    Model(const std::string& file_path) {
        std::ifstream file(file_path);
        if (!file) {
            std::cerr << "Can't load model " << file_path << std::endl;
            return;
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
    }

    inline int nverts() const { return vertices.size(); }
    inline int nfaces() const { return faces.size()/3; }

    inline vec3 vert(const int i) const {
        return vertices[i];
    }

    inline vec3 vert(const int iface, const int nthvert) const {
        return vertices[faces[iface*3+nthvert]];
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

    inline std::tuple<int, int, int> project(const vec3 coord, const int width, const int height) noexcept {
        return {
            static_cast<int>((coord[0] + 1.) * width / 2),
            static_cast<int>((coord[1] + 1.) * height / 2),
            static_cast<int>((coord[2] + 1.) * 255. / 2)   
        };
    }

};