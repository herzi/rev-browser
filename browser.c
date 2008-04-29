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

#include "repository.h"
#include "repository-model.h"
#include "time-bar.h"

int
main (int   argc,
      char**argv)
{
	Repository  * repository;
	GtkTreeModel* model;
	GtkTreeIter   iter;
	GtkWidget   * window;
	GtkWidget   * box;
	GtkWidget   * button;
	GtkWidget   * time_bar;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);

	box = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (box);
	gtk_container_add (GTK_CONTAINER (window),
			   box);

	button = gtk_button_new_from_stock (GTK_STOCK_OPEN);
	gtk_widget_show (button);
	gtk_container_add (GTK_CONTAINER (box),
			   button);

	time_bar = time_bar_new ();
	gtk_widget_show (time_bar);
	gtk_container_add (GTK_CONTAINER (box),
			   time_bar);

	repository = repository_new ();
	model = repository_model_new (repository);
	g_object_unref (repository);

	time_bar_set_model        (TIME_BAR (time_bar),
				   model);
	time_bar_set_label_column (TIME_BAR (time_bar),
				   REPOSITORY_MODEL_COLUMN_DAY);
	time_bar_set_value_column (TIME_BAR (time_bar),
				   REPOSITORY_MODEL_COLUMN_COMMITS);

	gtk_widget_show (window);

	gtk_main ();

	g_object_unref (model);

	return 0;
}

