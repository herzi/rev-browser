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

#include "highlight-widget.h"
#include "time-period.h"
#include "time-selector.h"

#define DEFAULT_SIZE     70 /* the default size and minimum of an element */
#define VERTICAL_PADDING 6
#define TEXT_OFFSET      5

/* TODO:
 * - implement the book-keeping of the visible area with GtkTreeRowReference
 * - implement the selection in a more dynamic way (so the selection stays the
 *   same even if the repository changes)
 */

struct _DisplayPrivate {
	GtkWidget   * selector;

	/* the data model and display settings */
	GtkTreeModel* model;
	gint          column_label;
	gint          column_value;

	/* range settings */
	guint        selected_start;
	guint        selected_end;

	/* size state */
	guint        elements_visible;
	gint         element_size;

	/* display state */
	guint        update_idle;     /* idle job to check the dimensions from the tree model */
	gint         maximum_value;   /* maxmimum value in the model */
	gint         label_max_width;
	gint         offset;
	TimePeriod   zoom;

	/* input only window */
	GdkWindow  * event_window;
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

	self->_private->label_max_width = DEFAULT_SIZE;
	self->_private->selector = time_selector_new ();
	gtk_widget_show (self->_private->selector);
	gtk_container_add (GTK_CONTAINER (self),
			   self->_private->selector);

	self->_private->column_label = -1;
	self->_private->column_value = -1;

	self->_private->element_size = 33;
	self->_private->zoom       = TIME_PERIOD_YEAR;
}

static void
display_dispose (GObject* object)
{
	display_set_model (DISPLAY (object), NULL);

	G_OBJECT_CLASS (display_parent_class)->dispose (object);
}

static void
display_finalize (GObject* object)
{
	Display* self = DISPLAY (object);

	if (self->_private->update_idle) {
		g_source_remove (self->_private->update_idle);
	}

	G_OBJECT_CLASS (display_parent_class)->finalize (object);
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

static guint
display_get_range_size (Display const* self)
{
	if (G_UNLIKELY (!self->_private->model)) {
		return 0;
	}

	return gtk_tree_model_iter_n_children (self->_private->model,
					       NULL);
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
	pango_layout_set_width (layout, PANGO_SCALE * (self->_private->element_size - 2 * TEXT_OFFSET));

	for (i = 0; i < self->_private->elements_visible; i++) {
		GtkTreeIter  iter;

		if (G_LIKELY (i)) {
			gdk_cairo_draw_line (cr,
					     widget->style->black_gc,
					     widget->allocation.x + i * (self->_private->element_size + 1),
					     widget->allocation.y + VERTICAL_PADDING + 1,
					     widget->allocation.x + i * (self->_private->element_size + 1),
					     widget->allocation.y + VERTICAL_PADDING + 7);
		}

		if (!self->_private->model ||
		    !gtk_tree_model_iter_nth_child (self->_private->model,
						    &iter,
						    NULL,
						    i + self->_private->offset))
		{
			continue;
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
					       widget->allocation.x + i * (self->_private->element_size + 1) + TEXT_OFFSET,
					       widget->allocation.y + VERTICAL_PADDING + TEXT_OFFSET,
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

			height = (widget->allocation.height - 35) * (1.0 * value / self->_private->maximum_value);
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

	return GTK_WIDGET_CLASS (display_parent_class)->expose_event (widget, event);
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
			if (!display_can_step_left (self)) {
				break;
			}

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
			if (!display_can_step_right (self)) {
				break;
			}

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

	if (GTK_WIDGET_REALIZED (widget)) {
		gdk_window_move_resize (self->_private->event_window,
					allocation->x, allocation->y,
					allocation->width, allocation->height);
	}

	notify_changes (self);
}

static void
display_size_request (GtkWidget     * widget,
		      GtkRequisition* req)
{
	Display* self = DISPLAY (widget);
	req->width = 3 * (self->_private->label_max_width + 1) + 1;
	req->height = 12 /* default white box size */
		      + 12 /* 6 pixels above and below the white box */
		      + 6  /* 3 pixels above and below the inner shadow */
		      + 4  /* ~2*shadow-width */;
}

static void
display_realize (GtkWidget *widget)
{
	Display* self = DISPLAY (widget);
	GdkWindowAttr attributes;
	gint attributes_mask;

	GTK_WIDGET_CLASS (display_parent_class)->realize (widget);

	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = widget->allocation.x;
	attributes.y = widget->allocation.y;
	attributes.width = widget->allocation.width;
	attributes.height = widget->allocation.height;
	attributes.wclass = GDK_INPUT_ONLY;
	attributes.event_mask = gtk_widget_get_events (widget);
	attributes.event_mask |= GDK_SCROLL_MASK;
	attributes_mask = GDK_WA_X | GDK_WA_Y;

	self->_private->event_window = gdk_window_new (widget->window, &attributes, attributes_mask);
	gdk_window_set_user_data (self->_private->event_window, widget);
}

static void
display_unrealize (GtkWidget *widget)
{
	Display* self = DISPLAY (widget);

	gdk_window_set_user_data (self->_private->event_window, NULL);
	g_object_unref (self->_private->event_window);
	self->_private->event_window = NULL;

	GTK_WIDGET_CLASS (display_parent_class)->unrealize (widget);
}

static void
display_map (GtkWidget *widget)
{
	Display* self = DISPLAY (widget);

	gdk_window_show (self->_private->event_window);
	GTK_WIDGET_CLASS (display_parent_class)->map (widget);
}

static void
display_unmap (GtkWidget *widget)
{
	Display* self = DISPLAY (widget);

	gdk_window_hide (self->_private->event_window);
	GTK_WIDGET_CLASS (display_parent_class)->map (widget);
}

static gboolean
display_scroll_event (GtkWidget      *widget,
		      GdkEventScroll *event)
{
	Display* self = DISPLAY (widget);

	switch (event->direction) {
	case GDK_SCROLL_DOWN:
	case GDK_SCROLL_RIGHT:
		if (display_can_step_right (self)) {
			display_step_right (self);
		}

		break;
	case GDK_SCROLL_UP:
	case GDK_SCROLL_LEFT:
		if (display_can_step_left (self)) {
			display_step_left (self);
		}

		break;
	default:
		g_warning ("Unknown scroll");
	}

	return TRUE;
}

static void
display_class_init (DisplayClass* self_class)
{
	GObjectClass  * object_class = G_OBJECT_CLASS (self_class);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (self_class);

	object_class->dispose      = display_dispose;
	object_class->finalize     = display_finalize;
	object_class->get_property = display_get_property;

	widget_class->expose_event    = display_expose_event;
	widget_class->key_press_event = display_key_press_event;
	widget_class->size_allocate   = display_size_allocate;
	widget_class->size_request    = display_size_request;
	widget_class->realize         = display_realize;
	widget_class->unrealize       = display_unrealize;
	widget_class->map             = display_map;
	widget_class->unmap           = display_unmap;
	widget_class->scroll_event    = display_scroll_event;

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

static gboolean
update_from_tree (gpointer data)
{
	GtkTreeIter  iter;
	Display* self = data;

	gint maximum = 0;
	gint maximum_width = DEFAULT_SIZE;

	if (self->_private->model &&
	    (self->_private->column_value != -1 ||
	     self->_private->column_label != -1) &&
	    gtk_tree_model_get_iter_first (self->_private->model,
					   &iter))
	{
		do {
			if (G_LIKELY (self->_private->column_value != -1)) {
				gint value = 0;

				gtk_tree_model_get (self->_private->model, &iter,
						    self->_private->column_value, &value,
						    -1);

				maximum = MAX (maximum, value);
			}
		} while (gtk_tree_model_iter_next (self->_private->model, &iter));
	}

	if (maximum_width != self->_private->label_max_width) {
		self->_private->label_max_width = maximum_width;
		gtk_widget_queue_resize (GTK_WIDGET (self));
	}

	if (maximum != self->_private->maximum_value) {
		self->_private->maximum_value = maximum;
		gtk_widget_queue_draw (GTK_WIDGET (self));
	}

	self->_private->update_idle = 0;
	return FALSE;
}

static void
queue_update_task (Display* self)
{
	/* queue an update of the current maximum, so we can normalize the
	 * stack heights */
	if (G_UNLIKELY (!self->_private->update_idle)) {
		self->_private->update_idle = g_idle_add_full (G_PRIORITY_HIGH_IDLE,
							       update_from_tree,
							       self,
							       NULL);
	}
}

void
display_set_value_column (Display* self,
			  gint     column)
{
	g_return_if_fail (IS_DISPLAY (self));
	g_return_if_fail (column >= -1);

	self->_private->column_value = column;
	/* update size-request */
	queue_update_task (self);

	// FIXME: g_object_notify (G_OBJECT (self), "value-column");
}

static void
display_cb_model_row_changed (GtkTreeModel* model,
			      GtkTreePath * path,
			      GtkTreeIter * iter,
			      Display     * self)
{
	queue_update_task (self);

	/* FIXME: check if visible; trigger potential redraw */
	gtk_widget_queue_draw (GTK_WIDGET (self));
}

static void
display_cb_model_row_inserted (GtkTreeModel* model,
			       GtkTreePath * path,
			       GtkTreeIter * iter,
			       Display     * self)
{
	queue_update_task (self);

	/* FIXME: check if visible; trigger potential redraw */
	gtk_widget_queue_draw (GTK_WIDGET (self));
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
		g_signal_handlers_disconnect_by_func (self->_private->model,
						      G_CALLBACK (display_cb_model_row_changed),
						      self);
		g_object_unref (self->_private->model);
		self->_private->model = NULL;
	}
	if (model) {
		self->_private->model = g_object_ref (model);
		g_signal_connect (self->_private->model, "row-changed",
				  G_CALLBACK (display_cb_model_row_changed), self);
		g_signal_connect (self->_private->model, "row-inserted",
				  G_CALLBACK (display_cb_model_row_inserted), self);
	}

	queue_update_task (self);

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

