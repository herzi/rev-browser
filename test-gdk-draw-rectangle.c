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

	gdk   = gdk_pixmap_new (NULL,
				100, 80,
				24);
	cairo = gdk_pixmap_new (NULL,
				100, 80,
				24);
	gdkgc   = gdk_gc_new (gdk);
	cairogc = gdk_gc_new (cairo);

	/* excercise */
	gdk_draw_rectangle (gdk,
			    gdkgc,
			    TRUE,
			    0, 0,
			    100, 80);

	cr = gdk_cairo_create (cairo);
	gdk_cairo_draw_rectangle (cr,
				  cairogc,
				  TRUE,
				  0, 0,
				  100, 80);
	cairo_destroy (cr);

	/* verify */
	gdkpix   = gdk_pixbuf_get_from_drawable (NULL,
						 gdk,
						 gdk_rgb_get_colormap (),
						 0, 0,
						 0, 0,
						 100, 80);
	cairopix = gdk_pixbuf_get_from_drawable (NULL,
						 cairo,
						 gdk_rgb_get_colormap (),
						 0, 0,
						 0, 0,
						 100, 80);

	gdkdata   = gdk_pixbuf_get_pixels (gdkpix);
	cairodata = gdk_pixbuf_get_pixels (cairopix);

	g_return_val_if_fail (gdk_pixbuf_get_rowstride (gdkpix) == gdk_pixbuf_get_rowstride (cairopix), 1);
	g_return_val_if_fail (gdk_pixbuf_get_n_channels (gdkpix) == gdk_pixbuf_get_n_channels (cairopix), 1);
	g_return_val_if_fail (gdk_pixbuf_get_height (gdkpix) == gdk_pixbuf_get_height (cairopix), 1);
	g_return_val_if_fail (gdk_pixbuf_get_width (gdkpix) == gdk_pixbuf_get_width (cairopix), 1);

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

	g_object_unref (gdkpix);
	g_object_unref (cairopix);

	/* cleanup */
	g_object_unref (gdkgc);
	g_object_unref (cairogc);
	g_object_unref (gdk);
	g_object_unref (cairo);

	g_object_unref (testcase);

	return passed ? 0 : 1;
}

