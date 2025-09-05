#pragma once

#include <cmath>
#include <cstdlib>
#include <tuple>
#include <vector>
#include <algorithm>

#include "tgaimage.h"
#include "types.h"
#include <cstring>

#define tup(...) std::tuple<__VA_ARGS__>
#define create_tup(...) std::make_tuple(__VA_ARGS__)
#define PI 3.14159265358979323846

inline double triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5*((by - ay) * (bx + ax) + (cy - by) * (cx + bx) + (ay - cy) * (ax + cx));
}

inline void swap(int &a, int &b) noexcept {
    int temp = a;
    a = b;
    b = temp;
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