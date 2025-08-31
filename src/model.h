#pragma once

#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "tgaimage.h"
#include "util.h"

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

    inline void draw_model(model &model, TGAImage &framebuffer, TGAColor color, const int width, const int height) {
        if (model.load_model("obj\\diablo3_pose.obj")) {
            std::cout << "model loaded" << std::endl;        

            const int model_face_size = model.faces.size() / 3;

            for (int i = 0; i < model_face_size; ++i) {
                const auto face = &model.faces[i * 3];
                vec3 coords[3] = { model.vertices[*face], model.vertices[*(face + 1)], model.vertices[*(face + 2)]};
                draw_triangle(framebuffer, coords, color, width, height);
            }

            framebuffer.write_tga_file("framebuffer.tga");
        }
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

    inline std::tuple<int, int> project(const vec3 coord, const int width, const int height) noexcept {
        return {
            static_cast<int>((coord.x + 1.) * width / 2),
            static_cast<int>((coord.y + 1.) * height / 2)   
        };
    }

    inline void draw_triangle(TGAImage &framebuffer, const vec3 coords[3], const TGAColor color, const int width, const int height) noexcept {
        auto [ax, ay] = project(coords[0], width, height);
        auto [bx, by] = project(coords[1], width, height);
        auto [cx, cy] = project(coords[2], width, height);
        draw_line(ax, ay, bx, by, framebuffer, color);
        draw_line(bx, by, cx, cy, framebuffer, color);
        draw_line(cx, cy, ax, ay, framebuffer, color);
    }
};