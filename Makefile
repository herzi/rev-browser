all: rev-lister

rev-lister: rev-lister.c Makefile
	gcc -o $@ $< $(shell pkg-config --cflags --libs glib-2.0)
