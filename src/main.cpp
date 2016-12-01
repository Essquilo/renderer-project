#include "tgaimage.h"
#include "primitives.h"
#include <vector>
#include <limits>
#include <algorithm>
#include "model.h"

using namespace std;

const int width = 1200;
const int height = 1200;
const int depth = 512;

Model *model = NULL;
int *zbuffer = NULL;
Vec3f light_dir(0, 0, -1);
int main(int argc, char **argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("models/bat/bat.obj");
    }
	Vec3f dif = model->max() - model->min();
	Vec3f start = Vec3f(model->min());

	double maxdif = max(dif[0], dif[1]), minstart = min(start[0], start[1]);
	maxdif = max(dif[1], dif[2]);
	minstart = min(start[1], start[2]);
    zbuffer = new int[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = numeric_limits<int>::min();
    }

    { // draw the model
        TGAImage image(width, height, TGAImage::RGB);
        for (int i = 0; i < model->nfaces(); i++) {
            vector<Vec3i> face = model->face(i);
            Vec3i screen_coords[3];
            Vec3f world_coords[3];
            Vec3i texture_coords[3];
            for (int j = 0; j < 3; j++) {
                Vec3f v = model->v(face[j].x);
                screen_coords[j] = Vec3i((v.x - start.x)/ ceil(maxdif) * width, (v.y  - start.y)/ ceil(maxdif) * height, (v.z - start.z)/ ceil(maxdif) * depth);
                world_coords[j] = v;
                Vec3f vt = model->vt(face[j].y);
                texture_coords[j] = model->diffuse_point(i, vt);
            }
            Vec3f n = (world_coords[2] - world_coords[0]) ^(world_coords[1] - world_coords[0]);
            n.normalize();
			float intensity = n * light_dir;
        	if(intensity>0){ 
				triangle(i, screen_coords[0], screen_coords[1], screen_coords[2], texture_coords[0], texture_coords[1],
						 texture_coords[2], image, intensity, *model, zbuffer, width);
			}
        }

        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("output.tga");
    }

    { // dump z-buffer (debugging purposes only)
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                zbimage.set(i, j, TGAColor(zbuffer[i + j * width], 1));
            }
        }
        zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        zbimage.write_tga_file("zbuffer.tga");
    }
    delete model;
    delete[] zbuffer;
    return 0;
}
