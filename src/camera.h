#pragma once
#include "rtow.h"

class camera {
   public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup = vec3(0, 1, 0), f8 vfov = 90,
           f8 asp_ratio = 16.0 / 9.0, f8 aperture = 0, f8 foc_len = 1,
           f8 _t_open = 0, f8 _t_close = 0) {
        f8 deg_fov = deg2rad(vfov);
        f8 half_hei =
            tan(deg_fov / 2);  // 对边比临边，但是临边是 1，也就是和相机的距离为
                               // 1 时，画面的大小
        f8 half_wid = half_hei * asp_ratio;

        cam_z = (lookfrom - lookat).unit_vec();
        // z 和镜头指向位置是反的
        cam_x = cross(vup, cam_z).unit_vec();  // 同时和 vup，z 垂直
        cam_y = vup.unit_vec();

        horizon = 2 * half_wid * cam_x * foc_len;  // 焦平面的横竖边框
        vertic =
            2 * half_hei * cam_y * foc_len;  // 乘 foc_len 是按照 foc_len 放大
        /*
                *(vertic)
                |
                |
                |
                *------(orig)------*(horizon)
                |
                |
                |
                *(左下角)
        */
        // orig 是中心
        orig = lookfrom;

        lower_left_corner = orig - horizon / 2 - vertic / 2 -
                            cam_z * foc_len;  // 用于定位渲染时扫描的表面
        // orig - z 移动到 lookat 方向
        // cam_z 是一个代表向量的方向，乘以 foc_len 会把这个面移动到焦平面

        len_radius = aperture / 2;

        t_open = _t_open;
        t_close = _t_close;
    }

    inline ray get_ray(f8 x, f8 y) const {
        // x 和 y 的范围：[0, 1]
        // 相机传感器的像素点坐标
        vec3 rd = len_radius * rand_unit_disk();
        vec3 offset = cam_x * rd.x() + cam_y * rd.y();

        ray r;
        r.orig = orig + offset;
        r.dir = lower_left_corner + x * horizon + y * vertic - orig - offset;
        r.tm = rand_f8(t_open, t_close);
        // 产生一个从 orig + offset 到对应像素的向量
        // 因为 ray 对应的就是 orig + t * dir
        return r;
    }

    vec3 orig;               // 摄像机的位置
    vec3 lower_left_corner;  // 画面的左下角
    vec3 horizon, vertic;  // 画面的尺寸（或者说离相机 foc_len 的平面大小）
    vec3 cam_x, cam_y, cam_z;  // 相机朝向
    f8 len_radius;             // 光圈半径
    f8 t_open, t_close;        // 快门时间
};