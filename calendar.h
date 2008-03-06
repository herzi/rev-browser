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

#ifndef CALENDAR_H
#define CALENDAR_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _Calendar        Calendar;
typedef struct _CalendarPrivate CalendarPrivate;
typedef struct _CalendarClass   CalendarClass;

#define TYPE_CALENDAR         (calendar_get_type ())
#define CALENDAR(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_CALENDAR, Calendar))
#define CALENDAR_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_CALENDAR, CalendarClass))
#define IS_CALENDAR(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), TYPE_CALENDAR))
#define IS_CALENDAR_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), TYPE_CALENDAR))
#define CALENDAR_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), TYPE_CALENDAR, CalendarClass))

GType     calendar_get_type (void);
Calendar* calendar_new      (void);

struct _Calendar {
	GObject          base_instance;
	CalendarPrivate* _private;
};

struct _CalendarClass {
	GObjectClass     base_class;
};

G_END_DECLS

#endif /* !CALENDAR_H */
