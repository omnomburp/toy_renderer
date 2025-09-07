#include <cstdint>
#include <cstdlib>
#include "our_gl.h"
#include "model.h"
#include "types.h"

extern mat<4,4> ModelView, Perspective;

struct PhongShader : IShader {
    vec4 l;
    // vec3 triangle[3];
    // vec3 nrm[3];
    vec2 varying_uv[3];
    const Model& model;

    PhongShader(const vec3& light, Model& model) : model(model) {
        vec4 l4 = { light.x, light.y, light.z, 1.}; // turn light to a vec4
        l = normalized((ModelView * l4));   // get the direction of the light vector
    }

    virtual std::pair<bool,TGAColor> fragment(const vec3 bar) const {
        vec2 uv = varying_uv[0] * bar[0] + varying_uv[1] * bar[1] + varying_uv[2] * bar[2];
        TGAColor gl_FragColor = model.diff(uv);     
        vec4 n = normalized(ModelView.invert_transpose() * model.normal(uv));
        vec4 r = normalized(n * (n * l)*2 - l);                   // reflected light direction                   
        double ambient = .3;                                      
        double diff = std::max(0., n * l);                        
        double spec = std::pow(std::max(r.z, 0.) * (model.spec(uv) * model.spec(uv)), 35);            
        for (int channel : {0,1,2})
            gl_FragColor[channel] *= std::min(1., ambient + .4*diff + .6*spec);
        return {false, gl_FragColor};
    }

    virtual vec4 vertex(const int face, const int n) {
        varying_uv[n] = model.uv(face, n);
        vec4 gl_Position = ModelView * model.vert(face, n);
        return Perspective * gl_Position; // clip coordinates
    }
};


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }

    constexpr int width  = 800;      // output image size
    constexpr int height = 800;
    constexpr vec3  light{1, 1, 1};
    constexpr vec3    eye{-1, 0, 2}; // camera position
    constexpr vec3 center{ 0, 0, 0}; // camera direction
    constexpr vec3     up{ 0, 1, 0}; // camera up vector

    lookat(eye, center, up);                                   // build the ModelView   matrix
    init_perspective(norm(eye-center));                        // build the Perspective matrix
    init_viewport(width/16, height/16, width*7/8, height*7/8); // build the Viewport    matrix
    init_zbuffer(width, height);

    TGAImage framebuffer(width, height, TGAImage::RGB, {0, 0, 0, 255});

    for (int m=1; m<argc; m++) {                    // iterate through all input objects
        Model model(argv[m]);
        PhongShader shader(light, model);                       // load the data
        for (int f=0; f<model.nfaces(); f++) {      // iterate through all facets
            Triangle clip = { shader.vertex(f, 0),  // assemble the primitive
                              shader.vertex(f, 1),
                              shader.vertex(f, 2) };
            rasterize(clip, shader, framebuffer);   // rasterize the primitive
        }
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
