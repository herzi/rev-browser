COMPILE_FLAGS=$(shell pkg-config --cflags --libs glib-2.0)

all: rev-lister

rev-lister: rev-lister.c Makefile
	gcc -o $@ $< $(COMPILE_FLAGS)
