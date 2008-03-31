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
	GObject       base_instance;

	/*< priv >*/
	GtkWidget   * widget;
	GObject     * subject;
	gchar       * bindingname;
} Binding;

typedef struct {
	GObjectClass  base_class;
} BindingClass;

#define TYPE_BINDING         (binding_get_type ())
#define BINDING(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_BINDING, Binding))

/* GType Implementation */

G_DEFINE_TYPE (Binding, binding, G_TYPE_OBJECT);

static void
binding_init (Binding* self)
{}

static void
widget_destroy (GtkObject* object,
		Binding  * self)
{
	g_signal_handlers_disconnect_by_func (object, widget_destroy, self);

	g_object_unref (self->widget);
	self->widget = NULL;
}

static void
binding_finalize (GObject* object)
{
	Binding* binding = BINDING (object);

	if (binding->widget) {
		widget_destroy (GTK_OBJECT (binding->widget), binding);
	}

	g_free (binding->bindingname);

	G_OBJECT_CLASS (binding_parent_class)->finalize (object);
}

static void
binding_class_init (BindingClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = binding_finalize;
}

/* Public API Implementation */

static void
update_state (GObject   * subject,
	      GParamSpec* pspec,
	      Binding   * binding)
{
	gboolean sensitive = G_PARAM_SPEC_BOOLEAN (pspec)->default_value;

	g_object_get (subject,
		      pspec->name, &sensitive,
		      NULL);

	gtk_widget_set_sensitive (binding->widget, sensitive);
}

void
bind_sensitive (GtkWidget  * widget,
		gchar const* trigger_signal,
		GCallback    callback,
		GObject    * subject,
		gchar const* property_sensitivity)
{
	gchar      * dataname;
	gchar      * signal;
	GParamSpec* property;
	Binding* binding;

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

	dataname = g_strdup_printf ("BindingConnection::%s",
				    property_sensitivity);
	if (g_object_get_data (subject, dataname)) {
		// FIXME: resolve this with a dynamic name
		g_warning ("you can only bind one property each time, right now");
		g_free (dataname);
		return;
	}

	binding = g_object_new (TYPE_BINDING, NULL);

	/* connect to the lifetime of the subject */
	binding->subject = subject; /* FIXME: turn into real weak reference */
	binding->bindingname = dataname;
	g_object_set_data_full (subject,
				dataname,
				binding,
				g_object_unref);

	/* connect to the widget */
	binding->widget = g_object_ref_sink (widget);
	g_signal_connect (binding->widget, "destroy",
			  G_CALLBACK (widget_destroy), binding);

	signal = g_strdup_printf ("notify::%s", property_sensitivity);
	g_signal_connect (subject, signal,
			  G_CALLBACK (update_state), binding);
	g_free (signal);
	// connect the signal
	// store the handler to be cleanly disposed
}

