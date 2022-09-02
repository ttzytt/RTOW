#include <iostream>
#include <fstream>

#include "src/camera.h"
#include "src/hittables/hittable_list.h"
#include "src/hittables/sphere.h"
#include "src/material.h"
#include "src/materials/materials.h"
#include "src/rtow.h"
#include "src/render.h"

hittable_list rand_scene() {
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            f8 choose_mat = rand_f8();
            pt3 center(a + 0.9 * rand_f8(), 0.2, b + 0.9 + rand_f8());
            if ((center - pt3(4, 0.2, 0)).len() > 0.9) {
                // 距离第一个大球远
                shared_ptr<material> sphere_mat;
                if (choose_mat < 0.5) {
                    color albedo = color::rand() * color::rand();
                    sphere_mat = make_shared<lambertian>(albedo);

                } else if (choose_mat < 0.82) {
                    color albedo = color::rand(0.5, 1);
                    f8 fuzz = rand_f8(0, 0.5);
                    sphere_mat = make_shared<metal>(albedo, fuzz);
                } else {
                    f8 fuzz = rand_f8();
                    if (fuzz < 0.7) {
                        fuzz = 0;
                    }
                    color albedo = color::rand(0.88, 1);

                    sphere_mat = make_shared<dielectric>(rand_f8(1.3, 1.7),
                                                         albedo, fuzz);
                }
                world.add(make_shared<sphere>(center, 0.2, sphere_mat));
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(pt3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(pt3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(pt3(4, 1, 0), 1.0, material3));

    return world;
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        cerr << "usage: <filename>\n";
    }  
    
    ofstream outfile;
    outfile.open(argv[1]);
    
    // img
    const f4 asp_ratio = 3.0 / 2.0;
    const int wid = 2500;
    const int hei = round(wid / asp_ratio);
    const int sample_per_pixel = 500;
    const int max_dep = 120;

    // cam
    pt3 lookfrom(13,2,3);
    pt3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0;

    camera cam(lookfrom, lookat, vup, 20, asp_ratio, aperture, dist_to_focus);

    hittable_list world = rand_scene();

    render::out_ppm(render::out_color_map(cam, world, wid, hei, sample_per_pixel, max_dep), outfile);
}