#include <fstream>
#include <iostream>
#define DBG_MACRO_DISABLE
#include "src/backgrounds/backgrounds.h"
#include "src/camera.h"
#include "src/hittables/hittables.h"
#include "src/materials/materials.h"
#include "src/render.h"
#include "src/rtow.h"
#include "src/scenes/scenes.h"
using namespace std;

int main(int argc, char* argv[]) {
	auto start_tm = chrono::steady_clock::now();

	if (argc != 2) {
		cerr << "usage: <filename>\n";
	}

	ofstream outfile;
	outfile.open(argv[1]);

	// img
	const f4 asp_ratio = 1;
	const int wid = 300;
	int mx_th = std::thread::hardware_concurrency();
	render::config conf{
		.wid = wid,
		.hei = round((f8)wid / asp_ratio),
		.sample_per_pix = 500,
		.max_dep = 120,
		.th_cnt = mx_th,
	};

	cornell_box().out_ppm(outfile, conf);

	auto end_tm = chrono::steady_clock::now();

	auto duration_mili =
		std::chrono::duration_cast<chrono::milliseconds>(end_tm - start_tm);
	cerr << "render time: " << duration_mili.count() << '\n';
}