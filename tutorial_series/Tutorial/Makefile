CFLAGS = -std=c++17 -O3 -g -Wall
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

Tutorial: *.cpp *.hpp
	./compile.sh
	g++ $(CFLAGS) -o Tutorial.out *.cpp $(LDFLAGS)

.PHONY: test clean

test: Tutorial
	VK_INSTANCE_LAYERS=VK_LAYER_MESA_overlay VK_LAYER_MESA_OVERLAY_CONFIG=position=top-left ./Tutorial.out

clean:
	rm -r Tutorial.out