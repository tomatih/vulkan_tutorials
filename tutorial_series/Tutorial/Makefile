CFLAGS = -std=c++17 -O3
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

Tutorial: main.cpp
	g++ $(CFLAGS) -o Tutorial.out main.cpp $(LDFLAGS)

.PHONY: test clean

test: Tutorial
	./Tutorial.out

clean:
	rm -r Tutorial.out