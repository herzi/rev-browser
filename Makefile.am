bin_PROGRAMS=\
	rev-lister \
	rev-browser \
	$(NULL)

rev_lister_SOURCES=\
	rev-lister.c
	$(NULL)
rev_lister_CFLAGS=$(GLIB_CFLAGS) $(GLIB_LIBS)

rev_browser_SOURCES=\
	browser.c \
	calendar.c \
	calendar.h \
	date.c \
	date.h \
	display.c \
	display.h
rev_browser_CFLAGS=$(shell pkg-config --cflags --libs glib-2.0 gtk+-2.0)
