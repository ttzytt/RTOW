#include <immintrin.h>

#include <iostream>
#include <optional>

using f8 = double;
class vec {
   public:
	constexpr vec(f8 x = 0, f8 y = 0, f8 z = 0, f8 t = 0)
		: vec_data{x, y, z, t} {}
	__m256d vec_data;
};

class aabb {
   public:
	aabb(const vec& _mn, const vec& _mx) : mn(_mn), mx(_mx) {}
	vec mn, mx;
};

constexpr vec g_pt_by_axis(const f8 a, const f8 b, const f8 c, const int axis) {
	// cosnteval 只能编译期运行，constexpr 不是
	switch (axis) {
		case 0:
			return vec(c, a, b);
		case 1:
			return vec(a, c, b);
		case 2:
			return vec(a, b, c);
	}
	return vec(0);
}

class rect_slice {
   public:
	constexpr rect_slice(const f8 _a00, const f8 _a01, const f8 _a10,
						 const f8 _a11, const f8 _a2)
		: a00(_a00), a01(_a01), a10(_a10), a11(_a11), a2(_a2) {}
	aabb bounding_box(f8 tm0, f8 tm1) const {

		return aabb(g_pt_by_axis(a00, a10, a2 - 0.0001, 2),
					g_pt_by_axis(a01, a11, a2 + 0.0001, 2));
	}

	aabb bounding_box2(f8 tm0, f8 tmp1) const {
		vec&& mn = g_pt_by_axis(a00, a10, a2 - 0.0001, 2);
		vec&& mx = g_pt_by_axis(a01, a11, a2 + 0.0001, 2);
		return aabb(mn, mx);
	}
	f8 a00, a01, a10, a11, a2;
};

std::ostream& operator<<(std::ostream& out, const aabb& obj) {
	return out << obj.mn.vec_data[0] << ' ' << obj.mn.vec_data[1] << ' ' << obj.mn.vec_data[2] << ' '
			   << obj.mx.vec_data[0] << ' ' << obj.mx.vec_data[1] << ' ' << obj.mx.vec_data[2] << '\n';
}

int main() {
	rect_slice slice(1, 2, 4, 5, 6);
	std::cout << slice.bounding_box(0, 0) << '\n';
	std::cout << slice.bounding_box2(0, 0) << '\n';
}