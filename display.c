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
#include "gdk-cairo.h"

#include "calendar.h"
#include "highlight-widget.h"
#include "time-period.h"
#include "time-selector.h"

#define DEFAULT_SIZE     40 /* the default size and minimum of an element */
#define VERTICAL_PADDING 6

struct _DisplayPrivate {
	GtkWidget   * selector;

	/* the data model and display settings */
	GtkTreeModel* model;
	gint          column_label;
	gint          column_value;

	/* range settings */
	Date       * date_start;
	Date       * date_end;
	guint        selected_start;
	guint        selected_end;

	/* size state */
	guint        elements_visible;
	gint         element_size;

	/* display state */
	gint         offset;
	TimePeriod   zoom;
};

enum {
	PROP_0,
	PROP_CAN_STEP_LEFT,
	PROP_CAN_STEP_RIGHT,
	PROP_CAN_ZOOM_IN,
	PROP_CAN_ZOOM_OUT
};

/* GType definition */

G_DEFINE_TYPE (Display, display, GTK_TYPE_BIN);

static void
display_init (Display* self)
{
	GTK_WIDGET_SET_FLAGS (self, GTK_NO_WINDOW);

	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self, display_get_type (), DisplayPrivate);

	self->_private->selector = time_selector_new ();
	gtk_widget_show (self->_private->selector);
	gtk_container_add (GTK_CONTAINER (self),
			   self->_private->selector);

	self->_private->column_label = -1;
	self->_private->column_value = -1;

	self->_private->element_size = 33;
	self->_private->date_start = date_new (1, 1, 1982);
	self->_private->date_end   = date_new (31, 12, 1988);
	self->_private->zoom       = TIME_PERIOD_YEAR;
}

static void
display_get_property (GObject   * object,
		      guint       prop_id,
		      GValue    * value,
		      GParamSpec* pspec)
{
	Display* self = DISPLAY (object);

	switch (prop_id) {
	case PROP_CAN_STEP_LEFT:
		g_value_set_boolean (value,
				     display_can_step_left (self));
		break;
	case PROP_CAN_STEP_RIGHT:
		g_value_set_boolean (value,
				     display_can_step_right (self));
		break;
	case PROP_CAN_ZOOM_IN:
		g_value_set_boolean (value,
				     display_can_zoom_in (self));
		break;
	case PROP_CAN_ZOOM_OUT:
		g_value_set_boolean (value,
				     display_can_zoom_out (self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
display_finalize (GObject* object)
{
	G_OBJECT_CLASS (display_parent_class)->finalize (object);
}

static guint
display_get_range_size (Display const* self)
{
	return gtk_tree_model_iter_n_children (self->_private->model,
					       NULL);
}

static gchar*
display_get_date_string (Display const* self,
			 gsize          i)
{
	switch (self->_private->zoom) {
		guint temp;
	case TIME_PERIOD_MONTH:
		temp = (date_get_month (self->_private->date_start) +
			self->_private->offset + i - 1) % 12 + 1; // month number (1-12)
		if (G_UNLIKELY (temp == 1)) {
			return g_strdup_printf ("%d\n%d",
						temp,
						date_get_year (self->_private->date_start) +
						(date_get_month (self->_private->date_start) +
						 self->_private->offset + i - 1) / 12);
		} else {
			return g_strdup_printf ("%d",
						temp);
		}
	case TIME_PERIOD_YEAR:
		return g_strdup_printf ("%d",
					date_get_year (self->_private->date_start) +
					self->_private->offset + i);
	default:
		g_warning ("%s(): unknown zoom value %d",
			   G_STRFUNC,
			   self->_private->zoom);
		return g_strdup ("---");
	}
}

static gint
get_selector_x (Display const* self)
{
	GtkWidget const* widget = GTK_WIDGET (self);
	return widget->allocation.x + (self->_private->selected_start - self->_private->offset) * (self->_private->element_size + 1);
}

static gint
get_selector_width (Display const* self)
{
	return (1 + self->_private->selected_end - self->_private->selected_start) * (self->_private->element_size + 1);
}

static gboolean
display_expose_event (GtkWidget     * widget,
		      GdkEventExpose* event)
{
	Display* self = DISPLAY (widget);
	cairo_t* cr;
	gsize i;

	/* FIXME: get color/gc from theme */
	cr = gdk_cairo_create (widget->window);
	gdk_cairo_draw_rectangle (cr,
				  widget->style->base_gc[GTK_STATE_NORMAL],
				  TRUE,
				  widget->allocation.x + 1,
				  widget->allocation.y + VERTICAL_PADDING + 1,
				  MIN (widget->allocation.width,
				       1 + display_get_range_size (self) *(self->_private->element_size + 1)) - 2,
				  widget->allocation.height - 2 * VERTICAL_PADDING - 2);

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

	for (i = 0; i < self->_private->elements_visible; i++) {
		GtkTreeIter  iter;

		if (!gtk_tree_model_iter_nth_child (self->_private->model,
						    &iter,
						    NULL,
						    i + self->_private->offset))
		{
			break;
		}

		if (G_LIKELY (i)) {
			gdk_cairo_draw_line (cr,
					     widget->style->black_gc,
					     widget->allocation.x + i * (self->_private->element_size + 1),
					     widget->allocation.y + VERTICAL_PADDING + 1,
					     widget->allocation.x + i * (self->_private->element_size + 1),
					     widget->allocation.y + VERTICAL_PADDING + 7);
		}

		if (self->_private->column_label >= 0) {
			gchar* year = NULL;
			gtk_tree_model_get (self->_private->model, &iter,
					    self->_private->column_label,
					    &year,
					    -1);

			pango_layout_set_text (layout, year, -1);
			gdk_cairo_draw_layout (cr,
					       widget->style->text_gc[GTK_STATE_NORMAL],
					       widget->allocation.x + i * (self->_private->element_size + 1) + 5,
					       widget->allocation.y + VERTICAL_PADDING + 5,
					       layout);

			g_free (year);
		}

		if (self->_private->column_value >= 0) {
			gint value = 0;
			gint height = 0;
			gtk_tree_model_get (self->_private->model, &iter,
					    self->_private->column_value,
					    &value,
					    -1);

			height = MIN (value, widget->allocation.height - 35);
			gdk_cairo_draw_rectangle (cr,
						  widget->style->base_gc[GTK_STATE_ACTIVE],
						  TRUE,
						  widget->allocation.x + i * (self->_private->element_size + 1) + 6,
						  widget->allocation.y + widget->allocation.height - 12 - height,
						  (self->_private->element_size + 1) - 12,
						  height);
		}
	}
	cairo_destroy (cr);
	g_object_unref (layout);

	gtk_paint_shadow (widget->style,
			  widget->window,
			  GTK_STATE_NORMAL,
			  GTK_SHADOW_IN,
			  &widget->allocation,
			  widget,
			  NULL,
			  widget->allocation.x,
			  widget->allocation.y + VERTICAL_PADDING,
			  widget->allocation.width,
			  widget->allocation.height - 2 * VERTICAL_PADDING);

	/* display selected item */
	gtk_container_propagate_expose (GTK_CONTAINER (self),
					self->_private->selector,
					event);

	return FALSE;
}

static void
notify_changes (Display* self)
{
	g_object_notify (G_OBJECT (self), "can-step-left");
	g_object_notify (G_OBJECT (self), "can-step-right");
	g_object_notify (G_OBJECT (self), "can-zoom-in");
	g_object_notify (G_OBJECT (self), "can-zoom-out");
}

static void
allocate_selector (Display* self)
{
	GtkAllocation allocation = GTK_WIDGET (self)->allocation;
	allocation.x = get_selector_x (self);
	allocation.width = get_selector_width (self);

	if (allocation.x < GTK_WIDGET (self)->allocation.x) {
		gtk_widget_hide (self->_private->selector);
	} else if (allocation.x >= GTK_WIDGET (self)->allocation.x + GTK_WIDGET (self)->allocation.width) {
		gtk_widget_hide (self->_private->selector);
	} else {
		gtk_widget_size_allocate (self->_private->selector,
					  &allocation);

		gtk_widget_show (self->_private->selector);
	}
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
					allocate_selector (self);
					gtk_widget_queue_draw (widget);
					notify_changes (self);
					return TRUE;
				} else if (self->_private->selected_end > 0) {
					self->_private->selected_end = self->_private->selected_start;
					allocate_selector (self);
					gtk_widget_queue_draw (widget);
					notify_changes (self);
					return TRUE;
				}
			} else {
				if (self->_private->selected_start > 0) {
					self->_private->selected_start--;
					allocate_selector (self);
					gtk_widget_queue_draw (widget);
					notify_changes (self);
					return TRUE;
				}
			}
			break;
		case GDK_Right:
			if ((event->state & GDK_SHIFT_MASK) == 0) {
				if (self->_private->selected_end < MIN (self->_private->elements_visible, display_get_range_size (self)) - 1) {
					self->_private->selected_end++;
					self->_private->selected_start = self->_private->selected_end;
					allocate_selector (self);
					gtk_widget_queue_draw (widget);
					notify_changes (self);
					return TRUE;
				} else if (self->_private->selected_start < MIN (self->_private->elements_visible, display_get_range_size (self)) - 1) {
					self->_private->selected_start = self->_private->selected_end;
					allocate_selector (self);
					gtk_widget_queue_draw (widget);
					notify_changes (self);
					return TRUE;
				}
			} else {
				if (self->_private->selected_end < MIN (self->_private->elements_visible, display_get_range_size (self)) - 1) {
					self->_private->selected_end++;
					allocate_selector (self);
					gtk_widget_queue_draw (widget);
					notify_changes (self);
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

	self->_private->elements_visible = (allocation->width - 1) / (DEFAULT_SIZE + 1);
	self->_private->element_size = (allocation->width - 1) / self->_private->elements_visible - 1;

	if (self->_private->offset != 0) {
		self->_private->offset = MIN (self->_private->offset, display_get_range_size (self) - self->_private->elements_visible);
	}

	GTK_WIDGET_CLASS (display_parent_class)->size_allocate (widget, allocation);

	allocate_selector (self);

	notify_changes (self);
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

	object_class->get_property = display_get_property;
	object_class->finalize     = display_finalize;

	widget_class->expose_event    = display_expose_event;
	widget_class->key_press_event = display_key_press_event;
	widget_class->size_allocate   = display_size_allocate;
	widget_class->size_request    = display_size_request;

	g_object_class_install_property (object_class, PROP_CAN_STEP_LEFT,
					 g_param_spec_boolean ("can-step-left", NULL, NULL,
							       FALSE, G_PARAM_READABLE));
	g_object_class_install_property (object_class, PROP_CAN_STEP_RIGHT,
					 g_param_spec_boolean ("can-step-right", NULL, NULL,
							       FALSE, G_PARAM_READABLE));
	g_object_class_install_property (object_class, PROP_CAN_ZOOM_IN,
					 g_param_spec_boolean ("can-zoom-in", NULL, NULL,
							       FALSE, G_PARAM_READABLE));
	g_object_class_install_property (object_class, PROP_CAN_ZOOM_OUT,
					 g_param_spec_boolean ("can-zoom-out", NULL, NULL,
							       FALSE, G_PARAM_READABLE));

	g_type_class_add_private (self_class, sizeof (DisplayPrivate));
}

/* Public API Implementation */

GtkWidget*
display_new (void)
{
	return g_object_new (TYPE_DISPLAY, NULL);
}

gboolean
display_can_step_left (Display const* self)
{
	g_return_val_if_fail (IS_DISPLAY (self), FALSE);

	return self->_private->offset > 0;
}

gboolean
display_can_step_right (Display const* self)
{
	g_return_val_if_fail (IS_DISPLAY (self), FALSE);

	return (self->_private->offset + self->_private->elements_visible) < display_get_range_size (self);
}

gboolean
display_can_zoom_in (Display const* self)
{
	g_return_val_if_fail (IS_DISPLAY (self), FALSE);

	return self->_private->zoom < TIME_PERIOD_MONTH;
}

gboolean
display_can_zoom_out (Display const* self)
{
	g_return_val_if_fail (IS_DISPLAY (self), FALSE);

	return self->_private->zoom > TIME_PERIOD_YEAR;
}

void
display_set_label_column (Display* self,
			  gint     column)
{
	g_return_if_fail (IS_DISPLAY (self));
	g_return_if_fail (column >= -1);

	self->_private->column_label = column;
	gtk_widget_queue_draw (GTK_WIDGET (self));

	// FIXME: g_object_notify (G_OBJECT (self), "label-column");
}

void
display_set_value_column (Display* self,
			  gint     column)
{
	g_return_if_fail (IS_DISPLAY (self));
	g_return_if_fail (column >= -1);

	self->_private->column_value = column;
	// FIXME: update size-request

	// FIXME: g_object_notify (G_OBJECT (self), "value-column");
}

void
display_set_model (Display      * self,
		   GtkTreeModel * model)
{
	g_return_if_fail (IS_DISPLAY (self));
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

	/* update the widget */
	notify_changes (self);

	// FIXME: g_object_notify (G_OBJECT (self), "model");
}

void
display_step_left (Display* self)
{
	g_return_if_fail (IS_DISPLAY (self));
	g_return_if_fail (display_can_step_left (self));

	self->_private->offset--;
	allocate_selector (self);
	gtk_widget_queue_draw (GTK_WIDGET (self));

	notify_changes (self);
}

void
display_step_right (Display* self)
{
	g_return_if_fail (IS_DISPLAY (self));
	g_return_if_fail (display_can_step_right (self));

	self->_private->offset++;
	allocate_selector (self);
	gtk_widget_queue_draw (GTK_WIDGET (self));

	notify_changes (self);
}

void
display_zoom_in (Display* self)
{
	g_return_if_fail (IS_DISPLAY (self));
	g_return_if_fail (display_can_zoom_in (self));

	self->_private->zoom++;
	gtk_widget_queue_draw (GTK_WIDGET (self));

	notify_changes (self);
}

void
display_zoom_out (Display* self)
{
	g_return_if_fail (IS_DISPLAY (self));
	g_return_if_fail (display_can_zoom_out (self));

	self->_private->zoom--;
	gtk_widget_queue_draw (GTK_WIDGET (self));

	notify_changes (self);
}

