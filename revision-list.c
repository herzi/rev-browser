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

#include "revision-list.h"

#include <sys/types.h>
#include <sys/wait.h>

#include <gfc-job.h>

static void
read_line_cb (GfcReader  * reader,
	      gchar const* line,
	      GPtrArray  * array)
{
	g_ptr_array_add (array,
			 g_strdup (line));
}

GfcJob*
revision_list_get_job (gchar const* location)
{
	return gfc_job_new (location,
			    "git-rev-list --all --pretty=format:%ai");
}

gchar**
revision_list_get_lines (void)
{
	gboolean  result = TRUE;
	GError  * error  = NULL;
	gchar   **out    = NULL;
	gchar   * err    = NULL;
	gint      status = 0;
	gchar   * working_folder = g_get_current_dir ();

	GMainLoop* loop = g_main_loop_new (NULL, FALSE);
	GPtrArray* array = g_ptr_array_new ();
	GfcJob* job = revision_list_get_job (working_folder);
	g_signal_connect         (gfc_job_get_out_reader (job), "read-line",
				  G_CALLBACK (read_line_cb), array);
	g_signal_connect_swapped (job, "done",
				  G_CALLBACK (g_main_loop_quit), loop);
	g_main_loop_run (loop);
	g_main_loop_unref (loop);
	result = gfc_job_get_return_code (job) == 0;
	g_object_unref (job);
	g_free (working_folder);

	g_ptr_array_add (array, NULL); /* terminate properly */
	out = (gchar**)array->pdata;
	g_ptr_array_free (array, FALSE);

	if (!result || error) {
		g_warning ("Error executing 'git-rev-list': %s",
			   error ? error->message : "no error message found");
		g_clear_error (&error);
		g_strfreev (out);
		g_free (err);
		return NULL;
	};

	if (!WIFEXITED (status)) {
		g_warning ("git-rev-list didn't exit cleanly");
		g_strfreev (out);
		g_free (err);
		return NULL;
	}

	if (WEXITSTATUS (status)) {
		g_warning ("git-rev-list didn't return 0 but %d",
			   WEXITSTATUS (status));
		g_strfreev (out);
		g_free (err);
		return NULL;
	}

	if (err && !*err) {
		g_free (err);
		err = NULL;
	}

	if (err) {
		g_warning ("git-rev-lister wrote an error:\n%s",
			   err);
		g_strfreev (out);
		g_free (err);
		return NULL;
	}

	g_return_val_if_fail (out, NULL); /* for the warning, to see if it's possible */

	return out;
}

