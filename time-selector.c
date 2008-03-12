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

#include "time-selector.h"

/* GType Implementation */

G_DEFINE_TYPE (TimeSelector, time_selector, GTK_TYPE_WIDGET);

static void
time_selector_init (TimeSelector* self)
{}

static void
time_selector_class_init (TimeSelectorClass* self_class)
{}

/* Public API Implementation */

GtkWidget*
time_selector_new (void)
{
	return g_object_new (TYPE_TIME_SELECTOR,
			     NULL);
}

