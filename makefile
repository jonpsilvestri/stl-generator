generator: generator.c 3d.o
	gcc -Wall -Werror -std=c11 -o generator generator.c 3d.o -lm
3d: 3d.c
	gcc -Wall -Werror -std=c11 -c 3d.c
clean:
	rm -f 3d.o
	rm -f generator
	rm -f fractals.stl
	rm -f sierpinski.stl
	rm -f spheres.stl
