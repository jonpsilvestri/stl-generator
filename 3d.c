/****
	*Author: Jonathan Silvestri
	*Dscrptn: Implementation of 3d.h which will define functions used to create
	*		  3d objects in an .stl format
	*
*/

#include "3d.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void Scene3D_add_triangle(Scene3D*, Triangle3D);
float get_x(float, float, float);
float get_y(float, float, float);
float get_z(float, float);
float O(float);

Scene3D* Scene3D_create(){
	Scene3D* scene = calloc(sizeof(Scene3D), 1);
	scene->count = 0;
	scene->root = NULL;
	return scene;
}

void Scene3D_destroy(Scene3D* scene){
	Triangle3DNode* node = scene->root;
	Triangle3DNode* cur;
	while (node != NULL){
		cur = node;
		node = node->next;
		free(cur);
	}
	free(scene);
}

void Scene3D_write_stl_binary(Scene3D* scene, char* file_name){
	FILE* file = fopen(file_name, "wb");
	if (file == NULL){
	printf("Error: count not open file %s\n", file_name);
	fclose(file);
	exit(-1);
	}
	
	// Write Header 80bytes of 0
	char header[80] = {(char)0};
	fwrite(header, 1, sizeof(header), file);

	// Write facet count
	uint32_t facet_count = scene->count;
	fwrite(&facet_count, 4, 1, file);

	// normal vector info (always 0.0)
	float normal[3] = {(float)0, (float)0, (float)0};
	
	Triangle3DNode* cur = scene->root;
	// Write triangle info
	while (cur != NULL){
		// write normal
		fwrite(normal, 4, 3, file);
		
		float v1[3] = {cur->triangle.a.x, cur->triangle.a.y, cur->triangle.a.z};
		float v2[3] = {cur->triangle.b.x, cur->triangle.b.y, cur->triangle.b.z};
		float v3[3] = {cur->triangle.c.x, cur->triangle.c.y, cur->triangle.c.z};
	
		// write triangle verticies
		fwrite(v1, 4, 3, file);
		fwrite(v2, 4, 3, file);
		fwrite(v3, 4, 3, file);
		
		// Write the attribute count (set to 0)
		uint16_t attribute_count = 0;
        fwrite(&attribute_count, 2, 1, file);
		cur = cur->next;
	}
	fclose(file);
	
}

void Scene3D_write_stl_text(Scene3D* scene, char* file_name){
	FILE* file = fopen(file_name, "w");
	if (file == NULL){
	printf("Error: could not open file %s\n", file_name);
	exit(-1);
	}

	Triangle3DNode* cur = scene->root;
	fprintf(file, "solid scene\n");
	while (cur != NULL){
		fprintf(file, "  facet normal 0.0 0.0 0.0\n");
		fprintf(file, "    outer loop\n");
		fprintf(file, "      vertex %.5f %.5f %.5f\n", cur->triangle.a.x, cur->triangle.a.y, cur->triangle.a.z);
		fprintf(file, "      vertex %.5f %.5f %.5f\n", cur->triangle.b.x, cur->triangle.b.y, cur->triangle.b.z);
		fprintf(file, "      vertex %.5f %.5f %.5f\n", cur->triangle.c.x, cur->triangle.c.y, cur->triangle.c.z);
		fprintf(file, "    endloop\n");
		fprintf(file, "  endfacet\n");
		cur = cur->next;
	}
	fprintf(file, "endsolid scene\n");
	fclose(file);
}

void Scene3D_add_fractal(Scene3D* scene, Coordinate3D origin, double size, int depth){
	if (depth == 0) {return;}
	
	// add cube
	Scene3D_add_cuboid(scene, origin, size, size, size);
	// +x & -x changes
	origin.x += (size / 2);
	Scene3D_add_fractal(scene, origin, size/2, depth - 1);
	origin.x -= size;
	Scene3D_add_fractal(scene, origin, size/2, depth - 1);
	origin.x += (size / 2);
	// +y & -y changes
	origin.y += (size / 2);
	Scene3D_add_fractal(scene, origin, size/2, depth - 1);
	origin.y -= size;
	Scene3D_add_fractal(scene, origin, size/2, depth - 1);
	origin.y += (size / 2);
	// +z & -z changes
	origin.z += (size / 2);
	Scene3D_add_fractal(scene, origin, size/2, depth - 1);
	origin.z -= size;
	Scene3D_add_fractal(scene, origin, size/2, depth - 1);
	origin.z += (size / 2);
}

void Scene3D_add_sphere(Scene3D* scene, Coordinate3D origin, double radius, double increment){
	for (float phi = increment; phi <= 180.0; phi += increment){
		for (float theta = 0.0; theta < 360; theta += increment){
			Coordinate3D a = (Coordinate3D) {get_x(radius, theta, phi) + origin.x, get_y(radius, theta, phi) + origin.y, get_z(radius, phi)+ origin.z};
			Coordinate3D b = (Coordinate3D) {get_x(radius, theta, phi - increment)+origin.x, get_y(radius, theta, phi - increment)+origin.y, get_z(radius, phi - increment)+ origin.z};
			Coordinate3D c = (Coordinate3D) {get_x(radius, theta - increment, phi)+origin.x, get_y(radius, theta - increment, phi)+ origin.y, get_z(radius, phi)+ origin.z};
			Coordinate3D d = (Coordinate3D) {get_x(radius, theta-increment, phi-increment)+origin.x, get_y(radius, theta - increment, phi - increment)+origin.y, get_z(radius, phi - increment)+ origin.z};
			
			Scene3D_add_quadrilateral(scene, a, b, c, d);
		}
	} 	
}

void Scene3D_add_pyramid(Scene3D* scene, Coordinate3D origin, double width, double height, char* orientation){
    // Base Coordinates
	Coordinate3D a;
	Coordinate3D b;
	Coordinate3D c;
	Coordinate3D d;
	if ((strcmp(orientation, "up") == 0) || (strcmp(orientation, "down") == 0)){
    	a = (Coordinate3D) {origin.x - width/2, origin.y - width/2, origin.z};
   		b = (Coordinate3D) {origin.x - width/2, origin.y + width/2, origin.z};
   		c = (Coordinate3D) {origin.x + width/2, origin.y + width/2, origin.z};
  		d = (Coordinate3D) {origin.x + width/2, origin.y - width/2, origin.z};
	}
	else if ((strcmp(orientation, "left") == 0) || (strcmp(orientation, "right") == 0)){
		a = (Coordinate3D) {origin.x, origin.y - width/2, origin.z - width/2};
		b = (Coordinate3D) {origin.x, origin.y + width/2, origin.z - width/2};
		c = (Coordinate3D) {origin.x, origin.y + width/2, origin.z + width/2};
		d = (Coordinate3D) {origin.x, origin.y - width/2, origin.z + width/2};
	}
	else if ((strcmp(orientation, "forward") == 0) || (strcmp(orientation, "backward") == 0)){
		a = (Coordinate3D) {origin.x - width/2, origin.y, origin.z - width/2};
		b = (Coordinate3D) {origin.x - width/2, origin.y, origin.z + width/2};
		c = (Coordinate3D) {origin.x + width/2, origin.y, origin.z - width/2};
		d = (Coordinate3D) {origin.x + width/2, origin.y, origin.z + width/2};
	}


    // Top Coordinate based on orientation
    Coordinate3D top;
    if (strcmp(orientation, "up") == 0) {
        top = (Coordinate3D) {origin.x, origin.y, origin.z + height};
    } else if (strcmp(orientation, "down") == 0) {
        top = (Coordinate3D) {origin.x, origin.y, origin.z - height};
    } else if (strcmp(orientation, "forward") == 0) {
        top = (Coordinate3D) {origin.x, origin.y + height, origin.z};
    } else if (strcmp(orientation, "backward") == 0) {
        top = (Coordinate3D) {origin.x, origin.y - height, origin.z};
    } else if (strcmp(orientation, "right") == 0) {
        top = (Coordinate3D) {origin.x + height, origin.y, origin.z};
    } else if (strcmp(orientation, "left") == 0) {
        top = (Coordinate3D) {origin.x - height, origin.y, origin.z};
    } else {
        printf("Invalid orientation: %s\n", orientation);
        return;
    }
  

    // Add the base
    Scene3D_add_quadrilateral(scene, a, b, c, d);
  
    // Add the sides
    Scene3D_add_triangle(scene, (Triangle3D) {a, b, top});
    Scene3D_add_triangle(scene, (Triangle3D) {b, c, top});
    Scene3D_add_triangle(scene, (Triangle3D) {c, d, top});
    Scene3D_add_triangle(scene, (Triangle3D) {d, a, top});
}


void Scene3D_add_quadrilateral(Scene3D* scene, Coordinate3D a, Coordinate3D b,
	Coordinate3D c, Coordinate3D d){
	Triangle3D triangle_1 = (Triangle3D) {a, b, c};
	Triangle3D triangle_2 = (Triangle3D) {b, c, d};
	Triangle3D triangle_3 = (Triangle3D) {a, c, d};
	Triangle3D triangle_4 = (Triangle3D) {a, b, d};

	Scene3D_add_triangle(scene, triangle_1);
	Scene3D_add_triangle(scene, triangle_2);
	Scene3D_add_triangle(scene, triangle_3);
	Scene3D_add_triangle(scene, triangle_4);
}


void Scene3D_add_cuboid(Scene3D* scene, Coordinate3D origin, double width, double height, double depth){
    double half_width = width / 2.0;
    double half_height = height / 2.0;
    double half_depth = depth / 2.0;

    // Coordinates of the corners of the cuboid
    Coordinate3D front_top_left = (Coordinate3D) { origin.x - half_width, origin.y + half_height, origin.z - half_depth };
    Coordinate3D front_top_right = (Coordinate3D) { origin.x + half_width, origin.y + half_height, origin.z - half_depth };
    Coordinate3D front_bottom_right = (Coordinate3D) { origin.x + half_width, origin.y - half_height, origin.z - half_depth };
    Coordinate3D front_bottom_left = (Coordinate3D) { origin.x - half_width, origin.y - half_height, origin.z - half_depth };
    Coordinate3D back_top_left = (Coordinate3D) { origin.x - half_width, origin.y + half_height, origin.z + half_depth };
    Coordinate3D back_top_right = (Coordinate3D) { origin.x + half_width, origin.y + half_height, origin.z + half_depth };
    Coordinate3D back_bottom_right = (Coordinate3D) { origin.x + half_width, origin.y - half_height, origin.z + half_depth };
    Coordinate3D back_bottom_left = (Coordinate3D) { origin.x - half_width, origin.y - half_height, origin.z + half_depth };

    // add cuboid faces
    Scene3D_add_quadrilateral(scene, front_top_left, front_top_right, front_bottom_right, front_bottom_left);
    Scene3D_add_quadrilateral(scene, back_top_right, back_top_left, back_bottom_left, back_bottom_right);
    Scene3D_add_quadrilateral(scene, back_top_left, front_top_left, front_top_right, back_top_right);
    Scene3D_add_quadrilateral(scene, back_bottom_right, front_bottom_right, front_bottom_left, back_bottom_left);
    Scene3D_add_quadrilateral(scene, back_top_left, back_bottom_left, front_bottom_left, front_top_left);
    Scene3D_add_quadrilateral(scene, front_top_right, front_bottom_right, back_bottom_right, back_top_right);
}

// -------- private methods ------------

/**
returns the cartesian coordinate for x
*/
float get_x(float radius, float theta, float phi){
	return O(radius * sin(phi * PI / 180) * cos(theta * PI / 180));
}

/**
returns the cartesian coordinate for y
*/
float get_y(float radius, float theta, float phi){
	return O(radius * sin(phi * PI / 180) * sin(theta * PI / 180));
}

/**
returns the cartesian coordinate for z
*/
float get_z(float radius, float phi){
	return O(radius * cos(phi * PI / 180));
}


/**
Rounding method. Named 'O' because it is round. will round to 4th decimal place
*/
float O(float num){
	return floor(num * 10000 + 0.50000001) / 10000;
}

/**
Allocates space for a new triangle node, and adds given triangle to the node
*/
void Scene3D_add_triangle(Scene3D* scene, Triangle3D triangle){
	scene->count++;

	Triangle3DNode* new_node = calloc(sizeof(Triangle3DNode), 1);
	new_node->triangle = triangle;
	new_node->next = scene->root;
	scene->root = new_node;
}

