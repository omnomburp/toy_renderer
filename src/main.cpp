#include <cmath>
#include <cstdlib>
#include <ctime>

#include "tgaimage.h"

constexpr TGAColor white =  {.bgra = {255, 255, 255, 255 }};
constexpr TGAColor green =  {.bgra = {0, 255, 0, 255 }};
constexpr TGAColor red =  {.bgra = {0, 0, 255, 255 }};
constexpr TGAColor blue =  {.bgra = {255, 128, 64, 255 }};
constexpr TGAColor yellow =  {.bgra = {0, 200, 255, 255 }};

inline void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

inline void draw_line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    auto steep = std::abs(ax - bx) < std::abs(ay - by);

    if (steep) {
        swap(ax, ay);
        swap(bx, by);
    }

    if (ax > bx) {
        swap(ax, bx);
    }

    int y = ay;
    int error = 0;

    for (int x = ax; x <= bx; ++x) {

        if (steep)
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);

        error += 2 * std::abs(by - ay);

        const int mask = error > bx - ax;
        y += (by > ay ? 1: -1) * mask;
        error -= 2 * (bx - ax) * mask;
    }
}

int main() {
    constexpr int width = 64;
    constexpr int height = 64;

    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::srand(std::time({}));
    for (int i = 0; i < (1 << 24); i++) {
        int ax = rand()%width, ay = rand()%height;
        int bx = rand()%width, by = rand()%height;
        draw_line(ax, ay, bx, by, framebuffer, { static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255) });
    }

    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}