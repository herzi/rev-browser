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
	GtkWidget* vbox;
};

#define PRIV(i) (TIME_BAR (i)->_private)

/* GType Implementation */

G_DEFINE_TYPE (TimeBar, time_bar, GTK_TYPE_HBOX);

static void
display_notify_can_step_left (GObject   * object,
			      GParamSpec* pspec,
			      GtkWidget * button)
{
	gtk_widget_set_sensitive (button, display_can_step_left (DISPLAY (object)));
}

static void
display_notify_can_step_right (GObject   * object,
			       GParamSpec* pspec,
			       GtkWidget * button)
{
	gtk_widget_set_sensitive (button, display_can_step_right (DISPLAY (object)));
}

static void
display_notify_can_zoom_in (GObject   * object,
			    GParamSpec* pspec,
			    GtkWidget * button)
{
	gtk_widget_set_sensitive (button, display_can_zoom_in (DISPLAY (object)));
}

static void
display_notify_can_zoom_out (GObject   * object,
			     GParamSpec* pspec,
			     GtkWidget * button)
{
	gtk_widget_set_sensitive (button, display_can_zoom_out (DISPLAY (object)));
}

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
		g_signal_connect (display, "notify::can-step-right",
				  G_CALLBACK (display_notify_can_step_right), button);
		g_signal_connect_swapped (button, "clicked",
					  G_CALLBACK (display_step_right), display);
	} else {
		g_signal_connect (display, "notify::can-step-left",
				  G_CALLBACK (display_notify_can_step_left), button);
		g_signal_connect_swapped (button, "clicked",
					  G_CALLBACK (display_step_left), display);
	}

	return button;
}

static GtkWidget*
add_icon_button (GtkBox     * box,
		 gchar const* stock_id,
		 GtkWidget  * display)
{
	GtkWidget* button = add_button (box,
					gtk_image_new_from_stock (stock_id,
								  GTK_ICON_SIZE_MENU),
					display);

	if (!strcmp (stock_id, GTK_STOCK_ZOOM_IN)) {
		g_signal_connect (display, "notify::can-zoom-in",
				  G_CALLBACK (display_notify_can_zoom_in), button);
		g_signal_connect_swapped (button, "clicked",
					  G_CALLBACK (display_zoom_in), display);
	} else {
		g_signal_connect (display, "notify::can-zoom-out",
				  G_CALLBACK (display_notify_can_zoom_out), button);
		g_signal_connect_swapped (button, "clicked",
					  G_CALLBACK (display_zoom_out), display);
	}

	return button;
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
	button = add_icon_button (GTK_BOX (PRIV(self)->vbox),
				  GTK_STOCK_ZOOM_OUT,
				  PRIV(self)->display);
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

