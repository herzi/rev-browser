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

#ifndef DATE_H
#define DATE_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _Date        Date;
typedef struct _DatePrivate DatePrivate;
typedef struct _DateClass   DateClass;

#define TYPE_DATE         (date_get_type ())
#define DATE(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_DATE, Date))
#define DATE_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_DATE, DateClass))
#define IS_DATE(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), TYPE_DATE))
#define IS_DATE_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), TYPE_DATE))
#define DATE_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), TYPE_DATE, DateClass))

GType date_get_type (void);

Date* date_new      (guint day,
		     guint month,
		     guint year);
guint date_get_year (Date const* self);

struct _Date {
	GObject       base_instance;
	DatePrivate*  _private;
};

struct _DateClass {
	GObjectClass  base_class;
};

G_END_DECLS

#endif /* !DATE_H */
