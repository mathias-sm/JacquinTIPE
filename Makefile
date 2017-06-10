all:
	gcc -O4 -I include code.c decode.c main.c outils.c `pkg-config --cflags --libs opencv` -lpthread -o jacquin
clean:
	rm -rf jacquin *.o
