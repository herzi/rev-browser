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

GtkWidget* display_new            (void);
gboolean   display_can_step_right (Display const* self);

struct _Display {
	GtkWidget       base_instance;
	DisplayPrivate* _private;
};

struct _DisplayClass {
	GtkWidgetClass  base_class;
};

G_END_DECLS

#endif /* !DISPLAY_H */
