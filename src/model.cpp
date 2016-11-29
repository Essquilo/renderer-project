#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : vertices_(), texture_vertices_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            vertices_.push_back(v);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >>trash;
            Vec3f vt;
            for (int i=0;i<3;i++) iss >> vt[i];
            texture_vertices_.push_back(vt);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            int ivertice, itexture, inorm;
            iss >> trash;
            while (iss >> ivertice >> trash >> itexture >> trash >> inorm) {
                // in wavefront obj all indices start at 1, not zero
                ivertice--;
                itexture--;
                inorm--;
                f.push_back(Vec3i(ivertice, itexture, inorm));
            }
            faces_.push_back(f);
        }
    }
    std::cout << "# v# " << vertices_.size() <<"# vt# "<< texture_vertices_.size()<< " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)vertices_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<Vec3i> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::v(int i) {
    return vertices_[i];
}

Vec3f Model::vt(int i){
    return texture_vertices_[i];
}

int Model::ntexture_verts() {
    return (int)texture_vertices_.size();
}
