lib_LTLIBRARIES=libctk.la
bin_PROGRAMS=\
	rev-lister \
	rev-browser \
	$(NULL)
check_LTLIBRARIES=\
	libtest.la \
	$(NULL)
check_PROGRAMS=\
	test-gdk-draw-rectangle \
	$(NULL)
TESTS=$(check_PROGRAMS)

rev_lister_SOURCES=\
	rev-lister.c
	$(NULL)
rev_lister_CPPFLAGS=$(GLIB_CFLAGS)
rev_lister_LDADD=$(GLIB_LIBS)

libctk_la_SOURCES=\
	gdk-cairo.c \
	gdk-cairo.h \
	$(NULL)
libctk_la_CPPFLAGS=$(GTK_CFLAGS)
libctk_la_LIBADD=$(GTK_LIBS)

rev_browser_SOURCES=\
	browser.c \
	calendar.c \
	calendar.h \
	date.c \
	date.h \
	display.c \
	display.h \
	highlight-widget.c \
	highlight-widget.h \
	time-bar.h \
	time-bar.c \
	time-selector.h \
	time-selector.c \
	$(NULL)
rev_browser_CPPFLAGS=$(libctk_la_CPPFLAGS)
rev_browser_LDADD=$(libctk_la_LIBADD) libctk.la

libtest_la_BUILT_SOURCES=\
	test-marshallers.c \
	test-marshallers.h \
	$(NULL)
libtest_la_SOURCES=\
	$(libtest_la_BUILT_SOURCES) \
	testcase.c \
	testcase.h \
	$(NULL)
libtest_la_CPPFLAGS=$(libctk_la_CPPFLAGS)
libtest_la_LIBADD=$(libctk_la_LIBADD)

test_gdk_draw_rectangle_CPPFLAGS=$(libctk_la_CPPFLAGS)
test_gdk_draw_rectangle_LDADD=$(libctk_la_LIBADD) libtest.la libctk.la

dist_marshallers=\
	test-marshallers.list \
	$(NULL)

EXTRA_DIST=\
	$(dist_marshallers) \
	$(NULL)
CLEANFILES=\
	$(libtest_la_BUILT_SOURCES) \
	$(NULL)

test-marshallers.h: $(srcdir)/test-marshallers.list Makefile
	@( $(GLIB_GENMARSHAL) --header --prefix=test_cclosure_marshal $< ) > xgen-$(notdir $@) \
	&& (cmp -s xgen-$(notdir $@) $(notdir $@) || (echo "Creating $(notdir $@)..."; cp xgen-$(notdir $@) $(notdir $@)) ) \
	&& rm -f xgen-$(notdir $@)

test-marshallers.c: $(srcdir)/test-marshallers.list Makefile
	@( echo "#include \"test-marshallers.h\""; \
	$(GLIB_GENMARSHAL) --body --prefix=test_cclosure_marshal $< ) > xgen-$(notdir $@) \
	&& (cmp -s xgen-$(notdir $@) $(notdir $@) || (echo "Creating $(notdir $@)..."; cp xgen-$(notdir $@) $(notdir $@)) ) \
	&& rm -f xgen-$(notdir $@)

