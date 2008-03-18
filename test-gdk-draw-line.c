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

#include "gdk-cairo.h"

#include "testcase.h"

static void
test_gdk_cb (Testcase   * testcase,
	     GdkDrawable* drawable,
	     GdkGC      * gc)
{
	gdk_draw_line (drawable,
		       gc,
		       10, 20,
		       10, 30);
}

static void
test_cairo_cb (Testcase* testcase,
	       cairo_t * cr,
	       GdkGC   * gc)
{
	gdk_cairo_draw_line (cr, gc,
			     10, 20,
			     10, 30);
}

int
main (int   argc,
      char**argv)
{
	Testcase* testcase;
	gboolean  passed = TRUE;

	/* prepare */
	gtk_init (&argc, &argv);

	testcase = testcase_new ();

	/* exercise */
	g_signal_connect (testcase, "exercise-gdk",
			  G_CALLBACK (test_gdk_cb), NULL);
	g_signal_connect (testcase, "exercise-cairo",
			  G_CALLBACK (test_cairo_cb), NULL);
	testcase_exercise (testcase);

	/* verify */
	passed = testcase_get_passed (testcase);

	/* cleanup */
	g_object_unref (testcase);

	return passed ? 0 : 1;
}

