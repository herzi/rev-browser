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

void
add_button (GtkWidget   * hbox,
	    GtkArrowType  arrow,
	    gchar const * stock_id,
	    GtkWidget   * display)
{
	GtkWidget* button;

	button = gtk_button_new ();
	if (!stock_id) {
		gtk_container_add (GTK_CONTAINER (button),
				   gtk_arrow_new (arrow,
						  GTK_SHADOW_IN));

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
	} else {
		gtk_container_add (GTK_CONTAINER (button),
				   gtk_image_new_from_stock (stock_id,
							     GTK_ICON_SIZE_MENU));

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
	}
	gtk_button_set_relief (GTK_BUTTON (button),
			       GTK_RELIEF_NONE);
	gtk_widget_show (gtk_bin_get_child (GTK_BIN (button)));
	gtk_widget_show (button);
	gtk_box_pack_start (GTK_BOX (hbox),
			    button,
			    FALSE,
			    FALSE,
			    0);
}

static void
time_bar_init (TimeBar* self)
{
	GtkWidget* display; /* FIXME: drop after extracting time-bar from main() */
	GtkWidget* hbox = GTK_WIDGET (self); /* FIXME: drop */
	GtkWidget* vbox; /* FIXME: drop */

	PRIV(self) = G_TYPE_INSTANCE_GET_PRIVATE (self,
						  TYPE_TIME_BAR,
						  TimeBarPrivate);

	PRIV(self)->display = display_new ();

	display = PRIV(self)->display;
	gtk_widget_show (display);

	add_button (hbox,
		    GTK_ARROW_LEFT,
		    NULL,
		    display);
	gtk_box_pack_start_defaults (GTK_BOX (hbox),
				     display);
	add_button (hbox,
		    GTK_ARROW_RIGHT,
		    NULL,
		    display);

	PRIV(self)->vbox = gtk_vbox_new (TRUE, 0);
	vbox = PRIV(self)->vbox;
	gtk_widget_show (PRIV (self)->vbox);
	gtk_box_pack_start (GTK_BOX (hbox),
			    vbox,
			    FALSE,
			    FALSE,
			    0);
	add_button (vbox,
		    0,
		    GTK_STOCK_ZOOM_IN,
		    display);
	add_button (vbox,
		    0,
		    GTK_STOCK_ZOOM_OUT,
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

GtkWidget*
time_bar_get_vbox (TimeBar const* self)
{
	g_return_val_if_fail (IS_TIME_BAR (self), NULL);

	return PRIV (self)->vbox;
}

