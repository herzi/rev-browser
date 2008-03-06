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

#define DISPLAY(i) G_TYPE_CHECK_INSTANCE_CAST (widget, display_get_type (), Display);

struct _Display {
	GtkWidget       base_instance;
	DisplayPrivate* _private;
};

struct _DisplayClass {
	GtkWidgetClass  base_class;
};

struct _DisplayPrivate {
	gint element_size;
};

G_DEFINE_TYPE (Display, display, GTK_TYPE_WIDGET);

static void
display_init (Display* self)
{
	GTK_WIDGET_SET_FLAGS (self, GTK_NO_WINDOW);

	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self, display_get_type (), DisplayPrivate);

	self->_private->element_size = 33;
}

static gboolean
display_expose_event (GtkWidget     * widget,
		      GdkEventExpose* event)
{
	gchar const* years[] = {"2006", "2007", "2008"};
	Display* self = DISPLAY (widget);
	gsize i;

	/* FIXME: get color/gc from theme */
	gdk_draw_rectangle (widget->window,
			    widget->style->white_gc,
			    TRUE,
			    widget->allocation.x,
			    widget->allocation.y,
			    widget->allocation.width,
			    widget->allocation.height);

	PangoLayout* layout = pango_layout_new (gdk_pango_context_get_for_screen (gtk_widget_get_screen (widget)));
	pango_layout_set_font_description (layout,
					   widget->style->font_desc);
	pango_layout_set_ellipsize (layout, PANGO_ELLIPSIZE_END);
	{
		PangoAttrList* attributes = pango_attr_list_new ();
		PangoAttribute* attribute = pango_attr_size_new (0.8 *
								 pango_font_description_get_size (widget->style->font_desc));
		attribute->start_index = 0;
		attribute->end_index   = G_MAXUINT;
		pango_attr_list_insert (attributes, attribute);
		pango_layout_set_attributes (layout, attributes);
		pango_attr_list_unref (attributes);
	}
	pango_layout_set_width (layout, PANGO_SCALE * self->_private->element_size);

	for (i = 0; i < G_N_ELEMENTS (years); i++) {
		gdk_draw_line (widget->window,
			       widget->style->black_gc,
			       widget->allocation.x + i * self->_private->element_size,
			       widget->allocation.y + 1,
			       widget->allocation.x + i * self->_private->element_size,
			       widget->allocation.y + 5);

		pango_layout_set_text (layout, years[i], -1);
		gdk_draw_layout (widget->window,
				 widget->style->black_gc,
				 widget->allocation.x + i * self->_private->element_size + 5,
				 widget->allocation.y + 5,
				 layout);
	}
	g_object_unref (layout);

	gtk_paint_shadow (widget->style,
		       widget->window,
		       GTK_STATE_NORMAL,
		       GTK_SHADOW_IN,
		       &widget->allocation,
		       widget,
		       NULL,
		       widget->allocation.x,
		       widget->allocation.y,
		       widget->allocation.width,
		       widget->allocation.height);

	/* display selected item */

	return FALSE;
}

static void
display_size_request (GtkWidget     * widget,
		      GtkRequisition* req)
{
	Display* self = DISPLAY (widget);
	req->width = 3 * self->_private->element_size; /* FIXME: adjust to required size */
	req->height = 3;
}

static void
display_class_init (DisplayClass* self_class)
{
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (self_class);

	widget_class->expose_event  = display_expose_event;
	widget_class->size_request  = display_size_request;

	g_type_class_add_private (self_class, sizeof (DisplayPrivate));
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

