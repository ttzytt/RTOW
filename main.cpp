#include <fstream>
#include <iostream>
#define DBG_MACRO_DISABLE 
#include <dbg.h>
#include "src/camera.h"
#include "src/hittables/hittables.h"
#include "src/materials/materials.h"
#include "src/render.h"
#include "src/rtow.h"


#include "src/scenes/scenes.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "usage: <filename>\n";
    }

    ofstream outfile;
    outfile.open(argv[1]);

    // img
    const f4 asp_ratio = 3.0 / 2.0;
    const int wid = 300;
    const int hei = round(wid / asp_ratio);
    const int sample_per_pixel = 100;
    const int max_dep = 120;

    // cam
    const int coeff = 1;
    pt3 lookfrom(coeff * 13, coeff * 2, coeff * 3);
    pt3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0;

    camera cam(lookfrom, lookat, vup, 20, asp_ratio, aperture, dist_to_focus,
               0.0, 1.0);

    // hittable_list&& world = rand_mul_sphere_mat();
    hittable_list&& world = perlin_noise_sphere();
    render::out_ppm(
        render::out_color_map(cam, world, wid, hei, sample_per_pixel, max_dep, 1),
        outfile);
}