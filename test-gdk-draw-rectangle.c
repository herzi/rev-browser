/* This file is part of herzi's playground
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

static void
gdk_cairo_draw_rectangle (GdkDrawable* drawable,
			  GdkGC      * gc,
			  gboolean     filled,
			  gint         x,
			  gint         y,
			  gint         width,
			  gint         height)
{
	/* FIXME: implement */
}

int
main (int   argc,
      char**argv)
{
	gboolean passed = TRUE;
	GdkPixmap* gdk;
	GdkPixmap* cairo;
	GdkGC    * gdkgc;
	GdkGC    * cairogc;

	/* prepare */
	gtk_init (&argc, &argv);

	gdk   = gdk_pixmap_new (NULL,
				100, 80,
				8);
	cairo = gdk_pixmap_new (NULL,
				100, 80,
				8);
	gdkgc   = gdk_gc_new (gdk);
	cairogc = gdk_gc_new (cairo);

	/* excercise */
	gdk_draw_rectangle (gdk,
			    gdkgc,
			    TRUE,
			    0, 0,
			    100, 80);
	gdk_cairo_draw_rectangle (cairo,
				  cairogc,
				  TRUE,
				  0, 0,
				  100, 80);

	/* verify */

	/* cleanup */
	g_object_unref (gdkgc);
	g_object_unref (cairogc);
	g_object_unref (gdk);
	g_object_unref (cairo);

	return passed ? 0 : 1;
}

