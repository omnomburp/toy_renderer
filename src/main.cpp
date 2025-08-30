#include "tgaimage.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

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

inline int ret_abs(int x) {
    if (x < 0) {
        return -x;
    }

    return x;
}

inline void draw_line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    auto steep = ret_abs(ax - bx) < ret_abs(ay - by);

    if (steep) {
        swap(ax, ay);
        swap(bx, by);
    }


    if (ax > bx) {
        swap(ax, bx);
    }

    for (int x = ax; x < bx; ++x) {
        float t = (x - ax) / static_cast<float>((bx - ax));

        int x_coord = std::round(ax + t*(bx - ax));
        int y_coord = std::round(ay + t*(by - ay));
        if (!steep)
            framebuffer.set(x_coord, y_coord, color);
        else
            framebuffer.set(y_coord, x_coord, color);

    }
}

int main() {
    constexpr int width = 64;
    constexpr int height = 64;

    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::srand(std::time({}));
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width, ay = rand()%height;
        int bx = rand()%width, by = rand()%height;
        draw_line(ax, ay, bx, by, framebuffer, { static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255) });
    }

    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}