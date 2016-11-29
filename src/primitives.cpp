//
// Created by Milena on 02.10.2016.
//

#include "primitives.h"
#include <algorithm>
#include <iostream>

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

void triangle(Vec3i v0, Vec3i v1, Vec3i v2, Vec3i uv0, Vec3i uv1, Vec3i uv2, TGAImage &image, float intensity,
              TGAImage &texture, int *zbuffer, int width) {
    if (v0.y==v1.y && v0.y==v2.y) return; // i dont care about degenerate triangles
    if (v0.y>v1.y) { std::swap(v0, v1); std::swap(uv0, uv1); }
    if (v0.y>v2.y) { std::swap(v0, v2); std::swap(uv0, uv2); }
    if (v1.y>v2.y) { std::swap(v1, v2); std::swap(uv1, uv2); }

    int total_height = v2.y-v0.y;
    int segment1_height = v1.y-v0.y;
    int segment2_height = v2.y-v1.y;

    int uv_total_height = v2.y-v0.y;
    int uv_segment1_height = v1.y-v0.y;
    int uv_segment2_height = v2.y-v1.y;

    Vec3f d20 = total_height==0 ?    Vec3f(0, 0, 0):Vec3f(v2-v0)/(double)total_height;
    Vec3f d10 = segment1_height==0 ? Vec3f(0, 0, 0):Vec3f(v1-v0)/(double)segment1_height;
    Vec3f d21 = segment2_height==0 ? Vec3f(0, 0, 0):Vec3f(v2-v1)/(double)segment2_height;

    Vec3f uv_d20 = uv_total_height==0 ?    Vec3f(0, 0, 0):Vec3f(uv2-uv0)/(double)uv_total_height;
    Vec3f uv_d10 = uv_segment1_height==0 ? Vec3f(0, 0, 0):Vec3f(uv1-uv0)/(double)uv_segment1_height;
    Vec3f uv_d21 = uv_segment2_height==0 ? Vec3f(0, 0, 0):Vec3f(uv2-uv1)/(double)uv_segment2_height;

    Vec3f left_f =                  Vec3f(v0);
    Vec3f right_f = (v1.y == v0.y) ? Vec3f(v1) : Vec3f(v0);

    Vec3f uv_left_f =                  Vec3f(uv0);
    Vec3f uv_right_f = (v1.y == v0.y) ? Vec3f(uv1) : Vec3f(uv0);

    for (int i=0; i<total_height; i++) {
        if(i==segment1_height){right_f = Vec3f(v1); uv_right_f = Vec3f(uv1);}
        bool second_half = i>=v1.y-v0.y || v1.y==v0.y;
        int segment_height = second_half ? v2.y-v1.y : v1.y-v0.y;
        Vec3f left   = Vec3f(left_f);
        Vec3f right   = Vec3f(right_f);
        Vec3f uv_left = Vec3f(uv_left_f);
        Vec3f uv_right = Vec3f(uv_right_f);
        if (left.x>right.x) { std::swap(left, right); std::swap(uv_left, uv_right); }
        Vec3f d = Vec3f(right.x==left.x ?(Vec3f(right - left)):(Vec3f(right - left))/(double)(right.x-left.x));
        Vec3f uv_d = Vec3f(right.x==left.x ?(Vec3f(uv_right - uv_left)):(Vec3f(uv_right - uv_left))/(double)(right.x-left.x));
        Vec3f point_f = Vec3f(left);
        Vec3f uv_point_f = Vec3f(uv_left);
        for (int j=left.x; j<=right.x; j++) {
            float phi = right.x==left.x ? 1. : (float)(j-left.x)/(float)(right.x-left.x);
            Vec3i point = Vec3i(point_f);
            Vec3i uv_point = Vec3i(uv_point_f);
            int idx = point.x+point.y*width;
            if (zbuffer[idx]<point.z) {
                zbuffer[idx] = point.z;
                //TGAColor color = texture.get(uv_point.x, uv_point.y);
                TGAColor color = TGAColor(255, 255, 255, 255);
                image.set(point.x, point.y, TGAColor(color.r*intensity, color.g*intensity, color.b*intensity, 255));
            }
            point_f = point_f + d;
            uv_point_f = uv_point_f + uv_d;
        }
        left_f = left_f + d20;
        right_f = right_f + (second_half?d21:d10);
        uv_left_f = uv_left_f + uv_d20;
        uv_right_f = uv_right_f + (second_half?uv_d21:uv_d10);
    }
}
