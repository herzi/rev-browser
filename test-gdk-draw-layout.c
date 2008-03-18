/* This file is part of rev-lister
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

#include "gdk-cairo.h"

#include "testcase.h"

static void
test_gdk_cb (Testcase   * self,
	     GdkDrawable* drawable,
	     GdkGC      * gc,
	     PangoLayout* layout)
{
	gdk_draw_layout (drawable,
			 gc,
			 20, 20,
			 layout);
}

static void
test_cairo_cb (Testcase   * self,
	       cairo_t    * cr,
	       GdkGC      * gc,
	       PangoLayout* layout)
{
	gdk_cairo_draw_layout (cr,
			       gc,
			       20, 20,
			       layout);
}

int
main (int   argc,
      char**argv)
{
	PangoContext* context;
	PangoLayout * layout;
	Testcase    * testcase;
	gboolean      passed = TRUE;

	/* prepare */
	gtk_init (&argc, &argv);

	testcase = testcase_new ();
	context  = gdk_pango_context_get ();
	layout   = pango_layout_new (context);
	pango_layout_set_text (layout, "Foo Bar Baz", -1);

	/* exercise */
	g_signal_connect (testcase, "exercise-gdk",
			  G_CALLBACK (test_gdk_cb), layout);
	g_signal_connect (testcase, "exercise-cairo",
			  G_CALLBACK (test_cairo_cb), layout);
	testcase_exercise (testcase);

	/* verify */
	passed = testcase_get_passed (testcase);

	/* cleanup */
	g_object_unref (testcase);
	g_object_unref (layout);
	g_object_unref (context);

	return passed ? 0 : 1;
}

