default:
	gcc -o test math.c shader.c texture.c obj.c canvas.c main.c \
	-lm -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi \
	&& ./test
