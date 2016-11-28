//
// Created by Milena on 02.10.2016.
//

#include "primitives.h"
#include <algorithm>

void brezenchem_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        steep = true;
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    dx = x1 - x0;
    dy = y1 - y0;
    int k = abs(dy);
    int error = 0;
    int pace = dy != 0 ? dy / abs(dy) : 0;
    for (int x = x0; x <= x1; x++) {
        if (!steep)
            image.set(x, y0, color);
        else
            image.set(y0, x, color);
        error += k;
        if (error * 2 > dx) {
            y0 += pace;
            error -= abs(dx);
        }
    }
}

void triangle(Vec3i v0, Vec3i v1, Vec3i v2, Vec3i vt0, Vec3i vt1, Vec3i vt2, TGAImage &image, float intensity, TGAImage &texture, int *zbuffer, int width) {
    TGAColor color(0, 255, 0, 255);

    //sorting vertices so v0.y is lowest
    if (swap_by_cond(v0, v1)) {
        std::swap(vt0, vt1);
    }
    if (swap_by_cond(v0, v2)) {
        std::swap(vt0, vt2);
    }
    if (swap_by_cond(v1, v2)) {
        std::swap(vt1, vt2);
    }

    if (v1.x < v2.x && v1.y == v2.y) {
        std::swap(v1, v2);
        std::swap(vt1, vt2);
    }
    if (v0.x > v1.x && v0.y == v1.y) {
        std::swap(v0, v1);
        std::swap(vt0, vt1);
    }
    //brezenchem_line(v0.x, v0.y, v1.x, v1.y, image, color);
    //brezenchem_line(v1.x, v1.y, v2.x, v2.y, image, color);
    //brezenchem_line(v2.x, v2.y, v0.x, v0.y, image, color);

    double dx20 = (v2.y == v0.y) ? 0 : (double) (v2.x - v0.x) / (double) (v2.y - v0.y);
    double dx10 = (v1.y == v0.y) ? 0 : (double) (v1.x - v0.x) / (double) (v1.y - v0.y);
    double dx21 = (v2.y == v1.y) ? 0 : (double) (v2.x - v1.x) / (double) (v2.y - v1.y);

    double dz20 = (v2.y == v0.y) ? 0 : (double) (v2.z - v0.z) / (double) (v2.y - v0.y);
    double dz10 = (v1.y == v0.y) ? 0 : (double) (v1.z - v0.z) / (double) (v1.y - v0.y);
    double dz21 = (v2.y == v1.y) ? 0 : (double) (v2.z - v1.z) / (double) (v2.y - v1.y);

    double dtx20 = (vt2.y == vt0.y) ? 0 : (double) (vt2.x - vt0.x) / (double) (vt2.y - vt0.y);
    double dtx10 = (vt1.y == vt0.y) ? 0 : (double) (vt1.x - vt0.x) / (double) (vt1.y - vt0.y);
    double dtx21 = (vt2.y == vt1.y) ? 0 : (double) (vt2.x - vt1.x) / (double) (vt2.y - vt1.y);


    double dx20_temp = dx20;
    double dz20_temp = dz20;
    double dtx20_temp = dtx20;

    if (swap_by_cond(dx20, dx10)) {
        std::swap(dz20, dz10);
        std::swap(dtx20, dtx10);
    }
    double x1 = v0.x;
    double x2 = x1;

    double z1 = v0.z;
    double z2 = z1;

    double xt1 = vt0.x;
    double xt2 = xt1;

    double dty = v1.y == v0.y ? 0 :(vt1.y - vt0.y) / (v1.y - v0.y);
    double dtx1 = dtx20 * dty;
    double dtx2 = dtx10 * dty;
    //first triangle drawing (lowest part)
    for (int y = v0.y, yt = vt0.y; y < v1.y; y++, yt += dty) {
        double tg_alpha = x2 == x1 ? 0 : (z2 - z1) / (x2 - x1);
        double dtx = x1==x2 ? 0 : (xt2 - xt1)/(x2-x1);
        for (int x = (int) ceil(x1), xt = (int) floor(xt1); x <= (int) floor(x2); x++, xt += dtx) {
            int idx = x + y * width;
            if (zbuffer[idx] < tg_alpha * (x - x1) + z1) {
                TGAColor t_color = texture.get(xt, yt);
                color = TGAColor(intensity*t_color.r, intensity*t_color.g, intensity*t_color.b, 255);
                zbuffer[idx] = tg_alpha * (x - x1) + z1;
                image.set(x, y, color);
            }
        }
        x1 += dx20;
        x2 += dx10;
        z1 += dz20;
        z2 += dz10;

        xt1 += dtx1;
        xt2 += dtx2;
    }

    if (v0.y == v1.y) {
        x1 = v0.x;
        x2 = v1.x;
        z1 = v0.z;
        z2 = v1.z;

        xt1 = vt0.x;
        xt2 = vt1.x;
    }

    if (swap_by_cond(dx21, dx20_temp)) {
        std::swap(dz21, dz20_temp);
        std::swap(dtx21, dtx20_temp);
    }

    dty = v2.y == v1.y ? 0 :(vt2.y - vt1.y) / (v2.y - v1.y);
    dtx1 = dtx20_temp * dty;
    dtx2 = dtx21 * dty;

    //second triangle drawing (highest part)
    for (int y = v1.y, yt = vt1.y; y < v2.y; y++, yt += dty) {
        double dtx = x1==x2 ? 0 : (xt2 - xt1)/(x2-x1);
        double tg_alpha = x2 == x1 ? 0 : (z2 - z1) / (x2 - x1);
        for (int x = (int) ceil(x1), xt = (int) floor(xt1); x <= (int) floor(x2); x++, xt += dtx) {
            int idx = x + y * width;
            if (zbuffer[idx] < tg_alpha * (x - x1) + z1) {
                TGAColor t_color = texture.get(xt, yt);
                color = TGAColor(intensity*t_color.r, intensity*t_color.g, intensity*t_color.b, 255);
                zbuffer[idx] = tg_alpha * (x - x1) + z1;
                image.set(x, y, color);
            }
        }
        x1 += dx20_temp;
        x2 += dx21;
        z1 += dz20_temp;
        z2 += dz21;

        xt1 += dtx1;
        xt2 += dtx2;
    }
}
