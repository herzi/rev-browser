/* This file is part of ref-browser
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

#include <gtk/gtk.h>

#include <sys/types.h>
#include <unistd.h>

#include "gdk-cairo.h"
#include "testcase.h"

static void
test_gdk_cb (Testcase   * testcase,
	     GdkDrawable* drawable,
	     GdkGC      * gc)
{
	gdk_draw_rectangle (drawable,
			    gc,
			    TRUE,
			    0, 0,
			    100, 80);
}

static void
test_cairo_cb (Testcase* testcase,
	       cairo_t * cr,
	       GdkGC   * gc)
{
	gdk_cairo_draw_rectangle (cr,
				  gc,
				  TRUE,
				  0, 0,
				  100, 80);
}

int
main (int   argc,
      char**argv)
{
	gboolean passed = TRUE;
	Testcase * testcase;
	GdkPixmap* gdk;
	GdkPixmap* cairo;
	GdkGC    * gdkgc;
	GdkGC    * cairogc;
	GdkPixbuf* gdkpix;
	GdkPixbuf* cairopix;
	gchar    * gdkdata;
	gchar    * cairodata;
	gsize      index;
	cairo_t  * cr;

	/* prepare */
	gtk_init (&argc, &argv);

	testcase = testcase_new ();

	gdk   = testcase_get_pixmap_gdk   (testcase);
	cairo = testcase_get_pixmap_cairo (testcase);
	gdkgc   = testcase_get_gc_gdk   (testcase);
	cairogc = testcase_get_gc_cairo (testcase);

	/* excercise */
	g_signal_connect (testcase, "exercise-gdk",
			  G_CALLBACK (test_gdk_cb), NULL);
	g_signal_connect (testcase, "exercise-cairo",
			  G_CALLBACK (test_cairo_cb), NULL);
	testcase_exercise (testcase);

	/* verify */
	gdkpix   = testcase_get_pixbuf_gdk (testcase);
	cairopix = testcase_get_pixbuf_cairo (testcase);

	g_return_val_if_fail (gdk_pixbuf_get_rowstride (gdkpix) == gdk_pixbuf_get_rowstride (cairopix), 1);
	g_return_val_if_fail (gdk_pixbuf_get_n_channels (gdkpix) == gdk_pixbuf_get_n_channels (cairopix), 1);
	g_return_val_if_fail (gdk_pixbuf_get_height (gdkpix) == gdk_pixbuf_get_height (cairopix), 1);
	g_return_val_if_fail (gdk_pixbuf_get_width (gdkpix) == gdk_pixbuf_get_width (cairopix), 1);

	gdkdata   = gdk_pixbuf_get_pixels (gdkpix);
	cairodata = gdk_pixbuf_get_pixels (cairopix);

	for (index = 0; index < gdk_pixbuf_get_height (gdkpix) * gdk_pixbuf_get_rowstride (gdkpix); index++) {
		if (gdkdata[index] != cairodata[index]) {
			gchar* filepath;
			g_warning ("Eeek! Differences at byte %d",
				   index);
			passed = FALSE;

			filepath = g_strdup_printf ("%d-%s-gdk.png",
						    getpid (),
						    g_get_prgname ());
			gdk_pixbuf_save (gdkpix,
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
			gdk_pixbuf_save (cairopix,
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

	/* cleanup */
	g_object_unref (testcase);

	return passed ? 0 : 1;
}

