//
// Created by Milena on 02.10.2016.
//

#ifndef RENDERER_PROJECT_PRIMITIVES_H
#define RENDERER_PROJECT_PRIMITIVES_H
#include "tgaimage.h"
#include "geometry.h"

void brezenchem_line(int x0, int y0, int x1, int y1, TGAImage&image, TGAColor color);
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);
template<typename T>
void swap_by_cond(T& a, T& b)
{
    if (a > b)
    {
        T tmp(a);
        a = b;
        b = tmp;
    }
}




#endif //RENDERER_PROJECT_PRIMITIVES_H
