/**
 * File: 3d.h
 * Description:
 * A C library for creating 3D shapes and scenes.
 * This header includes several structs needed for representing the 3D shapes.
 * It also contains the prototypes for several of the public-facing functions.
 */

#ifndef THREE_D_H
#define THREE_D_H

#define PI 3.1415926535897932384626433832795028841971

/**
 * Represents a Cartesian coordinate (x,y,z) in 3D space.
 */
typedef struct Coordinate3D {
  double x;
  double y;
  double z;
} Coordinate3D;

/**
 * Represents a triangle in 3D Space.
 * a, b, and c represent the locations of each corner of the triangle.
 */
typedef struct Triangle3D {
  Coordinate3D a;
  Coordinate3D b;
  Coordinate3D c;
} Triangle3D;

/**
 * A Triangle3DNode represends a triangle in 3D space.
 * It also contains a pointer to the next node.
 * This is so that we can maintain a linked-list of all of the triangles in this scene.
 */
typedef struct Triangle3DNode {
  Triangle3D triangle;
  struct Triangle3DNode * next;
} Triangle3DNode;

/**
 * This represents a scene in 3D space containing 0 or more objects within it.
 * The count field represents the number of Object3D's this scene contains.
 * The size field represents the size of the array to store the Object3Ds.
 * The objects field represents a pointer to an array of Object3D pointers.
 * This array should start off with a small value to save memory, and only grow
 * as needed as more Object3Ds are added.
 */
typedef struct Scene3D {
  long count;
  Triangle3DNode* root;
} Scene3D;

/**
 * This function allocate space for a new Scene3D object on the heap, 
 * initializes the values to defaults as necessary, and returns a pointer to
 * the object in memory.
 */
Scene3D* Scene3D_create();

/** 
 * Frees the memory on the heap for the Scene3D itself, as well as the Object3D
 * and Triangle3DNode's within it.
 *   Parameters:
 *     scene: The scene to destroy
 */
void Scene3D_destroy(Scene3D* scene);

/**
 * Write every shape from the Scene3D to the file with file_name using the STL
 * text format. The function is responsible for opening, writing to, and 
 * closing the file.
 *   Parameters:
 *     scene: The scene to write to the file
 *     file_name: The name of the file to write the STL data to
 */
void Scene3D_write_stl_text(Scene3D* scene, char* file_name);

/**
 * This function should create a new Object3D on the heap and populate it with
 * a bunch of triangles to represent a pyramid in 3D space.
 * The caller is responsible for freeing the memory, or, if this shape gets 
 * added to a Scene3D, that is can be freed when the scene is.
 *   Parameters:
 *     origin: The origin point for the pyramid (center of base)
 *     width: The length of the sides of the base of the pyramid
 *     height: The height of the pyramid
 *     orientation: Determines the direction the pyramid points.
 *                  Should be either: 
 *                    "forward"
 *                    "backward" 
 *                    "up"
 *                    "down"
 *                    "left"
 *                    "right"
 */
void Scene3D_add_pyramid(
    Scene3D* scene,
    Coordinate3D origin, 
    double width, double height, char* orientation);

/**
 * This function should create a new Object3D on the heap and populate it with
 * a bunch of triangles to represent a cuboid in 3D space.
 * The caller is responsible for freeing the memory, or, if this shape gets 
 * added to a Scene3D, that is can be freed when the scene is.
 *   Parameters:
 *     origin: The origin point for the cuboid (center)
 *     width: The width of the cuboid (x)
 *     height: The height of the cuboid (y)
 *     depth: The depth of the cuboid (z)
 */
void Scene3D_add_cuboid(
    Scene3D* scene,
    Coordinate3D origin, 
    double width, double height, double depth);

/**
 * Function for adding a quadrilateral to a scene.
 *   Parameters:
 *     scene: The scene to add to the shape to
 *     a, b, c, d: The coordinates of the corners of this quadrilateral in 3D space.
 */
void Scene3D_add_quadrilateral(
    Scene3D* scene,
    Coordinate3D a, Coordinate3D b, Coordinate3D c, Coordinate3D d);

/**
 * Write every shape from the Scene3D to the file with file_name using the STL
 * binary format. The function is responsible for opening, writing to, and 
 * closing the file.
 *   Parameters:
 *     scene: The scene to write to the file
 *     file_name: The name of the file to write the STL data to
 */
void Scene3D_write_stl_binary(Scene3D* scene, char* file_name);

/**
 * This function should create a new Object3D on the heap and populate it with
 * a bunch of triangles to represent a sphere in 3D space.
 * The caller is responsible for freeing the memory, or, if this shape gets 
 * added to a Scene3D, that is can be freed when the scene is.
 *   Parameters:
 *     origin: The origin point for the sphere (center)
 *     radius: The desired radius of the sphere
 *     increment: A value in the range (180, 0) that determines 
 *                the smoothness of the sphere.
 *                
 */
void Scene3D_add_sphere(
    Scene3D* scene,
    Coordinate3D origin,
    double radius, double increment);

void Scene3D_add_triangle(
	Scene3D* scene,
	Triangle3D triangle);

/**
 * This function should create a new Object3D on the heap and populate it with
 * a bunch of triangles to represent a cube-based fractal in 3D space.
 * The caller is responsible for freeing the memory, or, if this shape gets 
 * added to a Scene3D, that is can be freed when the scene is.
 *   Parameters:
 *     origin: The origin point for the fractal (center)
 *     size: Used for the width, height, and depth of the center cube
 *     levels: The number of levels to recurse to when building the fractal
 */
void Scene3D_add_fractal(
    Scene3D* scene,
    Coordinate3D origin, 
    double size, int levels);

#endif

