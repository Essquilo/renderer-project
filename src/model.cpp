#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include "model.h"
#include <filesystem>

using namespace std;

string get_dir (const string& str){
    size_t found;
    cout << "Splitting: " << str << endl;
    found=str.find_last_of("/");
    return str.substr(0, found==0?found:found+1);
}

Model::Model(const char *filename) : vertices_(), texture_vertices_(), normals_(), mtl_files(), faces_(), material_boundaries(), materials() {
    std::ifstream in;
    double min_d = numeric_limits<double >::min(), max_d = numeric_limits<double >::max();
    min_ = Vec3f(max_d, max_d, max_d);
    max_ = Vec3f(min_d, min_d, min_d);
    in.open (filename);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
			for (int i = 0; i < 3; i++)  {
                double coord;
                iss >> coord;
                v[i] = coord;
                min_[i] = std::min(min_[i], coord);
                max_[i] = std::max(max_[i], coord);
            }
            vertices_.push_back(v);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >>trash;
            Vec3f vt;
            for (int i=0;i<3;i++) iss >> vt[i];
            texture_vertices_.push_back(vt);
        }else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >>trash;
            Vec3f vn;
            for (int i=0;i<3;i++) iss >> vn[i];
            normals_.push_back(vn);
        } else if (!line.compare(0, 2, "f ")) {
            vector<Vec3i> f = vector<Vec3i>();
            int ivertex, iuv, inorm;
            iss >> trash;
            while (iss >> ivertex >> trash >> iuv >> trash >> inorm) {
                // in wavefront obj all indices start at 1, not zero
                ivertex--;
                iuv--;
                inorm--;
                Vec3i v = Vec3i(ivertex, iuv, inorm);
                f.push_back(v);
            }
			if(f.size() != 3) {
				cerr << "malformed face " << nfaces() << endl;
			}
            faces_.push_back(f);
        } else if (!line.compare(0, 7, "mtllib ")) {
            std::string libname;
            iss>>libname>>libname;
            string directory = get_dir(filename);
			mtl_files.push_back(directory + libname);
        }
        else if (!line.compare(0, 7, "usemtl ")) {
            std::string name;
            iss>>name>>name;
            if(material_boundaries.size())
                material_boundaries.back().end = nfaces();
            Boundaries boundaries(nfaces(), 0, name);
            material_boundaries.push_back(boundaries);
        }
    }
    // last vertex
    material_boundaries.back().end = nfaces();
    /*for(int i = 0; i<faces_.size();i++){
        cerr<<"#"<<i<<" "<<faces_[i][0]<<"/"<<faces_[i][1]<<"/"<<faces_[i][2]<<endl;
    }*/
	std::cout << "# v# " << vertices_.size() <<"# vt# "<< texture_vertices_.size()<< "# vn# "<< normals_.size()<<" f# "  << faces_.size() << std::endl;
    in.close();
	load_mtl();
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

Vec3f Model::vn(int i) {
	return normals_[i];
}

int Model::ntexture_verts() {
    return (int)texture_vertices_.size();
}

int Model::n_normals() {
	return normals_.size();
}

TGAColor Model::diffuse(int iface, Vec3i point) {
    for (auto boundary : material_boundaries){
           if(iface>=boundary.start&&iface<boundary.end){
               return materials[boundary.material].diffuse(point);
           }
    }
    cerr<<"cannot find color for vertex "<<iface;
    return TGAColor();
}

Vec3i Model::diffuse_point(int iface, Vec3f point) {
	for (auto boundary : material_boundaries) {
		if (iface >= boundary.start&&iface<boundary.end) {
			return materials[boundary.material].diffuse_point(point);
		}
	}
	cerr << "cannot find diffuse coordinates for vertex " << iface;
	return Vec3i(0, 0, 0);
}


Vec3f Model::min(){
    return min_;
}

Vec3f Model::max(){
    return max_;
}

void Model::load_mtl() {
	std::ifstream mtl_in;
	for (auto mtl_file : mtl_files) {
		vector<string> material_files;
		vector<string> material_names;
		mtl_in.open(mtl_file);
		if (mtl_in.fail()) { std::cerr << "Cannot read " << mtl_file << std::endl; return; }
		std::string mtl_line;
		while (!mtl_in.eof()) {
			std::getline(mtl_in, mtl_line);
			std::transform(mtl_line.begin(), mtl_line.end(), mtl_line.begin(), ::tolower);
			std::istringstream mtl_iss(mtl_line.c_str());
			string material_name;
			if (!mtl_line.compare(0, 7, "newmtl ")) {
				mtl_iss >> material_name >> material_name;
				material_names.push_back(material_name);
			}
			else if (!mtl_line.compare(0, 7, "map_kd ")) {
				string material_file;
				mtl_iss >> material_file >> material_file;
				material_files.push_back(get_dir(mtl_file) + material_file);
			}
			mtl_line.clear();
		}
		mtl_in.close();
		for (auto i = 0; i < material_files.size(); i++) {
			Material material = Material(material_files[i].c_str());
			materials[material_names[i]] = material;
		}
	}
}