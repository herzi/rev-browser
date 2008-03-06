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

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _Display        Display;
typedef struct _DisplayPrivate DisplayPrivate;
typedef struct _DisplayClass   DisplayClass;

#define DISPLAY(i) G_TYPE_CHECK_INSTANCE_CAST ((i), display_get_type (), Display)

GType display_get_type (void);

struct _Display {
	GtkWidget       base_instance;
	DisplayPrivate* _private;
};

struct _DisplayClass {
	GtkWidgetClass  base_class;
};

G_END_DECLS

#endif /* !DISPLAY_H */
