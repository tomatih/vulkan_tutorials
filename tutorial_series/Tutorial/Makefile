CFLAGS = -std=c++17 -O3 -g -Wall
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

Tutorial: *.cpp *.hpp
	./compile.sh
	g++ $(CFLAGS) -o Tutorial.out *.cpp $(LDFLAGS)

.PHONY: test clean

test: Tutorial
	./Tutorial.out

clean:
	rm -r Tutorial.out