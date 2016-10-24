//
// Created by Milena on 02.10.2016.
//

#ifndef RENDERER_PROJECT_PRIMITIVES_H
#define RENDERER_PROJECT_PRIMITIVES_H
#include "tgaimage.h"
#include "geometry.h"

void brezenchem_line(int x0, int y0, int x1, int y1, TGAImage&image, TGAColor color);
void triangle(Vec3i t0, Vec3i t1, Vec3i t2, TGAImage &image, TGAColor color, int *zbuffer, int width);
template<typename T>
bool swap_by_cond(T& a, T& b)
{
    if (a > b)
    {
        T tmp(a);
        a = b;
        b = tmp;
        return true;
    }
    return false;
}




#endif //RENDERER_PROJECT_PRIMITIVES_H
