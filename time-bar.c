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

#include "time-bar.h"

#include "display.h"

struct _TimeBarPrivate {
	GtkWidget* display;
};

#define PRIV(i) (TIME_BAR (i)->_private)

/* GType Implementation */

G_DEFINE_TYPE (TimeBar, time_bar, GTK_TYPE_HBOX);

static void
time_bar_init (TimeBar* self)
{
	GtkWidget* display; /* FIXME: drop after extracting time-bar from main() */
	GtkWidget* hbox = GTK_WIDGET (self); /* FIXME: drop */

	PRIV(self) = G_TYPE_INSTANCE_GET_PRIVATE (self,
						  TYPE_TIME_BAR,
						  TimeBarPrivate);

	PRIV(self)->display = display_new ();

	display = PRIV(self)->display;

	add_button (hbox,
		    GTK_ARROW_LEFT,
		    NULL,
		    display);
	gtk_box_pack_start_defaults (GTK_BOX (hbox),
				     display);
}

static void
time_bar_class_init (TimeBarClass* self_class)
{
	g_type_class_add_private (self_class, sizeof (TimeBarPrivate));
}

/* Public API */

GtkWidget*
time_bar_new (void)
{
	return g_object_new (TYPE_TIME_BAR, NULL);
}

GtkWidget*
time_bar_get_display (TimeBar const* self)
{
	g_return_val_if_fail (IS_TIME_BAR (self), NULL);

	return PRIV (self)->display;
}

