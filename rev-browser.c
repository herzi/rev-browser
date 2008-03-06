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

/* START: DISPLAY WIDGET IMPLEMENTATION */

/* GType definition */

typedef struct _Display        Display;
typedef struct _DisplayPrivate DisplayPrivate;
typedef struct _DisplayClass   DisplayClass;

struct _Display {
	GtkWidget       base_instance;
	DisplayPrivate* _private;
};

struct _DisplayClass {
	GtkWidgetClass  base_class;
};

G_DEFINE_TYPE (Display, display, GTK_TYPE_WIDGET);

static void
display_init (Display* self)
{
	GdkColor color = {0, 0xFFFF, 0xFFFF, 0xFFFF};
	GTK_WIDGET_SET_FLAGS (self, GTK_NO_WINDOW);

	gtk_widget_modify_bg (GTK_WIDGET (self),
			      GTK_STATE_NORMAL,
			      &color);
}

static gboolean
display_expose_event (GtkWidget     * widget,
		      GdkEventExpose* event)
{
	gtk_paint_box (widget->style,
		       widget->window,
		       GTK_STATE_NORMAL,
		       GTK_SHADOW_IN,
		       NULL,
		       widget,
		       NULL,
		       widget->allocation.x,
		       widget->allocation.y,
		       widget->allocation.width,
		       widget->allocation.height);
	return FALSE;
}

static void
display_size_request (GtkWidget     * widget,
		      GtkRequisition* req)
{
	req->width = 100; /* FIXME: adjust to required size */
	req->height = 3;
}

static void
display_class_init (DisplayClass* self_class)
{
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (self_class);

	widget_class->expose_event = display_expose_event;
	widget_class->size_request = display_size_request;
}

/* END: DISPLAY WIDGET IMPLEMENTATION */

static void
add_button (GtkWidget   * hbox,
	    GtkArrowType  arrow)
{
	GtkWidget* button;

	button = gtk_button_new ();
	gtk_button_set_relief (GTK_BUTTON (button),
			       GTK_RELIEF_NONE);
	gtk_container_add (GTK_CONTAINER (button),
			   gtk_arrow_new (arrow,
					  GTK_SHADOW_IN));
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

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window),
			   hbox);

	add_button (hbox,
		    GTK_ARROW_LEFT);
	gtk_box_pack_start_defaults (GTK_BOX (hbox),
				     g_object_new (display_get_type (), NULL));
	add_button (hbox,
		    GTK_ARROW_RIGHT);

	gtk_widget_show_all (window);

	gtk_main ();

	return 0;
}

