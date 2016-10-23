//
// Created by Milena on 02.10.2016.
//

#include "primitives.h"
#include "geometry.h"
#include <math.h>
#include <algorithm>


void brezenchem_line(int x0, int y0, int x1, int y1, TGAImage&image, TGAColor color){
    int dx = x1 - x0;
    int dy = y1 - y0;
    bool steep = false;
    if (abs(x0 - x1)<abs(y0 - y1)){
        steep = true;
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    dx = x1 - x0;
    dy = y1 - y0;
    int k = abs(dy);
    int error = 0;
    int pace = dy!=0?dy / abs(dy):0 ;
    for (int x = x0; x <= x1; x++){
        if (!steep)
            image.set(x, y0, color);
        else
            image.set(y0, x, color);
        error += k;
        if (error*2 > dx){
            y0 += pace;
            error -=abs(dx);
        }
    }
}




void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {

    swap_by_cond(t0, t1);
    swap_by_cond(t0, t2);
    swap_by_cond(t1, t2);

    brezenchem_line(t0.x, t0.y, t1.x, t1.y, image, color);
    brezenchem_line(t1.x, t1.y, t2.x, t2.y, image, color);
    brezenchem_line(t2.x, t2.y, t0.x, t0.y, image, color);


    double delta20 = ( t2.y== t0.y) ? 0 : (double)(t2.x-t0.x)/(double)(t2.y - t0.y);
    double delta10 = ( t1.y== t0.y) ? 0 : (double)(t1.x-t0.x)/(double)(t1.y - t0.y);
    double delta21 = ( t2.y== t1.y) ? 0 : (double)(t2.x-t1.x)/(double)(t2.y - t1.y);

    double x1 = t0.x;
    double x2 = x1;

    double delta = delta20;

    swap_by_cond(delta20, delta10);


    //first triangle drawing
    for (int i = t0.y; i < t1.y; i++){
        for (int j = x1; j <= x2; j++){
            image.set(j, i, color);
        }
        x1 += delta20;
        x2 += delta10;
    }

    if (t0.y == t1.y){
        x1 = t0.x;
        x2 = t1.x;
    }

        swap_by_cond(delta21, delta);


    for (int i = t1.y; i < t2.y; i++){
        for (int j = x1; j <= x2; j++){
            image.set(j, i, color);
        }
        x1 += delta;
        x2 += delta21;
    }
}
