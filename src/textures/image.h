#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include "../externals/stb_image.h"
#include "../texture.h"

class image_texture : public texture {
	// 贴图类
   public:
	const static int channel_p_pixel = 3;
	image_texture(const char* filename) {
		int actual_channel_p_pix = channel_p_pixel;
		data = stbi_load(filename, &wid, &hei, &actual_channel_p_pix,
						 channel_p_pixel);
		if (!data) {
			throw std::runtime_error("cannot open file");
			wid = hei = 0;
		}
		byte_p_scanline = actual_channel_p_pix * wid;
	}

	~image_texture() { delete data; }

	virtual color value(f8 polar, f8 azim, const pt3& p) const override {
		if (data == nullptr) return color(0.114, 0.514, 0.1919);
		polar = 1 - polar;	// 图像的坐标系相反，最上方的是 0
		int i = round(azim * wid);
		int j = round(polar * hei);
		i >= wid ? i-- : 0;
		j >= hei ? j-- : 0;

		unsigned char* pix = data + j * byte_p_scanline + i * channel_p_pixel;
		return color((f8)pix[0] / (f8)MX_COLOR, (f8)pix[1] / (f8)MX_COLOR,
					 (f8)pix[2] / (f8)MX_COLOR);
	}

	unsigned char* data;
	int wid, hei;
	int byte_p_scanline;
};