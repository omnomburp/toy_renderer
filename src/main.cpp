#include <cmath>
#include <cstdlib>
//#include <ctime>

#include "tgaimage.h"
#include "model.h"
#include <tuple>

constexpr TGAColor white =  {.bgra = {255, 255, 255, 255 }};
constexpr TGAColor green =  {.bgra = {0, 255, 0, 255 }};
constexpr TGAColor red =  {.bgra = {0, 0, 255, 255 }};
constexpr TGAColor blue =  {.bgra = {255, 128, 64, 255 }};
constexpr TGAColor yellow =  {.bgra = {0, 200, 255, 255 }};
constexpr int width = 800;
constexpr int height = 800;

inline std::tuple<int, int> project(vec3 coord) noexcept {
    return {
        static_cast<int>((coord.x + 1.) * width / 2),
        static_cast<int>((coord.y + 1.) * height / 2)   
    };
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

        if (steep)
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);

        error += 2 * std::abs(by - ay);

        const int mask = error > bx - ax;
        y += (by > ay ? 1 : -1) * mask;
        error -= 2 * (bx - ax) * mask;
    }
}

inline void draw_triangle(TGAImage &framebuffer, vec3 coords[3], TGAColor color) noexcept {
    auto [ax, ay] = project(coords[0]);
    auto [bx, by] = project(coords[1]);
    auto [cx, cy] = project(coords[2]);
    draw_line(ax, ay, bx, by, framebuffer, color);
    draw_line(bx, by, cx, cy, framebuffer, color);
    draw_line(cx, cy, ax, ay, framebuffer, color);
}

int main() {
    TGAImage framebuffer(width, height, TGAImage::RGB);

    model model;

    if (model.load_model("obj\\diablo3_pose.obj")) {
        std::cout << "model loaded" << std::endl;        
        std::cout << model.vertices.size() << " vertices" << std::endl;
        std::cout << model.faces.size() << " faces" << std::endl;

        const int model_face_size = model.faces.size() / 3;

        for (int i = 0; i < model_face_size; ++i) {
            const auto face = &model.faces[i * 3];
            vec3 coords[3] = { model.vertices[*face], model.vertices[*(face + 1)], model.vertices[*(face + 2)]};
            draw_triangle(framebuffer, coords, red);
        }

        framebuffer.write_tga_file("framebuffer.tga");
    }

    return 0;
}