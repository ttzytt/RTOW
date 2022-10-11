#pragma once
#include "../hittables/hittables.h"
#include "../materials/materials.h"
#include "../noises/noises.h"
#include "../rtow.h"
#include "../textures/textures.h"
#include "scene.h"

scene rand_mul_sphere_mat() {
	hittable_list world;
	// auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	// world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000, ground_material));

	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1),
												color(0.9, 0.9, 0.9));
	world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000,
								  make_shared<lambertian>(checker)));

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

	//---------------- 场景设置

	const f4 asp_ratio = 3.0 / 2.0;
	const int coeff = 1;
	pt3 lookfrom(coeff * 13, coeff * 2, coeff * 3);
	pt3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0;
	auto cam_ptr = make_shared<camera>(lookfrom, lookat, vup, 20, asp_ratio,
									   aperture, dist_to_focus, 0.0, 1.0);
	//----------------------- 相机设置
	return scene(make_shared<bvh_node>(world), blue_sky_back_ptr, cam_ptr);
}

scene perlin_noise_sphere() {
	hittable_list world;
	auto ns = make_shared<perlin_noise<256>>(1);
	auto terb =
		make_shared<terbulence>(std::vector<f8>{1, 2, 4, 8, 16},
								std::vector<f8>{1, 0.5, .25, .125, .0625}, ns);
	// auto terb = make_shared<terbulence>(4, ns);
	auto noise_text = make_shared<marblelike>(terb);
	// auto noise_text = make_shared<noise_texture>(marb);
	world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000,
								  make_shared<lambertian>(noise_text)));
	world.add(make_shared<sphere>(pt3(0, 2, 0), 2,
								  make_shared<lambertian>(noise_text)));

	//-----------------场景设置

	const f4 asp_ratio = 3.0 / 2.0;
	const int coeff = 1;
	pt3 lookfrom(coeff * 13, coeff * 2, coeff * 3);
	pt3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0;
	auto cam_ptr = make_shared<camera>(lookfrom, lookat, vup, 20, asp_ratio,
									   aperture, dist_to_focus, 0.0, 1.0);
	//----------------------- 相机设置

	return scene(make_shared<bvh_node>(world), blue_sky_back_ptr, cam_ptr);
}

scene earth_img_sphere() {
	auto earth_text =
		make_shared<image_texture>("/mnt/e/prog/c++/RTOW/imgs/earthmap.jpg");
	auto earth_material = make_shared<lambertian>(earth_text);
	auto e_sphere = make_shared<sphere>(pt3(0, 0, 0), 2, earth_material);
	//-----------------场景设置
	const f4 asp_ratio = 3.0 / 2.0;
	const int coeff = 1;
	pt3 lookfrom(coeff * 13, coeff * 2, coeff * 3);
	pt3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0;
	auto cam_ptr = make_shared<camera>(lookfrom, lookat, vup, 20, asp_ratio,
									   aperture, dist_to_focus, 0.0, 1.0);
	//-----------相机设置
	return scene(make_shared<bvh_node>(hittable_list(e_sphere)),
				 blue_sky_back_ptr, cam_ptr);
}

scene light_emit_rect() {
	hittable_list world;
	auto marble = make_shared<marblelike>();  // 纹理
	world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000,
								  make_shared<lambertian>(marble)));
	world.add(
		make_shared<sphere>(pt3(0, 2, 0), 2, make_shared<lambertian>(marble)));
	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1) * 3,
												color(0.9, 0.9, 0.9 * 3));
	auto diff_light_mat = make_shared<diffuse_light>(checker);
	world.add(make_shared<rect_slice<2>>(3, 5, 1, 4, -2, diff_light_mat));
	//--------------- 场景设置
	const f4 asp_ratio = 3.0 / 2.0;
	pt3 lookfrom(26, 3, 6);
	pt3 lookat(0, 2, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0;

	auto cam_ptr = make_shared<camera>(lookfrom, lookat, vup, 20, asp_ratio,
									   aperture, dist_to_focus, 0.0, 1.0);
	// 相机设置
	scene ret(make_shared<bvh_node>(world), pure_black_back_ptr, cam_ptr);
	return ret;
}

scene cornell_box() {
	hittable_list world;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	auto cbox = make_shared<box>(pt3(0), pt3(555), (std::array<shared_ptr<material>, 6>){green, red, white, white, nullptr, white});
	world.add(cbox);
	world.add(make_shared<rect_slice<1>>(213, 343, 227, 332, 554, light));

	const f4 asp_ratio = 1;
	pt3 lookfrom(278, 278, -800);
	pt3 lookat(278, 278, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0;
	f8 vfov = 40.0;

	auto cam_ptr = make_shared<camera>(lookfrom, lookat, vup, vfov, asp_ratio,
									   aperture, dist_to_focus, aperture, 1.0);
	return scene(make_shared<bvh_node>(world), pure_black_back_ptr, cam_ptr);
};