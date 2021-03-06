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

#ifndef GDK_CAIRO_H
#define GDK_CAIRO_H

#include <gdk/gdk.h>

G_BEGIN_DECLS

void gdk_cairo_draw_layout       (cairo_t    * cr,
				  GdkGC      * gc,
				  gint         x,
				  gint         y,
				  PangoLayout* layout);
void gdk_cairo_draw_line         (cairo_t    * cr,
				  GdkGC      * gc,
				  gint         x1,
				  gint         y1,
				  gint         x2,
				  gint         y2);
void gdk_cairo_draw_rectangle    (cairo_t    * cr,
				  GdkGC      * gc,
				  gboolean     filled,
				  gint         x,
				  gint         y,
				  gint         width,
				  gint      height);

G_END_DECLS

#endif /* !GDK_CAIRO_H */
