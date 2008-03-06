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

static void
add_button (GtkWidget   * hbox,
	    GtkArrowType  arrow,
	    gchar const * stock_id)
{
	GtkWidget* button;

	if (!stock_id) {
		button = gtk_button_new ();
		gtk_container_add (GTK_CONTAINER (button),
				   gtk_arrow_new (arrow,
						  GTK_SHADOW_IN));
	} else {
		button = gtk_button_new_from_stock (stock_id);
	}
	gtk_button_set_relief (GTK_BUTTON (button),
			       GTK_RELIEF_NONE);
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
	GtkWidget* hbox;
	GtkWidget* vbox;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window),
			   hbox);

	add_button (hbox,
		    GTK_ARROW_LEFT,
		    NULL);
	gtk_box_pack_start_defaults (GTK_BOX (hbox),
				     g_object_new (display_get_type (), NULL));
	add_button (hbox,
		    GTK_ARROW_RIGHT,
		    NULL);

	vbox = gtk_vbox_new (TRUE, 0);
	gtk_container_add (GTK_CONTAINER (hbox),
			   vbox);
	add_button (vbox,
		    0,
		    GTK_STOCK_ZOOM_IN);
	add_button (vbox,
		    0,
		    GTK_STOCK_ZOOM_OUT);

	gtk_widget_show_all (window);

	gtk_main ();

	return 0;
}
