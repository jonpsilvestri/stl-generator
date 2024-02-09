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

int main(void){
	fractals();
	spheres();
	return 0;
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

