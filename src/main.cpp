//#include "util.h"
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

    model model;

    model.draw_model(model, framebuffer, width, height);

    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}