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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _Display        Display;
typedef struct _DisplayPrivate DisplayPrivate;
typedef struct _DisplayClass   DisplayClass;

#define TYPE_DISPLAY         (display_get_type ())
#define DISPLAY(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_DISPLAY, Display))
#define DISPLAY_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_DISPLAY, DisplayClass))
#define IS_DISPLAY(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), TYPE_DISPLAY))
#define IS_DISPLAY_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), TYPE_DISPLAY))
#define DISPLAY_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), TYPE_DISPLAY, DisplayClass))

GType display_get_type (void);

GtkWidget* display_new              (void);
gboolean   display_can_scroll_left  (Display const* self);
gboolean   display_can_scroll_right (Display const* self);
gboolean   display_can_zoom_in      (Display const* self);
gboolean   display_can_zoom_out     (Display const* self);
void       display_set_label_column (Display      * self,
				     gint           column);
void       display_set_value_column (Display      * self,
				     gint           column);
void       display_set_model        (Display      * self,
				     GtkTreeModel * model);
void       display_scroll_left      (Display      * self);
void       display_scroll_right     (Display      * self);
void       display_zoom_in          (Display      * self);
void       display_zoom_out         (Display      * self);

struct _Display {
	GtkBin          base_instance;
	DisplayPrivate* _private;
};

struct _DisplayClass {
	GtkBinClass     base_class;
};

G_END_DECLS

#endif /* !DISPLAY_H */
