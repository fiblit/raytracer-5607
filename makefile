raytrace: bin/raytrace

bin/raytrace: obj/main.o obj/vector3.o obj/point.o obj/ray.o obj/sphere.o obj/rgb.o obj/IO.o obj/material.o obj/light.o obj/object.o obj/cylinder.o bin/ obj/
	g++ -o bin/raytrace obj/*.o

obj/main.o: main.cpp | obj/
	g++ -std=c++11 -c main.cpp -o obj/main.o
obj/vector3.o: vector3.cpp vector3.h | obj/
	g++ -std=c++11 -c vector3.cpp -o obj/vector3.o
obj/point.o: point.cpp point.h | obj/
	g++ -std=c++11 -c point.cpp -o obj/point.o
obj/ray.o: ray.cpp ray.h | obj/
	g++ -std=c++11 -c ray.cpp -o obj/ray.o
obj/sphere.o: sphere.cpp sphere.h | obj/
	g++ -std=c++11 -c sphere.cpp -o obj/sphere.o
obj/rgb.o: rgb.cpp rgb.h | obj/
	g++ -std=c++11 -c rgb.cpp -o obj/rgb.o
obj/IO.o: IO.cpp IO.h | obj/
	g++ -std=c++11 -c IO.cpp -o obj/IO.o
obj/material.o: material.cpp material.h | obj/
	g++ -std=c++11 -c material.cpp -o obj/material.o
obj/light.o: light.cpp light.h | obj/
	g++ -std=c++11 -c light.cpp -o obj/light.o
obj/object.o: object.cpp object.h | obj/
	g++ -std=c++11 -c object.cpp -o obj/object.o
obj/cylinder.o: cylinder.cpp cylinder.h | obj/
	g++ -std=c++11 -c cylinder.cpp -o obj/cylinder.o

obj/:
	mkdir obj
bin/:
	mkdir bin

clean:
	rm -fr bin obj
