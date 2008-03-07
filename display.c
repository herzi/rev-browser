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

#include "display.h"

#include <gdk/gdkkeysyms.h>

#include "calendar.h"

#define DEFAULT_SIZE 40 /* the default size and minimum of an element */

struct _DisplayPrivate {
	Calendar* calendar;
	guint     selected_start;
	guint     selected_end;
	guint     n_elements;
	gint      element_size;
};

/* GType definition */

G_DEFINE_TYPE (Display, display, GTK_TYPE_WIDGET);

static void
display_init (Display* self)
{
	GTK_WIDGET_SET_FLAGS (self, GTK_NO_WINDOW);
	GTK_WIDGET_SET_FLAGS (self, GTK_CAN_FOCUS);

	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self, display_get_type (), DisplayPrivate);

	self->_private->calendar = calendar_new ();
	self->_private->element_size = 33;
}

static void
display_finalize (GObject* object)
{
	g_object_unref (DISPLAY (object)->_private->calendar);

	G_OBJECT_CLASS (display_parent_class)->finalize (object);
}

static gboolean
display_expose_event (GtkWidget     * widget,
		      GdkEventExpose* event)
{
	Display* self = DISPLAY (widget);
	gsize i;

	/* FIXME: get color/gc from theme */
	gdk_draw_rectangle (widget->window,
			    widget->style->white_gc,
			    TRUE,
			    widget->allocation.x,
			    widget->allocation.y + 6,
			    widget->allocation.width,
			    widget->allocation.height - 12);

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

	for (i = 0; i < self->_private->n_elements; i++) {
		gchar* year = g_strdup_printf ("%d", 2006 + i);

		if (G_LIKELY (i)) {
			gdk_draw_line (widget->window,
				       widget->style->black_gc,
				       widget->allocation.x + i * (self->_private->element_size + 1),
				       widget->allocation.y + 6 + 1,
				       widget->allocation.x + i * (self->_private->element_size + 1),
				       widget->allocation.y + 6 + 5);
		}

		pango_layout_set_text (layout, year, -1);
		gdk_draw_layout (widget->window,
				 widget->style->black_gc,
				 widget->allocation.x + i * (self->_private->element_size + 1) + 5,
				 widget->allocation.y + 6 + 5,
				 layout);

		g_free (year);
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
		       widget->allocation.y + 6,
		       widget->allocation.width,
		       widget->allocation.height - 12);

	/* display selected item */
	gtk_paint_flat_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_IN,
			    &widget->allocation,
			    widget,
			    NULL,
			    widget->allocation.x + self->_private->selected_start * (self->_private->element_size + 1),
			    widget->allocation.y,
			    (self->_private->element_size + 1),
			    9);
	gtk_paint_flat_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_IN,
			    &widget->allocation,
			    widget,
			    NULL,
			    widget->allocation.x + self->_private->selected_start * (self->_private->element_size + 1),
			    widget->allocation.y + widget->allocation.height - 9,
			    self->_private->element_size + 1,
			    9);
	gtk_paint_flat_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_IN,
			    &widget->allocation,
			    widget,
			    NULL,
			    widget->allocation.x + self->_private->selected_start * (self->_private->element_size + 1),
			    widget->allocation.y + 9,
			    3,
			    widget->allocation.height - 18);
	gtk_paint_flat_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_IN,
			    &widget->allocation,
			    widget,
			    NULL,
			    widget->allocation.x + (self->_private->selected_end + 1) * (self->_private->element_size + 1) - 4,
			    widget->allocation.y + 9,
			    3,
			    widget->allocation.height - 18);
	gtk_paint_shadow (widget->style,
			  widget->window,
			  GTK_STATE_NORMAL,
			  GTK_SHADOW_IN,
			  &widget->allocation,
			  widget,
			  NULL,
			  widget->allocation.x + 3 + self->_private->selected_start * (self->_private->element_size + 1),
			  widget->allocation.y + 9,
			  self->_private->element_size + 1 - 5,
			  widget->allocation.height - 18);
	gtk_paint_shadow (widget->style,
			  widget->window,
			  GTK_STATE_NORMAL,
			  GTK_SHADOW_OUT,
			  &widget->allocation,
			  widget,
			  NULL,
			  widget->allocation.x + self->_private->selected_start * (self->_private->element_size + 1),
			  widget->allocation.y,
			  self->_private->element_size + 1 + 1,
			  widget->allocation.height);

	if (GTK_WIDGET_HAS_FOCUS (widget)) {
		gtk_paint_focus (widget->style,
				 widget->window,
				 GTK_STATE_NORMAL,
				 &widget->allocation,
				 widget,
				 NULL,
				 widget->allocation.x + self->_private->selected_start * (self->_private->element_size + 1) + 2,
				 widget->allocation.y + 2,
				 (self->_private->element_size + 1) - 3,
				 widget->allocation.height - 4);
	}

	return FALSE;
}

static gboolean
display_key_press_event (GtkWidget  * widget,
			 GdkEventKey* event)
{
	if ((event->state & GDK_CONTROL_MASK) == 0) {
		Display* self = DISPLAY (widget);

		switch (event->keyval) {
		case GDK_Left:
			if ((event->state & GDK_SHIFT_MASK) == 0) {
				if (self->_private->selected_start > 0) {
					self->_private->selected_start--;
					self->_private->selected_end = self->_private->selected_start;
					gtk_widget_queue_draw (widget);
					return TRUE;
				}
			} else {
				if (self->_private->selected_start > 0) {
					self->_private->selected_start--;
					gtk_widget_queue_draw (widget);
					return TRUE;
				}
			}
			break;
		case GDK_Right:
			if ((event->state & GDK_SHIFT_MASK) == 0) {
				if (self->_private->selected_end < (self->_private->n_elements - 1)) {
					self->_private->selected_end++;
					self->_private->selected_start = self->_private->selected_end;
					gtk_widget_queue_draw (widget);
					return TRUE;
				}
			} else {
				if (self->_private->selected_end < (self->_private->n_elements - 1)) {
					self->_private->selected_end++;
					gtk_widget_queue_draw (widget);
					return TRUE;
				}
			}
			break;
		}
	}

	return FALSE;
}

static void
display_size_allocate (GtkWidget    * widget,
		       GtkAllocation* allocation)
{
	Display* self = DISPLAY (widget);

	self->_private->n_elements = (allocation->width - 1) / (DEFAULT_SIZE + 1);
	self->_private->element_size = (allocation->width - 1) / self->_private->n_elements - 1;

	GTK_WIDGET_CLASS (display_parent_class)->size_allocate (widget, allocation);
}

static void
display_size_request (GtkWidget     * widget,
		      GtkRequisition* req)
{
	Display* self = DISPLAY (widget);
	req->width = 3 * (DEFAULT_SIZE + 1) + 1;
	req->height = 12 /* default white box size */
		      + 12 /* 6 pixels above and below the white box */
		      + 6  /* 3 pixels above and below the inner shadow */
		      + 4  /* ~2*shadow-width */;
}

static void
display_class_init (DisplayClass* self_class)
{
	GObjectClass  * object_class = G_OBJECT_CLASS (self_class);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (self_class);

	object_class->finalize = display_finalize;

	widget_class->expose_event    = display_expose_event;
	widget_class->key_press_event = display_key_press_event;
	widget_class->size_allocate   = display_size_allocate;
	widget_class->size_request    = display_size_request;

	g_type_class_add_private (self_class, sizeof (DisplayPrivate));
}

