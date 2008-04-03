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
	      GString    * string)
{
	g_string_append_printf (string,
				"%s\n",
				line);
}

static gboolean
my_sync_spawn (gchar **out,
	       gchar **err,
	       gint  * status,
	       GError**error)
{
	gchar const* command = "git-rev-list --all --pretty=format:%ai";
	GMainLoop* loop = g_main_loop_new (NULL, FALSE);
	GString  * string  = g_string_new ("");
	GfcJob* job = gfc_job_new (NULL,
				   command);
	g_signal_connect         (gfc_job_get_out_reader (job), "read-line",
				  G_CALLBACK (read_line_cb), string);
	g_signal_connect_swapped (job, "done",
				  G_CALLBACK (g_main_loop_quit), loop);
	g_main_loop_run (loop);
	g_object_unref (job);

	g_main_loop_unref (loop);
	*out = string->str;
	g_string_free (string, FALSE);
	return TRUE;
}

static gchar*
revision_list_get (void)
{
	gboolean  result = TRUE;
	GError  * error  = NULL;
	gchar   * out    = NULL;
	gchar   * err    = NULL;
	gint      status = 0;

	result = my_sync_spawn (&out,
				&err,
				&status,
				&error);

	if (!result || error) {
		g_warning ("Error executing 'git-rev-list': %s",
			   error ? error->message : "no error message found");
		g_clear_error (&error);
		g_free (out);
		g_free (err);
		return NULL;
	};

	if (!WIFEXITED (status)) {
		g_warning ("git-rev-list didn't exit cleanly");
		g_free (out);
		g_free (err);
		return NULL;
	}

	if (WEXITSTATUS (status)) {
		g_warning ("git-rev-list didn't return 0 but %d",
			   WEXITSTATUS (status));
		g_free (out);
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
		g_free (out);
		g_free (err);
		return NULL;
	}

	g_return_val_if_fail (out, NULL); /* for the warning, to see if it's possible */

	return out;
}

gchar**
revision_list_get_lines (void)
{
	gchar* out   = NULL;
	gchar**lines = NULL;

	out = revision_list_get ();
	if (!out) {
		return NULL;
	}

	lines = g_strsplit (out, "\n", -1);
	g_free (out);

	return lines;
}

