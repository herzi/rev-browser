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

#include "sensitive-binding.h"

void
bind_sensitive (GtkWidget  * widget,
		gchar const* trigger_signal,
		GCallback    callback,
		GObject    * subject,
		gchar const* property_sensitivity)
{
	GParamSpec* property;

	g_return_if_fail (GTK_IS_WIDGET (widget));
	g_return_if_fail (G_IS_OBJECT (subject));

	property = g_object_class_find_property (G_OBJECT_GET_CLASS (subject), property_sensitivity);
	if (!property) {
		g_warning ("%s(%p) doesn't have a property called \"%s\"",
			   G_OBJECT_TYPE_NAME (subject),
			   subject,
			   property_sensitivity);
		return;
	}

	if (!G_IS_PARAM_SPEC_BOOLEAN (property)) {
		g_warning ("%s::%s doesn't contain a boolean value, but a value of %s",
			   G_OBJECT_TYPE_NAME (subject),
			   property_sensitivity,
			   g_type_name (property->value_type));
		return;
	}

	// create binding object
	// connect to the lifetime of both widget and subject
	// connect the signal
	// store the handler to be cleanly disposed
}

