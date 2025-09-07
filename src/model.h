#pragma once

#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <array>

#include "tgaimage.h"
#include "util.h"
#include "types.h"

struct Model {
    std::vector<vec4> vertices = {};
    std::vector<vec4> normals  = {};
    std::vector<vec2> textures = {};
    std::vector<int> facet_vrt = {}; 
    std::vector<int> facet_nrm = {}; 
    std::vector<int> facet_tex = {}; 
    TGAImage normalmap         = {};

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

                vec4 vertex = {
                    x,
                    y,
                    z,
                    1.
                };
                vertices.emplace_back(vertex);

            } else if (line.rfind("f ", 0) == 0) {
                char f;
                std::string v1, v2, v3;
                iss >> f >> v1 >> v2 >> v3;

                auto parse_index = [this](const std::string& token) {
                    constexpr char key = '/';

                    int count = 0, prev_index = 0;
                    
                    for (int i = 0; i < token.size(); ++i) {
                        if (token[i] == key) {
                            if (count == 0) {
                                facet_vrt.push_back(std::stoi(token.substr(prev_index, i)) - 1);
                                prev_index = i;
                            } else if (count == 1) {
                                facet_tex.push_back(std::stoi(token.substr(prev_index + 1, i - prev_index + 1)) - 1);
                                facet_nrm.push_back(std::stoi(token.substr(i + 1, token.size() - i + 1)) - 1);
                            }

                            ++count;
                        }
                    }
                };

                // -1 due to the file format storing indexes
                parse_index(v1);
                parse_index(v2);
                parse_index(v3);

            } else if (line.rfind("vn", 0) == 0) {
                char vn;
                float x, y, z;
                iss >> vn >> vn >> x >> y >> z;
                vec4 normal = {
                    x,
                    y,
                    z,
                };
                normals.emplace_back(normal);
            } else if (line.rfind("vt", 0) == 0) {
                char vt;
                float x, y, z;

                iss >> vt >> vt >> x >> y >> z;
                vec2 texture {
                    x,
                    1. - y,
                };

                textures.emplace_back(texture);
            }
        }

        auto load_texture = [&file_path](const std::string suffix, TGAImage &img) {
            size_t dot = file_path.find_last_of(".");
            if (dot==std::string::npos) return;
            std::string texfile = file_path.substr(0,dot) + suffix;
            std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        };
        load_texture("_nm.tga", normalmap);
    }

    inline int nverts() const { return vertices.size(); }
    inline int nfaces() const { return facet_vrt.size()/3; }

    inline vec4 vert(const int i) const {
        return vertices[i];
    }

    inline vec4 vert(const int iface, const int nthvert) const {
        return vertices[facet_vrt[iface*3+nthvert]];
    }

    inline vec4 normal(const int iface, const int nthvert) const {
        return vertices[facet_nrm[iface*3+nthvert]];
    }

    inline vec4 normal(const vec2& uv) const {
        TGAColor c = normalmap.get(uv[0]*normalmap.width(), uv[1]*normalmap.height());
        return vec4{(double)c[2],(double)c[1],(double)c[0],0}*2./255. - vec4{1,1,1,0};
    }

    inline vec2 uv(const int iface, const int nthvert) const {
        return textures[facet_tex[iface*3+nthvert]];
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