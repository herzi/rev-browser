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

enum {
	COLUMN_LABEL,
	COLUMN_COUNT, /* FIXME: rename to COLUMN_VALUE */
	N_COLUMNS
};

int
main (int   argc,
      char**argv)
{
	GtkListStore* store;
	GtkTreeIter   iter;
	GtkWidget   * window;
	GtkWidget   * time_bar;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);

	time_bar = time_bar_new ();
	gtk_widget_show (time_bar);
	gtk_container_add (GTK_CONTAINER (window),
			   time_bar);

	store = gtk_list_store_new (N_COLUMNS,
				    G_TYPE_STRING,
				    G_TYPE_INT);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set    (store, &iter,
			       COLUMN_LABEL, "2007-07-23",
			       COLUMN_COUNT, 8,
			       -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set    (store, &iter,
			       COLUMN_LABEL, "2007-07-29",
			       COLUMN_COUNT, 1,
			       -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set    (store, &iter,
			       COLUMN_LABEL, "2007-08-19",
			       COLUMN_COUNT, 1,
			       -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set    (store, &iter,
			       COLUMN_LABEL, "2007-11-29",
			       COLUMN_COUNT, 4,
			       -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set    (store, &iter,
			       COLUMN_LABEL, "2008-03-06",
			       COLUMN_COUNT, 59,
			       -1);

	time_bar_set_model        (TIME_BAR (time_bar),
			           GTK_TREE_MODEL (store));
	time_bar_set_label_column (TIME_BAR (time_bar),
				   COLUMN_LABEL);
	time_bar_set_value_column (TIME_BAR (time_bar),
				   COLUMN_COUNT);
	g_object_unref (store);

	gtk_widget_show (window);

	gtk_main ();

	return 0;
}

