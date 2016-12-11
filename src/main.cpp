#include "tgaimage.h"
#include "primitives.h"
#include <vector>
#include <limits>
#include <algorithm>
#include "model.h"

using namespace std;

const int width = 1200;
const int height = 1200;
const int depth = 2048;
Vec3f light_dir = Vec3f(1, -1, 1).normalize();
Vec3f eye;
Vec3f up(0, 1, 0);
Vec3f center;

Model *model = NULL;
int *zbuffer = NULL;
int main(int argc, char **argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("models/bat/bat.obj");
    }
	
    zbuffer = new int[width * height];
	center = model->min() + (model->max() - model->min()) / 2. ;
	eye = center;
	eye.z = 1;
	Matrix viewport = ::viewport(0, 0, width, height, depth);
	Matrix look = ::look_at(eye, center, up);
	Matrix projection = Matrix::identity();

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
				Vec4f screen4d = viewport * projection * look * Vec4f(v.x, v.y, v.z, 1.);
				screen_coords[j] = Vec3i(screen4d.x, screen4d.y, screen4d.z);
                auto test = Vec3i((v.x + 1.)* width/2., (v.y + 1)* height/2., v.z * depth);
                world_coords[j] = v;
                Vec3f vt = model->vt(face[j].y);
                texture_coords[j] = model->diffuse_point(i, vt);
            }
				triangle(i, screen_coords[0], screen_coords[1], screen_coords[2], texture_coords[0], texture_coords[1],
						 texture_coords[2], image, light_dir,*model, zbuffer);
        }

        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("output.tga");
    }

    { // dump z-buffer (debugging purposes only)
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                zbimage.set(i, j, TGAColor(double(255) * (zbuffer[i + j * width])/ double(depth), 1));
            }
        }
        zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        zbimage.write_tga_file("zbuffer.tga");
    }
    delete model;
    delete[] zbuffer;
	system("pause");
    return 0;
}
