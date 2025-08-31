#pragma once

#include <cstdlib>
#include <tuple>
#include <vector>

#include "tgaimage.h"

#define tup(...) std::tuple<__VA_ARGS__>
#define create_tup(...) std::make_tuple(__VA_ARGS__)

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

inline void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) noexcept {
    if (ay < by) {
        swap(ay, by);
        swap(ax, bx);
    }

    if (ay < cy) {
        swap(ay, cy);
        swap(ax, cx);
    }

    if (by < cy) {
        swap(by, cy);
        swap(bx, cx);
    } 

    const int total_height = ay - cy;

    if (ay != by) {
        const int segment_height = ay - by;

        for (int y = by; y <= ay; ++y) {
            int x1 = cx + ((ax - cx)*(y - cy)) / total_height;
            int x2 = bx + ((ax - bx)*(y - by)) / segment_height;
            
            for (int x = std::min(x1, x2); x < std::max(x1, x2); ++x) {
                framebuffer.set(x, y, color);
            }
        }
    }

    if (cy != by) {
        const int segment_height = by - cy;

        for (int y = cy; y <= by; ++y) {
            int x1 = cx + ((ax - cx)*(y - cy)) / total_height;
            int x2 = cx + ((bx - cx)*(y - cy)) / segment_height;
            
            for (int x = std::min(x1, x2); x < std::max(x1, x2); ++x) {
                framebuffer.set(x, y, color);
            }
        }
    }
}