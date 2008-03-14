/* This file is part of rev-browser
 *
 * AUTHORS
 *     Sven Herzberg  <sven@imendio.com>
 *
 * Copyright (C) 2008  Sven Herzberg
 *
 * This work is provided "as is"; redistribution and modification
 * in whole or in part, in any medium, physical or electronic is
 * permitted without restriction.
 *
 * This work is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In no event shall the authors or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 */

#include "testcase.h"

#include "test-marshallers.h"

struct _TestcasePrivate {
	GdkPixmap* gdk_pixmap;
	GdkPixmap* cairo_pixmap;
	GdkGC    * gdk_gc;
	GdkGC    * cairo_gc;

	gboolean   passed;
};

enum {
	EXERCISE_CAIRO,
	EXERCISE_GDK,
	N_SIGNALS
};

static guint testcase_signals[N_SIGNALS] = {0};

#define PRIV(i) (TESTCASE(i)->_private)

/* GType Implementation */

G_DEFINE_TYPE (Testcase, testcase, G_TYPE_OBJECT);

static void
testcase_init (Testcase* self)
{
	cairo_t* cr;

	PRIV(self) = G_TYPE_INSTANCE_GET_PRIVATE (self,
						  TYPE_TESTCASE,
						  TestcasePrivate);

	PRIV(self)->gdk_pixmap   = gdk_pixmap_new (NULL,
						   100, 80,
						   24);
	PRIV(self)->cairo_pixmap = gdk_pixmap_new (NULL,
						   100, 80,
						   24);

	PRIV(self)->gdk_gc   = gdk_gc_new (PRIV(self)->gdk_pixmap);
	PRIV(self)->cairo_gc = gdk_gc_new (PRIV(self)->cairo_pixmap);

	cr = gdk_cairo_create (PRIV(self)->gdk_pixmap);
	cairo_paint (cr);
	cairo_destroy (cr);
	cr = gdk_cairo_create (PRIV(self)->cairo_pixmap);
	cairo_paint (cr);
	cairo_destroy (cr);
}

static void
testcase_finalize (GObject* object)
{
	g_object_unref (PRIV(object)->gdk_gc);
	g_object_unref (PRIV(object)->cairo_gc);

	g_object_unref (PRIV(object)->gdk_pixmap);
	g_object_unref (PRIV(object)->cairo_pixmap);

	G_OBJECT_CLASS (testcase_parent_class)->finalize (object);
}

static void
testcase_class_init (TestcaseClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = testcase_finalize;

	testcase_signals[EXERCISE_CAIRO] =
		g_signal_new ("exercise-cairo",
			      TYPE_TESTCASE,
			      G_SIGNAL_ACTION, 0,
			      NULL, NULL,
			      test_cclosure_marshal_NONE__POINTER_OBJECT, G_TYPE_NONE,
			      2,
			      G_TYPE_POINTER,
			      GDK_TYPE_GC);
	testcase_signals[EXERCISE_GDK] =
		g_signal_new ("exercise-gdk",
			      TYPE_TESTCASE,
			      G_SIGNAL_ACTION, 0,
			      NULL, NULL,
			      test_cclosure_marshal_NONE__OBJECT_OBJECT, G_TYPE_NONE,
			      2,
			      GDK_TYPE_DRAWABLE,
			      GDK_TYPE_GC);

	g_type_class_add_private (self_class, sizeof (TestcasePrivate));
}

/* Public API Implementation */

Testcase*
testcase_new (void)
{
	return g_object_new (TYPE_TESTCASE,
			     NULL);
}

void
testcase_exercise (Testcase* self)
{
	gchar    * gdkdata;
	gchar    * cairodata;
	gsize      index;
	GdkPixbuf* gdk_pixbuf;
	GdkPixbuf* cairo_pixbuf;
	cairo_t* cr;

	g_return_if_fail (IS_TESTCASE (self));

	g_signal_emit (self,
		       testcase_signals[EXERCISE_GDK],
		       0,
		       PRIV(self)->gdk_pixmap,
		       PRIV(self)->gdk_gc);

	cr = gdk_cairo_create (PRIV(self)->cairo_pixmap);
	g_signal_emit (self,
		       testcase_signals[EXERCISE_CAIRO],
		       0,
		       cr,
		       PRIV(self)->cairo_gc);
	cairo_destroy (cr);

	gdk_pixbuf   = gdk_pixbuf_get_from_drawable (NULL,
						     PRIV(self)->gdk_pixmap,
								 gdk_rgb_get_colormap (),
								 0, 0,
								 0, 0,
								 100, 80);
	cairo_pixbuf = gdk_pixbuf_get_from_drawable (NULL,
						     PRIV(self)->cairo_pixmap,
								 gdk_rgb_get_colormap (),
								 0, 0,
								 0, 0,
								 100, 80);

	gdkdata   = gdk_pixbuf_get_pixels (gdk_pixbuf);
	cairodata = gdk_pixbuf_get_pixels (cairo_pixbuf);

	g_return_if_fail (gdk_pixbuf_get_rowstride (gdk_pixbuf) == gdk_pixbuf_get_rowstride (cairo_pixbuf));
	g_return_if_fail (gdk_pixbuf_get_n_channels (gdk_pixbuf) == gdk_pixbuf_get_n_channels (cairo_pixbuf));
	g_return_if_fail (gdk_pixbuf_get_height (gdk_pixbuf) == gdk_pixbuf_get_height (cairo_pixbuf));
	g_return_if_fail (gdk_pixbuf_get_width (gdk_pixbuf) == gdk_pixbuf_get_width (cairo_pixbuf));

	PRIV(self)->passed = TRUE;

	for (index = 0; index < gdk_pixbuf_get_height (gdk_pixbuf) * gdk_pixbuf_get_rowstride (gdk_pixbuf); index++) {
		if (gdkdata[index] != cairodata[index]) {
			gchar* filepath;
			g_warning ("Eeek! Differences at byte %d",
				   index);
			PRIV(self)->passed = FALSE;

			filepath = g_strdup_printf ("%d-%s-gdk.png",
						    getpid (),
						    g_get_prgname ());
			gdk_pixbuf_save (gdk_pixbuf,
					 filepath,
					 "png",
					 NULL, /* FIXME: handle errors */
					 NULL);
			g_message ("=> wrote gdk image to \"%s\"",
				   filepath);
			g_free (filepath);

			filepath = g_strdup_printf ("%d-%s-cairo.png",
						    getpid (),
						    g_get_prgname ());
			gdk_pixbuf_save (cairo_pixbuf,
					 filepath,
					 "png",
					 NULL, /* FIXME: handle errors */
					 NULL);
			g_message ("=> wrote cairo image to \"%s\"",
				   filepath);
			g_free (filepath);
			break;
		}
	}

	g_object_unref (gdk_pixbuf);
	g_object_unref (cairo_pixbuf);
}

gboolean
testcase_get_passed (Testcase const* self)
{
	g_return_val_if_fail (IS_TESTCASE (self), FALSE);

	return PRIV(self)->passed;
}

