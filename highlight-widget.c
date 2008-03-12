/* This file is part of rev-browser
 *
 * AUTHORS
 *     Sven Herzberg  <sven@imendio.com>
 *
 * Copyright (C) 2008  Sven Herzberg
 *
 * This work is provided "as is"; redistribution and modification
 * in whole or in part, in any medium, physical or electronic is
 * permitted without restriction.
 *
 * This work is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In no event shall the authors or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 */

#include "highlight-widget.h"

static GQuark highlight_pixbuf_quark = 0;

static void
update_highlight_pixbuf (GtkWidget* widget)
{
	GdkPixbuf* pixbuf = g_object_get_qdata (G_OBJECT (widget),
					        highlight_pixbuf_quark);

	if (!pixbuf ||
	    widget->allocation.width != gdk_pixbuf_get_width (pixbuf) ||
	    widget->allocation.height != gdk_pixbuf_get_height (pixbuf))
	{
		pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB,
					 TRUE,
					 8,
					 widget->allocation.width,
					 widget->allocation.height);

		gdk_pixbuf_fill (pixbuf, 0xff00007f);

		g_object_set_qdata_full (G_OBJECT (widget),
					 highlight_pixbuf_quark,
					 pixbuf,
					 g_object_unref);
	}
}

void
highlight_widget (GtkWidget* widget)
{
	g_return_if_fail (GTK_IS_WIDGET (widget));

	if (!highlight_pixbuf_quark) {
		highlight_pixbuf_quark = g_quark_from_static_string ("RefBrowserHighlightPixbuf");
	}

	g_signal_connect_after (widget, "size-allocate",
				G_CALLBACK (update_highlight_pixbuf), NULL);

	update_highlight_pixbuf (widget);
}

