//
// Created by Milena on 22.10.2016.
//

#ifndef RENDERER_PROJECT_MODEL_H
#define RENDERER_PROJECT_MODEL_H

#include <vector>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> vertices_;
    std::vector<Vec3f> texture_vertices_;
    std::vector<std::vector<Vec3i> > faces_;
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int ntexture_verts();
    int nfaces();
    Vec3f v(int i);
    Vec3f vt(int i);
    std::vector<Vec3i> face(int idx);
};

#endif //RENDERER_PROJECT_MODEL_H
