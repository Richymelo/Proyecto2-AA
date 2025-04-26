all: proyecto_2

proyecto_2: proyecto_2.c
	gcc -o proyecto_2 proyecto_2.c `pkg-config --cflags --libs gtk+-3.0 cairo` -lm -pthread

clean:
	rm -f ./proyecto_2 || true

