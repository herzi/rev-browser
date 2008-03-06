all: rev-lister rev-browser

rev-lister: rev-lister.c Makefile
	gcc -o $@ $< $(shell pkg-config --cflags --libs glib-2.0)

rev_browser_sources=\
	browser.c \
	calendar.c \
	calendar.h \
	display.c \
	display.h
rev-browser: $(rev_browser_sources) Makefile
	gcc -o $@ $(filter %.c,$(rev_browser_sources)) $(shell pkg-config --cflags --libs glib-2.0 gtk+-2.0)

