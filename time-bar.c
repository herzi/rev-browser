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
	GtkWidget   * display;
	GtkWidget   * vbox;
	GtkTreeModel* model;
};

enum {
	PROP_0,
	PROP_MODEL
};

#define PRIV(i) (TIME_BAR (i)->_private)

/* GType Implementation */

G_DEFINE_TYPE (TimeBar, time_bar, GTK_TYPE_HBOX);

static GtkWidget*
add_button (GtkBox      * box,
	    GtkWidget   * child,
	    GtkWidget   * display)
{
	GtkWidget* button;

	button = gtk_button_new ();
	gtk_container_add (GTK_CONTAINER (button),
			   child);
	gtk_button_set_relief (GTK_BUTTON (button),
			       GTK_RELIEF_NONE);
	gtk_widget_show (gtk_bin_get_child (GTK_BIN (button)));
	gtk_widget_show (button);
	gtk_box_pack_start (box,
			    button,
			    FALSE,
			    FALSE,
			    0);

	return button;
}

static GtkWidget*
add_arrow_button (GtkBox      * box,
		  GtkArrowType  arrow,
		  GtkWidget   * display)
{
	GtkWidget* button = add_button (box,
					gtk_arrow_new (arrow,
						       GTK_SHADOW_IN),
					display);

	if (arrow == GTK_ARROW_RIGHT) {
		bind_sensitive (button,  "clicked", G_CALLBACK (display_scroll_right),
				display, "can-scroll-right");
	} else {
		bind_sensitive (button,  "clicked", G_CALLBACK (display_scroll_left),
				display, "can-scroll-left");
	}

	return button;
}

static inline GtkWidget*
add_icon_button (GtkBox     * box,
		 gchar const* stock_id,
		 GtkWidget  * display)
{
	return add_button (box,
			   gtk_image_new_from_stock (stock_id,
						     GTK_ICON_SIZE_MENU),
			   display);
}

static void
time_bar_init (TimeBar* self)
{
	GtkWidget* button;

	PRIV(self) = G_TYPE_INSTANCE_GET_PRIVATE (self,
						  TYPE_TIME_BAR,
						  TimeBarPrivate);

	PRIV(self)->display = display_new ();
	gtk_widget_show (PRIV(self)->display);

	button = add_arrow_button (GTK_BOX (self),
				   GTK_ARROW_LEFT,
				   PRIV(self)->display);
	gtk_box_pack_start_defaults (GTK_BOX (self),
				     PRIV(self)->display);
	button = add_arrow_button (GTK_BOX (self),
				   GTK_ARROW_RIGHT,
				   PRIV(self)->display);

	PRIV(self)->vbox = gtk_vbox_new (TRUE, 0);
	gtk_widget_show (PRIV (self)->vbox);
	gtk_box_pack_start (GTK_BOX (self),
			    PRIV(self)->vbox,
			    FALSE,
			    FALSE,
			    0);

	button = add_icon_button (GTK_BOX (PRIV(self)->vbox),
				  GTK_STOCK_ZOOM_IN,
				  PRIV(self)->display);
	bind_sensitive (button,  "clicked", G_CALLBACK (display_zoom_in),
			PRIV(self)->display, "can-zoom-in");

	button = add_icon_button (GTK_BOX (PRIV(self)->vbox),
				  GTK_STOCK_ZOOM_OUT,
				  PRIV(self)->display);
	bind_sensitive (button,  "clicked", G_CALLBACK (display_zoom_out),
			PRIV(self)->display, "can-zoom-out");
}

static void
time_bar_finalize (GObject* object)
{
	TimeBar* self = TIME_BAR (object);

	if (self->_private->model) {
		g_object_unref (self->_private->model);
	}

	G_OBJECT_CLASS (time_bar_parent_class)->finalize (object);
}

static void
time_bar_get_property (GObject   * object,
		       guint       prop_id,
		       GValue    * value,
		       GParamSpec* pspec)
{
	TimeBar* self = TIME_BAR (object);

	switch (prop_id) {
	case PROP_MODEL:
		g_value_set_object (value,
				    time_bar_get_model (self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
time_bar_set_property (GObject     * object,
		       guint         prop_id,
		       GValue const* value,
		       GParamSpec  * pspec)
{
	TimeBar* self = TIME_BAR (object);

	switch (prop_id) {
	case PROP_MODEL:
		time_bar_set_model (self, g_value_get_object (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
time_bar_class_init (TimeBarClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize     = time_bar_finalize;
	object_class->get_property = time_bar_get_property;
	object_class->set_property = time_bar_set_property;

	g_object_class_install_property (object_class, PROP_MODEL,
					 g_param_spec_object ("model", NULL, NULL,
							      GTK_TYPE_TREE_MODEL,
							      G_PARAM_READWRITE));

	g_type_class_add_private (self_class, sizeof (TimeBarPrivate));
}

/* Public API */

GtkTreeModel*
time_bar_get_model (TimeBar const* self)
{
	g_return_val_if_fail (IS_TIME_BAR (self), NULL);

	return PRIV(self)->model;
}

GtkWidget*
time_bar_new (void)
{
	return g_object_new (TYPE_TIME_BAR, NULL);
}

void
time_bar_set_label_column (TimeBar* self,
			   gint     column)
{
	g_return_if_fail (IS_TIME_BAR (self));
	g_return_if_fail (column >= -1);

	display_set_label_column (DISPLAY (self->_private->display),
				  column);

	// FIXME: g_object_notify (G_OBJECT (self), "column-label");
}

void
time_bar_set_value_column (TimeBar* self,
			   gint     column)
{
	g_return_if_fail (IS_TIME_BAR (self));
	g_return_if_fail (column >= -1);

	display_set_value_column (DISPLAY (self->_private->display),
				  column);

	// FIXME: g_object_notify (G_OBJECT (self), "column-value");
}

void
time_bar_set_model (TimeBar     * self,
		    GtkTreeModel* model)
{
	g_return_if_fail (IS_TIME_BAR (self));
	g_return_if_fail (!model || GTK_IS_TREE_MODEL (model));
	g_return_if_fail (!model || (gtk_tree_model_get_flags (model) & GTK_TREE_MODEL_LIST_ONLY) != 0);

	if (self->_private->model == model) {
		return;
	}

	if (self->_private->model) {
		g_object_unref (self->_private->model);
		self->_private->model = NULL;
	}

	if (model) {
		self->_private->model = g_object_ref (model);
	}

	display_set_model (DISPLAY (self->_private->display),
			   self->_private->model);

	g_object_notify (G_OBJECT (self), "model");
}

