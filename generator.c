/**
Author: Jonathan Silvestri
Description:	generates different stuff using the 3d library to make .stl files

*/

#include "3d.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void spheres();
void fractals();
void sierpinski(int);
void sierpinski_r(Scene3D*, Coordinate3D, double, double, int);

int main(void){
	fractals();
	spheres();
	sierpinski(6);
	return 0;
}

void sierpinski(int depth){
	Scene3D* s = Scene3D_create();
	Coordinate3D origin = (Coordinate3D){100, 100, 100};
	sierpinski_r(s, origin, 100.0, 100.0, depth);
	Scene3D_write_stl_text(s, "sierpinski.stl");
	Scene3D_destroy(s);
}

void sierpinski_r(Scene3D* scene, Coordinate3D origin, double width, double height, int depth) {
    if (depth == 0) {
        Scene3D_add_pyramid(scene, origin, width, height, "up");
    } else {
        double new_width = width / 2;
        double new_height = height / 2;
        Coordinate3D top_point = {origin.x, origin.y, origin.z + new_height};
        Coordinate3D left_point = {origin.x - new_width, origin.y + new_height, origin.z};
        Coordinate3D right_point = {origin.x + new_width, origin.y + new_height, origin.z};

        sierpinski_r(scene, origin, new_width, new_height, depth - 1);
        sierpinski_r(scene, top_point, new_width, new_height, depth - 1);
        sierpinski_r(scene, left_point, new_width, new_height, depth - 1);
        sierpinski_r(scene, right_point, new_width, new_height, depth - 1);
    }
}


void fractals(){
	Scene3D* s = Scene3D_create();
	int levels = 1;
	for (int i = 1; i >= 0; i--) {
		for (int j = 0; j < 3; j++) {
			Coordinate3D origin = (Coordinate3D){j*100, i*100, 0};
			Scene3D_add_fractal(s, origin, 50, levels);
			printf("count for level %d: %ld\n", levels, s->count);
			levels += 1;
		}
	}
	Scene3D_write_stl_binary(s, "fractals.stl");
	Scene3D_destroy(s);
}

void spheres(){
	Scene3D* s = Scene3D_create();
	double increments[] = {15, 10, 5, 36, 30, 20, 90, 60, 45};
	for (int i = 0; i < 3; i += 1) {
		for (int j = 0; j < 3; j += 1) {
			Coordinate3D origin = (Coordinate3D){j*100, i*100, 0};
			Scene3D_add_sphere(s, origin, 45, increments[(i*3) + j]);
		}
	}
	Scene3D_write_stl_binary(s, "spheres.stl");
	Scene3D_destroy(s);	
}

