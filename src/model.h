//
// Created by Milena on 22.10.2016.
//

#ifndef RENDERER_PROJECT_MODEL_H
#define RENDERER_PROJECT_MODEL_H

#include <vector>
#include <map>
#include <string>

#include "geometry.h"
#include "material.h"

struct Boundaries{
    int start;
    int end;
    std::string material;
    Boundaries(int start, int end, const std::string &material): material(material){
        this->start = start;
        this->end = end;
    }
};

class Model {
private:
    Vec3f max_, min_;
    std::vector<Vec3f> vertices_;
    std::vector<Vec3f> texture_vertices_;
    std::vector<Vec3f> normals_;
    std::vector<std::string> mtl_files;
    std::vector<std::vector<Vec3i>> faces_;
    std::vector<Boundaries> material_boundaries;
    std::map<std::string, Material> materials;
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int ntexture_verts();
    int n_normals();
    int nfaces();
    Vec3f v(int i);
    Vec3f vt(int i);
    Vec3f vn(int i);
    std::vector<Vec3i> face(int idx);
    TGAColor diffuse(int iface, Vec3i point);
	Vec3i diffuse_point(int iface, Vec3f point);
    Vec3f min();
    Vec3f max();
	void load_mtl();
};

#endif //RENDERER_PROJECT_MODEL_H
