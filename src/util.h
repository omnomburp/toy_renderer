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

inline std::vector<tup(int, int)> draw_line_with_coords(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) noexcept {
    std::vector<tup(int, int)> res;

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
            res.push_back(create_tup(y, x));
        } else {
            framebuffer.set(x, y, color);
            res.push_back(create_tup(x, y));
        }
            
        error += 2 * std::abs(by - ay);

        const int mask = error > bx - ax;
        y += (by > ay ? 1 : -1) * mask;
        error -= 2 * (bx - ax) * mask;
    }

    return res;
}

inline void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) noexcept {
    // sort by highest y axis
    if (ay < by) {
        swap(ay, by);
        swap(ax, bx);
    }

    if (by < cy) {
        swap(by, cy);
        swap(bx, cx);
    } 

    if (ay < cy) {
        swap(ay, cy);
        swap(ax, cx);
    }

    // draw lines between the equal y axis from top down

    auto longest = draw_line_with_coords(ax, ay, cx, cy, framebuffer, color); // longest line has all the y axis possible

    // need all the y axis from these 2
    auto middle = draw_line_with_coords(ax, ay, bx, by, framebuffer, color);
    auto shortest = draw_line_with_coords(bx, by, cx, cy, framebuffer, color);

    middle.insert(middle.end(), shortest.begin(), shortest.end());

    for (int i = 0; i < longest.size(); ++i) {
        draw_line(std::get<0>(longest[i]), std::get<1>(longest[i]), std::get<0>(middle[i]), std::get<1>(middle[i]), framebuffer, color);
    }
}