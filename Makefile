all: inc/*/*.c src/*.c
	gcc src/*.c inc/*/*.c -o main -std=c89 -lglfw -lGL

lib: inc/*/*.c src/opap.c
	gcc src/opap.c inc/*/*.c -c -std=c89 -fpic
	gcc -shared *.o -o libopap.so -lglfw -lGL

slib: inc/*/*.c src/opap.c
	gcc src/opap.c inc/*/*.c -c -std=c89 -static
	ar -rc libopap.a *.o

install: lib
	cp -r libopap.so /usr/local/lib
	cp -r src/opap.h /usr/local/include 

sinstall: slib
	cp -r src/opap.h /usr/local/include

clean:
	rm -rf ./*.o
