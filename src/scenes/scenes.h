#pragma once
#include "../hittables/hittables.h"
#include "../materials/materials.h"
#include "../texture.h"
#include "../textures/textures.h"
#include "../rtow.h"
#include "../noises/noises.h"

bvh_node rand_mul_sphere_mat() {
    hittable_list world;
    // auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000, ground_material));

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(pt3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

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
                    pt3 center_ed = center + pt3(0, rand_f8(0, .5), 0);
                    world.add(make_shared<moving_sphere>(center, center_ed, 0,
                                                         1, 0.2, sphere_mat));
                    continue;
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

    return bvh_node(world, 0, infinity);
    // return world;
}


bvh_node perlin_noise_sphere(){
    hittable_list ret;
    auto ns = make_shared<perlin_noise<256>>(1);
    auto terb = make_shared<terbulence>(std::vector<f8>{1, 2, 4, 8, 16}, std::vector<f8>{1, 0.5, .25, .125, .0625}, ns);
    // auto terb = make_shared<terbulence>(4, ns);
	auto noise_text = make_shared<marblelike>(terb);
	// auto noise_text = make_shared<noise_texture>(marb);
	ret.add(make_shared<sphere>(pt3(0,-1000,0), 1000, make_shared<lambertian>(noise_text)));
    ret.add(make_shared<sphere>(pt3(0, 2, 0), 2, make_shared<lambertian>(noise_text)));
    return bvh_node(ret, 0, infinity);
}

bvh_node earth_img_sphere() {
	auto earth_text =
		make_shared<image_texture>("/mnt/e/prog/c++/RTOW/imgs/earthmap.jpg");
	auto earth_material = make_shared<lambertian>(earth_text);
    auto e_sphere = make_shared<sphere>(pt3(0,0,0), 2, earth_material);
    return bvh_node(hittable_list(e_sphere), 0, infinity);
}

bvh_node light_emit_rect(){
    hittable_list world;
    auto marble = make_shared<marblelike>(); // 纹理
    world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000, make_shared<lambertian>(marble)));
	world.add(make_shared<sphere>(pt3(0, 2, 0), 2,
								  make_shared<lambertian>(marble)));
	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1) * 3,
											   color(0.9, 0.9, 0.9 * 3));
	auto diff_light_mat = make_shared<diffuse_light>(checker);
    world.add(make_shared<rect_slice<2>>(3, 5, 1, 4, -2, diff_light_mat));
    // return world;
    return bvh_node(world, 0, infinity);
}