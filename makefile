CC = gcc
OPT = -std=c++11 
#SRC = main vector3 point ray sphere rgb IO material light object cylinder
#CPP = (SRC:=.cpp)
#OBJ = (SRC:=.o)
#INC = (SRC:=.h)

raytrace: bin/raytrace

bin/raytrace: obj/main.o obj/vector3.o obj/point.o obj/ray.o obj/sphere.o obj/rgb.o obj/IO.o obj/material.o obj/light.o obj/object.o obj/cylinder.o bin/ obj/
	$(CC) -o bin/raytrace obj/*.o

obj/main.o: main.cpp | obj/
	$(CC) $(OPT) -c main.cpp -o obj/main.o
obj/vector3.o: vector3.cpp vector3.h | obj/
	$(CC) $(OPT) -c vector3.cpp -o obj/vector3.o
obj/point.o: point.cpp point.h | obj/
	$(CC) $(OPT) -c point.cpp -o obj/point.o
obj/ray.o: ray.cpp ray.h | obj/
	$(CC) $(OPT) -c ray.cpp -o obj/ray.o
obj/sphere.o: sphere.cpp sphere.h | obj/
	$(CC) $(OPT) -c sphere.cpp -o obj/sphere.o
obj/rgb.o: rgb.cpp rgb.h | obj/
	$(CC) $(OPT) -c rgb.cpp -o obj/rgb.o
obj/IO.o: IO.cpp IO.h | obj/
	$(CC) $(OPT) -c IO.cpp -o obj/IO.o
obj/material.o: material.cpp material.h | obj/
	$(CC) $(OPT) -c material.cpp -o obj/material.o
obj/light.o: light.cpp light.h | obj/
	$(CC) $(OPT) -c light.cpp -o obj/light.o
obj/object.o: object.cpp object.h | obj/
	$(CC) $(OPT) -c object.cpp -o obj/object.o
obj/cylinder.o: cylinder.cpp cylinder.h | obj/
	$(CC) $(OPT) -c cylinder.cpp -o obj/cylinder.o
obj/texture.o: texture.cpp texture.h | obj/
	$(CC) $(OPT) -c texture.cpp -o obj/texture.o

obj/:
	mkdir obj
bin/:
	mkdir bin

clean:
	rm -fr bin obj
