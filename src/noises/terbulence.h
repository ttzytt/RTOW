#pragma once
#include <exception>
#include <vector>

#include "../noise.h"
#include "../rtow.h"
#include "./interpo_mapers.h"

f8 mx_ret = 0;

class terbulence : public noise {
   public:
	f8 noise_coeff(const pt3& pt) const override {
		// 很多不同频率的噪声叠加在一起，比如 sinx + sin2x + sin3x 的效果
		// 这里 pt 就对应上面函数的 pt
		f8 ret = 0;
		for (int i = 0; i < cnt; i++) {
			ret += wts[i] * (base->noise_coeff(freqs[i] * pt) * 2 - 1);
		}
		return fabs(ret) ;
		// 换成绝对值后，特别暗和特别亮的都是亮的
		// 只有原来过渡的地方是暗的，感觉是把明暗交界的边描
	}

	terbulence(int _cnt, shared_ptr<noise> _base) : cnt(_cnt), base(_base) {
		wt_tot = pow(2, cnt);
		f8 cur = 1;
		// wts = 0.5, 0.25, 0.125 ...
		freqs.resize(cnt), wts.resize(cnt);
		for (int i = 0; i < cnt; i++) {
			freqs[i] = cur;
			wts[i] = cur;
			cur *= 2;
		}
	}

	terbulence(const std::vector<f8>& _freqs, const std::vector<f8>& _wts,
			   shared_ptr<noise> _base)
		: freqs(_freqs), wts(_wts), base(_base) {
		if (freqs.size() != wts.size())
			throw std::invalid_argument(
				"size of frequency vec != size of weight vec");
		for (f8 cur_w : wts) wt_tot += cur_w;
		cnt = wts.size();
	}

	shared_ptr<noise> base;
	int cnt;				// 要叠加多少个的频率
	std::vector<f8> freqs;	// 所有需要的频率
	std::vector<f8> wts;	// 每个频率对应的权重
	f8 wt_tot;
};