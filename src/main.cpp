#include "util.h"
//#include <ctime>

#include "model.h"

constexpr TGAColor white =  {.bgra = {255, 255, 255, 255 }};
constexpr TGAColor green =  {.bgra = {0, 255, 0, 255 }};
constexpr TGAColor red =  {.bgra = {0, 0, 255, 255 }};
constexpr TGAColor blue =  {.bgra = {255, 128, 64, 255 }};
constexpr TGAColor yellow =  {.bgra = {0, 200, 255, 255 }};
constexpr int width = 800;
constexpr int height = 800;

int main() {
    TGAImage framebuffer(width, height, TGAImage::RGB);

    // model model;

    // model.draw_model(model, framebuffer, width, height);

    // int ax = 17, ay =  4, az =  13;
    // int bx = 55, by = 39, bz = 128;
    // int cx = 23, cy = 59, cz = 255;

    // filled_triangle(ax, ay, az, bx, by, bz, cx, cy, cz, framebuffer, red);
    model model;

    model.draw_model(framebuffer, width, height);
    //framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}