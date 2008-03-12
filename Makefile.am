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
rev_lister_CPPFLAGS=$(GLIB_CFLAGS)
rev_lister_LDADD=$(GLIB_LIBS)

rev_browser_SOURCES=\
	browser.c \
	calendar.c \
	calendar.h \
	date.c \
	date.h \
	display.c \
	display.h \
	time-bar.h \
	time-bar.c \
	time-selector.h \
	time-selector.c \
	$(NULL)
rev_browser_CPPFLAGS=$(GTK_CFLAGS)
rev_browser_LDADD=$(GTK_LIBS)
