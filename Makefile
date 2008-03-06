all: rev-lister rev-browser

rev-lister: rev-lister.c Makefile
	gcc -o $@ $< $(shell pkg-config --cflags --libs glib-2.0)

rev-browser: rev-browser.c Makefile
	gcc -o $@ $< $(shell pkg-config --cflags --libs glib-2.0 gtk+-2.0)

