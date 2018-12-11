default:
	gcc -o test math.c mesh.c shader.c texture.c obj.c gfx.c main.c \
	-g -lm -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi \
	&& ./test
