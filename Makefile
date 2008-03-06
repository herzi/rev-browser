COMPILE_FLAGS=$(shell pkg-config --cflags --libs glib-2.0)

all: rev-lister rev-browser

rev-lister: rev-lister.c Makefile
	gcc -o $@ $< $(COMPILE_FLAGS)

rev-browser: rev-browser.c Makefile
	gcc -o $@ $< $(COMPILE_FLAGS)

