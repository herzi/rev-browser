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
	guint month;
	guint year;
};

enum {
	PROP_0,
	PROP_MONTH,
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
date_get_property (GObject   * object,
		   guint       prop_id,
		   GValue    * value,
		   GParamSpec* pspec)
{
	Date* self = DATE (object);

	switch (prop_id) {
	case PROP_MONTH:
		g_value_set_int (value, date_get_month (self));
		break;
	case PROP_YEAR:
		g_value_set_int (value, date_get_year (self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
date_set_property (GObject     * object,
		   guint         prop_id,
		   GValue const* value,
		   GParamSpec  * pspec)
{
	Date* self = DATE (object);

	switch (prop_id) {
	case PROP_MONTH:
		self->_private->month = g_value_get_int (value);
		break;
	case PROP_YEAR:
		self->_private->year = g_value_get_int (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
date_class_init (DateClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->get_property = date_get_property;
	object_class->set_property = date_set_property;

	g_object_class_install_property (object_class, PROP_MONTH,
					 g_param_spec_int ("month", NULL, NULL,
							   G_MININT, G_MAXINT, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (object_class, PROP_YEAR,
					 g_param_spec_int ("year", NULL, NULL,
							   G_MININT, G_MAXINT, 1900, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_type_class_add_private (self_class, sizeof (DatePrivate));
}

/* Public API Implementation */

Date*
date_new (guint day, guint month, guint year)
{
	g_warning ("%s(%s): finish implementation",
		   G_STRFUNC, G_STRLOC);
	return g_object_new (TYPE_DATE,
			     "year", year,
			     "month", month,
			     NULL);
}

guint
date_get_month (Date const* self)
{
	g_return_val_if_fail (IS_DATE (self), 0);

	return self->_private->month;
}

guint
date_get_year (Date const* self)
{
	g_return_val_if_fail (IS_DATE (self), 1900);

	return self->_private->year;
}

