#pragma once

#include <cstdlib>
#include <tuple>
#include <vector>

#include "tgaimage.h"
#include "types.h"

#define tup(...) std::tuple<__VA_ARGS__>
#define create_tup(...) std::make_tuple(__VA_ARGS__)

inline double triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5*((by - ay) * (bx + ax) + (cy - by) * (cx + bx) + (ay - cy) * (ax + cx));
}

inline void swap(int &a, int &b) noexcept {
    int temp = a;
    a = b;
    b = temp;
}

inline vec3 rot(vec3 v) noexcept {
    constexpr double a = 3.14159265358979323846 / 6.0;
    const mat3 ry = {{{ (float)std::cos(a), 0, (float)std::sin(a)}, {0,1,0}, { (float)-std::sin(a), 0, (float)std::cos(a)}}};

    return ry * v;
}

inline vec3 persp(vec3 v) noexcept {
    constexpr double c = 3.;
    return v / (1-v.z/c);
}

inline void draw_line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) noexcept {
    auto steep = std::abs(ax - bx) < std::abs(ay - by);

    if (steep) {
        swap(ax, ay);
        swap(bx, by);
    }

    if (ax > bx) {
        swap(ax, bx);
        swap(ay, by);
    }

    int y = ay;
    int error = 0;

    for (int x = ax; x <= bx; ++x) {

        if (steep) {
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
            
        error += 2 * std::abs(by - ay);

        const int mask = error > bx - ax;
        y += (by > ay ? 1 : -1) * mask;
        error -= 2 * (bx - ax) * mask;
    }
}

inline void filled_triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &framebuffer, TGAImage &zbuffer, TGAColor color) noexcept {
    int min_x = std::min(std::min(ax, bx), cx);
    int max_x = std::max(std::max(ax, bx), cx);
    int min_y = std::min(std::min(ay, by), cy);
    int max_y = std::max(std::max(ay, by), cy);

    const double total_area = triangle_area(ax, ay, bx, by, cx, cy);

    if (total_area < 1) return;

    #pragma omp parallel for
    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {

            const double a = triangle_area(x, y, bx, by, cx, cy) / total_area;
            const double b = triangle_area(ax, ay, x, y, cx, cy) / total_area;
            const double c = triangle_area(ax, ay, bx, by, x, y) / total_area;

            if (a < 0 || b < 0 || c < 0) {
                continue;
            }

            unsigned char z = static_cast<unsigned char>(a * az + b * bz + c * cz);

            if (z <= zbuffer.get(x, y)[0]) continue;
            
            zbuffer.set(x, y, {z, z, z, z});

            framebuffer.set(x, y, color);
        }
    }    
}