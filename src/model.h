//
// Created by Milena on 22.10.2016.
//

#ifndef RENDERER_PROJECT_MODEL_H
#define RENDERER_PROJECT_MODEL_H


#include <vector>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<int> > faces_;
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    std::vector<int> face(int idx);
};

#endif //RENDERER_PROJECT_MODEL_H
