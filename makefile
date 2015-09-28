raytrace: main.o vector3.o point.o ray.o sphere.o rgb.o bin/ obj/
	g++ -o bin/raytrace obj/*.o

main.o: main.cpp obj/
	g++ -std=c++11 -c main.cpp -o obj/main.o
vector3.o: vector3.cpp vector3.h obj/
	g++ -std=c++11 -c vector3.cpp -o obj/vector3.o
point.o: point.cpp point.h obj/
	g++ -std=c++11 -c point.cpp -o obj/point.o
ray.o: ray.cpp ray.h obj/
	g++ -std=c++11 -c ray.cpp -o obj/ray.o
sphere.o: sphere.cpp sphere.h obj/
	g++ -std=c++11 -c sphere.cpp -o obj/sphere.o
rgb.o: rgb.cpp rgb.h obj/
	g++ -std=c++11 -c rgb.cpp -o obj/rgb.o
IO.o: IO.cpp IO.h obj/
	g++ -std=c++11 -c IO.cpp -o obj/IO.o

obj/:
	mkdir obj
bin/:
	mkdir bin

clean:
	rm bin/* obj/*
