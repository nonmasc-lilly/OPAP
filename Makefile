all: inc/*/*.c src/*.c
	gcc src/*.c inc/*/*.c -o main -std=c89 -lglfw -lGL
