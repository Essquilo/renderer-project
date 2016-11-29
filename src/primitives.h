//
// Created by Milena on 02.10.2016.
//

#ifndef RENDERER_PROJECT_PRIMITIVES_H
#define RENDERER_PROJECT_PRIMITIVES_H
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

void brezenchem_line(int x0, int y0, int x1, int y1, TGAImage&image, TGAColor color);
void triangle(Vec3i v0, Vec3i v1, Vec3i v2, Vec3i vt0, Vec3i vt1, Vec3i vt2, TGAImage &image, float intensity,  TGAImage &texture, int *zbuffer, int width);
template<typename T>
bool swap_by_cond(T& a, T& b)
{
    if (a > b){
        std::swap(a, b);
        return true;
    }
    return false;
}




#endif //RENDERER_PROJECT_PRIMITIVES_H
