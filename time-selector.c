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

#include "time-selector.h"

#define HORIZONTAL_BAR_HEIGHT 9
#define VERTICAL_BAR_WIDTH 3

/* GType Implementation */

G_DEFINE_TYPE (TimeSelector, time_selector, GTK_TYPE_WIDGET);

static void
time_selector_init (TimeSelector* self)
{
	GTK_WIDGET_SET_FLAGS (self, GTK_NO_WINDOW);
}

static gboolean
selector_expose_event (GtkWidget     * widget,
		       GdkEventExpose* event)
{
	GdkRectangle rectangles[] = {
		/* above the eye */
		{widget->allocation.x, widget->allocation.y,
		 widget->allocation.width, HORIZONTAL_BAR_HEIGHT},
		/* below the eye */
		{widget->allocation.x,
		 widget->allocation.y + widget->allocation.height - HORIZONTAL_BAR_HEIGHT,
		 widget->allocation.width, HORIZONTAL_BAR_HEIGHT},
		/* left of the eye */
		{widget->allocation.x, widget->allocation.y + HORIZONTAL_BAR_HEIGHT,
		 VERTICAL_BAR_WIDTH, widget->allocation.height - 2 * HORIZONTAL_BAR_HEIGHT},
		/* right of the eye */
		{widget->allocation.x + widget->allocation.width - VERTICAL_BAR_WIDTH,
		 widget->allocation.y + HORIZONTAL_BAR_HEIGHT,
		 VERTICAL_BAR_WIDTH, widget->allocation.height - 2 * HORIZONTAL_BAR_HEIGHT}
	};
	gsize i;

	for (i = 0; i < G_N_ELEMENTS (rectangles); i++) {
		gtk_paint_box (widget->style,
			       widget->window,
			       GTK_STATE_NORMAL,
			       GTK_SHADOW_OUT,
			       &rectangles[i],
			       widget,
			       NULL,
			       widget->allocation.x,
			       widget->allocation.y,
			       widget->allocation.width,
			       widget->allocation.height);
	}

	/* inner shadow */
	gtk_paint_shadow (widget->style,
			  widget->window,
			  GTK_STATE_NORMAL,
			  GTK_SHADOW_IN,
			  &widget->allocation,
			  widget,
			  NULL,
			  widget->allocation.x + VERTICAL_BAR_WIDTH,
			  widget->allocation.y + HORIZONTAL_BAR_HEIGHT,
			  widget->allocation.width - 2 * VERTICAL_BAR_WIDTH,
			  widget->allocation.height - 2 * HORIZONTAL_BAR_HEIGHT);

	return FALSE;
}

static void
time_selector_class_init (TimeSelectorClass* self_class)
{
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (self_class);

	widget_class->expose_event  = selector_expose_event;
}

/* Public API Implementation */

GtkWidget*
time_selector_new (void)
{
	return g_object_new (TYPE_TIME_SELECTOR,
			     NULL);
}

