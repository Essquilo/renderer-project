//
// Created by Milena on 02.10.2016.
//

#include "primitives.h"
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
