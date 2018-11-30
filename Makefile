default:
	gcc -o test canvas.c main.c \
	-lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi \
	&& ./test
