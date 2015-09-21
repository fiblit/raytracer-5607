raytrace: main.o vector3.o point.o ray.o sphere.o rgb.o
    g++ -o bin/raytrace obj/main.o obj/vector3.o obj/point.o obj/ray.o obj/sphere.o obj/rgb.o

main.o: main.cpp 
	g++ -Wall -fexceptions -g -std=c++11 -c main.cpp -o obj/main.o
vector3.o: vector3.cpp vector3.h
	g++ -Wall -fexceptions -g -std=c++11 -c vector3.cpp -o obj/vector3.o
point.o: point.cpp point.h
	g++ -Wall -fexceptions -g -std=c++11 -c point.cpp -o obj/point.o
ray.o: ray.cpp ray.h
	g++ -Wall -fexceptions -g -std=c++11 -c ray.cpp -o obj/ray.o
sphere.o: sphere.cpp sphere.h
	g++ -Wall -fexceptions -g -std=c++11 -c sphere.cpp -o obj/sphere.o
rgb.o: rgb.cpp rgb.h
	g++ -Wall -fexceptions -g -std=c++11 -c rgb.cpp -o obj/rgb.o

clean:
	rm bin/* obj/*