#include <cmath>
#include <cstdlib>
//#include <ctime>

#include "tgaimage.h"
#include "model.h"

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

inline void draw_triangle(TGAImage &framebuffer, triple_int coords[3], TGAColor color) {
    draw_line(coords[0].x, coords[0].y, coords[1].x, coords[1].y, framebuffer, color);
    draw_line(coords[2].x, coords[2].y, coords[1].x, coords[1].y, framebuffer, color);
    draw_line(coords[2].x, coords[2].y, coords[0].x, coords[0].y, framebuffer, color);
}

int main() {
    constexpr int width = 2000;
    constexpr int height = 2000;

    TGAImage framebuffer(width, height, TGAImage::RGB);

    model model;

    if (model.load_model("obj\\diablo3_pose.obj")) {
        std::cout << "model loaded" << std::endl;
        
        for (const auto& face : model.faces) {
            triple_int coords[3] = { model.vertices[face.x - 1], model.vertices[face.y - 1], model.vertices[face.z - 1] };
            draw_triangle(framebuffer, coords, red);
        }

        framebuffer.write_tga_file("framebuffer.tga");
    }

    

    return 0;
}