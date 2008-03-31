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

typedef struct {
	GObject      base_instance;
} Binding;

typedef struct {
	GObjectClass base_class;
} BindingClass;

#define TYPE_BINDING         (binding_get_type ())

/* GType Implementation */

G_DEFINE_TYPE (Binding, binding, G_TYPE_OBJECT);

static void
binding_init (Binding* self)
{}

static void
binding_class_init (BindingClass* self_class)
{}

/* Public API Implementation */

void
bind_sensitive (GtkWidget  * widget,
		gchar const* trigger_signal,
		GCallback    callback,
		GObject    * subject,
		gchar const* property_sensitivity)
{
	gchar const* dataname = "BindingConnection";
	GParamSpec* property;
	Binding* binding;

	g_return_if_fail (GTK_IS_WIDGET (widget));
	g_return_if_fail (G_IS_OBJECT (subject));

	if (g_object_get_data (subject, dataname)) {
		// FIXME: resolve this with a dynamic name
		g_warning ("you can only bind one property right now");
		return;
	}

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

	binding = g_object_new (TYPE_BINDING, NULL);
	// connect to the lifetime of both widget and subject
	g_object_set_data_full (subject,
				dataname,
				binding,
				g_object_unref);
	// connect the signal
	// store the handler to be cleanly disposed
}

