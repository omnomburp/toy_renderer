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

inline const mat<4, 4> viewport(const int x, const int y, const int w, const int h) {
    return {{{(w/2.), 0, 0, (x+w/2.)}, {0, (h/2.), 0, (y+h/2.)}, {0,0,1,0}, {0,0,0,1}}};
}

inline const mat<4, 4> perspective(const double f) {
    return {{{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0, (float)(-1/f),1}}};
}

inline const mat<4, 4> lookat(const vec3 eye, const vec3 center, const vec3 up) {
    vec3 n = normalized(eye-center);
    vec3 l = normalized(cross(up,n));
    vec3 m = normalized(cross(n, l));
    auto res = mat<4,4>{{{l.x,l.y,l.z,0}, {m.x,m.y,m.z,0}, {n.x,n.y,n.z,0}, {0,0,0,1}}} *
                mat<4,4>{{{1,0,0,-center.x}, {0,1,0,-center.y}, {0,0,1,-center.z}, {0,0,0,1}}};

    return res;
}