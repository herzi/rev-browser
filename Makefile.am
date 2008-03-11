bin_PROGRAMS=\
	rev-lister \
	rev-browser \
	$(NULL)
check_PROGRAMS=\
	test-gdk-draw-rectangle \
	$(NULL)
TESTS=$(check_PROGRAMS)

test_gdk_draw_rectangle_CPPFLAGS=$(GTK_CFLAGS)
test_gdk_draw_rectangle_LDADD=$(GTK_LIBS)

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
rev_browser_CFLAGS=$(GTK_CFLAGS) $(GTK_LIBS)
