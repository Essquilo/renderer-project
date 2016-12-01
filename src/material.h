//
// Created by Ivan Prymak on 11/30/2016.
//

#ifndef RENDERER_PROJECT_MATERIAL_H
#define RENDERER_PROJECT_MATERIAL_H


#include "tgaimage.h"
#include "geometry.h"

class Material {
TGAImage diffusal;
public:
    Material(const char *filename);

    Material();

	TGAColor diffuse(Vec3i point);
	Vec3i diffuse_point(Vec3f point);

};


#endif //RENDERER_PROJECT_MATERIAL_H
