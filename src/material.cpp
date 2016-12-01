//
// Created by Ivan Prymak on 11/30/2016.
//

#include "material.h"

Material::Material(const char *filename) {
    diffusal = TGAImage();
    if(!diffusal.read_tga_file(filename)){
        std::cerr<<"error loading material "<<filename<<std::endl;
		return;
    }
	diffusal.flip_vertically();
}

Material::Material() : diffusal() {}

TGAColor Material::diffuse(Vec3i point) { return diffusal.get(point.x, point.y); }

Vec3i Material::diffuse_point(Vec3f point) {
	return Vec3i(point.x * diffusal.get_width(), point.y * diffusal.get_height(), 0);
}