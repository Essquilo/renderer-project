#include "tgaimage.h"
#include "primitives.h"
#include <vector>
#include <limits>
#include "model.h"

const int width = 800;
const int height = 800;
const int depth = 255;

Model *model = NULL;
int *zbuffer = NULL;
Vec3f light_dir(0, 0, -1);

int main(int argc, char **argv) {
    TGAImage texture;
    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("models/bat.obj");
        //model = new Model("models/african_head.obj");
        texture = TGAImage();
        //texture.read_tga_file("models/african_head_diffuse.tga");
        //texture.flip_vertically();
    }

    zbuffer = new int[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    { // draw the model
        TGAImage image(width, height, TGAImage::RGB);
        for (int i = 0; i < model->nfaces(); i++) {
            std::vector<Vec3i> face = model->face(i);
            Vec3i screen_coords[3];
            Vec3f world_coords[3];
            Vec3i texture_coords[3];
            for (int j = 0; j < 3; j++) {
                Vec3f v = model->v(face[j].x);
                //screen_coords[j] = Vec3i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2., (v.z + 1.) * depth / 2.);
                screen_coords[j] = Vec3i((v.x + 3.)/6. * width / 2., (v.y + 3.)/6. * height / 2., (v.z + 3.)/6. * depth / 2.);
                world_coords[j] = v;
                Vec3f vt = model->vt(face[j].y);
                //texture_coords[j] = Vec3i(vt.x*texture.get_width(), vt.y*texture.get_height(), 0);
                texture_coords[j] = Vec3i(0, 0, 0);
            }
            Vec3f n = (world_coords[2] - world_coords[0]) ^(world_coords[1] - world_coords[0]);
            n.normalize();
            float intensity = fabsf(n * light_dir);
                triangle(screen_coords[0], screen_coords[1], screen_coords[2], texture_coords[0], texture_coords[1],
                         texture_coords[2], image, intensity, texture, zbuffer, width);
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
