/* This file is part of rev-browser
 *
 * AUTHORS
 *     Sven Herzberg  <sven@imendio.com>
 *
 * Copyright (C) 2008  Sven Herzberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "gdk-cairo.h"

void
gdk_cairo_draw_rectangle (cairo_t * cr,
			  GdkGC   * gc,
			  gboolean  filled,
			  gint      x,
			  gint      y,
			  gint      width,
			  gint      height)
{
	GdkGCValues values;

	gdk_gc_get_values (gc, &values);
	gdk_colormap_query_color (gdk_gc_get_colormap (gc),
				  values.foreground.pixel,
				  &values.foreground);

	//g_print ("%s\n", gdk_color_to_string (&values.foreground));

	cairo_save (cr);
	gdk_cairo_set_source_color (cr, &values.foreground);
	if (filled) {
		/* FIXME: stroke, pattern, etc */
		cairo_rectangle (cr,
				 x, y,
				 width,
				 height);
		cairo_fill (cr);
	} else {
		g_warning ("stroked rectangles are not supported yet");
	}

	cairo_restore (cr);
}

