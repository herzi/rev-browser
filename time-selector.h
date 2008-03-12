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

#ifndef TIME_SELECTOR_H
#define TIME_SELECTOR_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _TimeSelector        TimeSelector;
typedef struct _TimeSelectorPrivate TimeSelectorPrivate;
typedef struct _TimeSelectorClass   TimeSelectorClass;

#define TYPE_TIME_SELECTOR         (time_selector_get_type ())
#define TIME_SELECTOR(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_TIME_SELECTOR, TimeSelector))
#define TIME_SELECTOR_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_TIME_SELECTOR, TimeSelectorClass))
#define IS_TIME_SELECTOR(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), TYPE_TIME_SELECTOR))
#define IS_TIME_SELECTOR_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), TYPE_TIME_SELECTOR))
#define TIME_SELECTOR_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), TYPE_TIME_SELECTOR, TimeSelectorClass))

GType      time_selector_get_type (void);
GtkWidget* time_selector_new      (void);

struct _TimeSelector {
	GtkWidget            base_instance;
	TimeSelectorPrivate* _private;
};

struct _TimeSelectorClass {
	GtkWidgetClass       base_class;
};

G_END_DECLS

#endif /* !TIME_SELECTOR_H */
