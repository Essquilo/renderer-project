#include "tgaimage.h"
#include "primitives.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);


    for (int i=0; i<1000000; i++) {
        brezenchem_line(13, 20, 80, 40, image, white);
        brezenchem_line(20, 13, 40, 80, image, red);
        brezenchem_line(80, 40, 13, 20, image, red);
    }



	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

