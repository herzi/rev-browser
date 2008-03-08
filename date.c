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

#include "date.h"

struct _DatePrivate {
	guint year;
};

enum {
	PROP_0,
	PROP_YEAR
};

/* GType Implementation */

G_DEFINE_TYPE (Date, date, G_TYPE_OBJECT);

static void
date_init (Date* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self, TYPE_DATE, DatePrivate);
}

static void
date_class_init (DateClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	g_object_class_install_property (object_class, PROP_YEAR,
					 g_param_spec_int ("year", NULL, NULL,
							   G_MININT, G_MAXINT, 1900, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_type_class_add_private (self_class, sizeof (DatePrivate));
}

/* Public API Implementation */

Date*
date_new (guint day, guint month, guint year)
{
	g_warning ("finish implementation");
	return g_object_new (TYPE_DATE, NULL);
}

