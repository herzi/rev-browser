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
	/* box above the eye */
	gtk_paint_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_NONE,
			    &widget->allocation,
			    widget,
			    NULL,
			    widget->allocation.x,
			    widget->allocation.y,
			    widget->allocation.width,
			    9);

	/* box below the eye */
	gtk_paint_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_NONE,
			    &widget->allocation,
			    widget,
			    NULL,
			    widget->allocation.x,
			    widget->allocation.y + widget->allocation.height - 9,
			    widget->allocation.width,
			    9);

	/* box left of the eye */
	gtk_paint_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_NONE,
			    &widget->allocation,
			    widget,
			    NULL,
			    widget->allocation.x,
			    widget->allocation.y + 9,
			    3,
			    widget->allocation.height - 18);

	/* box right of the eye */
	gtk_paint_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_NONE,
			    &widget->allocation,
			    widget,
			    NULL,
			    widget->allocation.x + widget->allocation.width - 3,
			    widget->allocation.y + 9,
			    3,
			    widget->allocation.height - 18);

	/* inner shadow */
	gtk_paint_shadow (widget->style,
			  widget->window,
			  GTK_STATE_NORMAL,
			  GTK_SHADOW_IN,
			  &widget->allocation,
			  widget,
			  NULL,
			  widget->allocation.x + 3,
			  widget->allocation.y + 9,
			  widget->allocation.width - 6,
			  widget->allocation.height - 18);

	/* outer shadow */
	gtk_paint_shadow (widget->style,
			  widget->window,
			  GTK_STATE_NORMAL,
			  GTK_SHADOW_OUT,
			  &widget->allocation,
			  widget,
			  NULL,
			  widget->allocation.x,
			  widget->allocation.y,
			  widget->allocation.width,
			  widget->allocation.height);
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

