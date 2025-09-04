#include "util.h"

#include "model.h"
#include <cstdint>

constexpr TGAColor white =  {.bgra = {255, 255, 255, 255 }};
constexpr TGAColor green =  {.bgra = {0, 255, 0, 255 }};
constexpr TGAColor red =  {.bgra = {0, 0, 255, 255 }};
constexpr TGAColor blue =  {.bgra = {255, 128, 64, 255 }};
constexpr TGAColor yellow =  {.bgra = {0, 200, 255, 255 }};
constexpr int width = 800;
constexpr int height = 800;

mat<4,4> model_view, view_port, persp;

void rasterize(const vec4 clip[3], std::vector<double> &zbuffer, TGAImage &framebuffer, const TGAColor color) {
    vec4 ndc[3]    = { clip[0]/clip[0].w, clip[1]/clip[1].w, clip[2]/clip[2].w };                
    vec2 screen[3] = { (view_port*ndc[0]).xy(), (view_port*ndc[1]).xy(), (view_port*ndc[2]).xy() }; 

    mat<3,3> abc = {{ {screen[0].x, screen[0].y, 1.}, {screen[1].x, screen[1].y, 1.}, {screen[2].x, screen[2].y, 1.} }};
    if (abc.det()<1) return; 

    auto [min_x, max_x] = std::minmax({screen[0].x, screen[1].x, screen[2].x}); 
    auto [min_y, max_y] = std::minmax({screen[0].y, screen[1].y, screen[2].y}); 

#pragma omp parallel for
        for (int x=std::max<int>(min_x, 0); x<=std::min<int>(max_x, framebuffer.width()-1); x++) { 
            for (int y=std::max<int>(min_y, 0); y<=std::min<int>(max_y, framebuffer.height()-1); y++) {
                vec3 bc = abc.invert_transpose() * vec3{static_cast<double>(x), static_cast<double>(y), 1.}; 
                if (bc.x<0 || bc.y<0 || bc.z<0) continue;                                                    
                double z = bc * vec3{ ndc[0].z, ndc[1].z, ndc[2].z };

                if (z <= zbuffer[x+y*framebuffer.width()]) continue;

                zbuffer[x+y*framebuffer.width()] = z;

                framebuffer.set(x, y, color);
            }
        }
}

int main() {
    TGAImage framebuffer(width, height, TGAImage::RGB);
    
    constexpr vec3    eye{-1,0,2}; 
    constexpr vec3 center{0,0,0};  
    constexpr vec3     up{0,1,0};  

    model_view = lookat(eye, center, up);                              
    persp = perspective(norm(eye-center));                        
    view_port = viewport(width/16, height/16, width*7/8, height*7/8); 
    
    model model;

    if (!model.load_model("obj\\diablo3_pose.obj")) {
        std::cout << "model not loaded" << std::endl;
        return 1;
    }

    std::vector<double> zbuffer(width*height, -std::numeric_limits<double>::max());

    const int size = model.faces.size() / 3;
    for (int i = 0; i < size; ++i) {
        vec4 clip[3];
        for (int d : {0,1,2}) {            // assemble the primitive
            vec3 v = model.vertices[model.faces[i*3+d]];
            clip[d] = persp * model_view * vec4{v.x, v.y, v.z, 1.};
        }
        TGAColor rnd;
        for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
        rasterize(clip, zbuffer, framebuffer, rnd);
    }


    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}