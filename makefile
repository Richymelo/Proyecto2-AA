all: proyecto_1

proyecto_1: proyecto_1.c
	gcc -o proyecto_1 proyecto_1.c `pkg-config --cflags --libs gtk+-3.0 cairo` -lm

clean:
	rm -f ./proyecto_1 || true

