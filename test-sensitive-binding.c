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

#include <glib/gtestutils.h>

#define implemented FALSE

static void
test_memory_widget_first (void)
{
	/* Objective:
	 * - Create an object-widget pair and a binding for it
	 * - destroy the widget
	 * - make sure everything went by cleanly
	 */
	//g_assert (implemented);
}

static void
test_memory_subject_first (void)
{
	/* Objective:
	 * - Create an object-widget pair and a binding for it
	 * - destroy the subject
	 * - make sure everything went by cleanly
	 */
	//g_assert (implemented);
}

int
main (int   argc,
      char**argv)
{
	g_test_init (&argc, &argv, NULL);

	// FIXME: test if connecting to multiple properties works
	// FIXME: test if the sensitivity-proxying works

	g_test_add_func ("/memory/widget-first", test_memory_widget_first);
	g_test_add_func ("/memory/subject-first", test_memory_subject_first);

	return g_test_run ();
}

