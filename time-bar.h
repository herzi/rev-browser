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

#ifndef TIME_BAR_H
#define TIME_BAR_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _TimeBar        TimeBar;
typedef struct _TimeBarPrivate TimeBarPrivate;
typedef struct _TimeBarClass   TimeBarClass;

#define TYPE_TIME_BAR         (time_bar_get_type ())
#define TIME_BAR(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_TIME_BAR, TimeBar))
#define TIME_BAR_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_TIME_BAR, TimeBarClass))
#define IS_TIME_BAR(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), TYPE_TIME_BAR))
#define IS_TIME_BAR_CLASS(c)  (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_TIME_BAR))
#define TIME_BAR_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), TYPE_TIME_BAR, TimeBarClass))

GType      time_bar_get_type         (void);
GtkWidget* time_bar_new              (void);
void       time_bar_set_label_column (TimeBar     * self,
				      gint          column);
void       time_bar_set_value_column (TimeBar     * self,
				      gint          column);
void       time_bar_set_model        (TimeBar     * self,
				      GtkTreeModel* model);

struct _TimeBar {
	GtkHBox         base_instance;
	TimeBarPrivate* _private;
};

struct _TimeBarClass {
	GtkHBoxClass    base_class;
};

G_END_DECLS

#endif /* !TIME_BAR_H */
