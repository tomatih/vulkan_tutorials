EXTERNAL_LIBRARIES_PATH = libraries

CFLAGS = -std=c++2a -O3 -g -Wall -Wextra -I$(EXTERNAL_LIBRARIES_PATH)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi

DrawingTriangle: main.cpp
	glslc shaders/shader.vert -o vert.spv
	glslc shaders/shader.frag -o frag.spv
	g++ $(CFLAGS) -o DrawingTriangle.out main.cpp $(LDFLAGS)

.PHONY: test clean

test: DrawingTriangle
	VK_INSTANCE_LAYERS=VK_LAYER_MESA_overlay VK_LAYER_MESA_OVERLAY_CONFIG=position=top-left ./DrawingTriangle.out

clean:
	rm -r DrawingTriangle.out
	rm -r frag.spv
	rm -r vert.spv
