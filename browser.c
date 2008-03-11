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

#include <gtk/gtk.h>

#include "display.h"
#include "time-bar.h"

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

int
main (int   argc,
      char**argv)
{
	GtkWidget* window;
	GtkWidget* time_bar;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);

	time_bar = time_bar_new ();
	gtk_widget_show (time_bar);
	gtk_container_add (GTK_CONTAINER (window),
			   time_bar);

	gtk_widget_show (window);

	gtk_main ();

	return 0;
}

