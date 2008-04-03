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

static gboolean
my_sync_spawn (gchar **out,
	       gchar **err,
	       gint  * status,
	       GError**error)
{
	/* FIXME: parse the stdout for debugging, too */
	return g_spawn_command_line_sync ("git-rev-list --all --pretty=format:%ai",
					  out,
					  err,
					  status,
					  error);

}

static gchar*
revision_list_get (void)
{
	gboolean  result = TRUE;
	GError  * error  = NULL;
	gchar   * out    = NULL;
	gchar   * err    = NULL;
	gint      status = 0;

	g_return_val_if_fail (!out || !*out, NULL);

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

	g_return_val_if_fail (out && *out, NULL); /* for the warning, to see if it's possible */

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

