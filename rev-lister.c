/* This file is part of rev-lister
 *
 * AUTHORS
 *     Sven Herzberg  <herzi@gnome-de.org>
 *
 * Copyright (C) 2007  Sven Herzberg
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

#include <glib.h>

int
main (int   argc,
      char**argv)
{
	GError* error  = NULL;
	gchar * out    = NULL;
	gint    status = 0;
	gchar **lines  = NULL;
	gchar **iter;

	g_spawn_command_line_sync ("git-rev-list --all --pretty=format:%at", &out, NULL, &status, &error);

	if (error) {
		g_warning ("Error executing 'git-rev-list': %s",
			   error->message);
		g_error_free (error);
		g_free (out);
		return 1;
	};

	if (status != 0) {
		g_warning ("git-rev-list didn't return 0");
		g_free (out);
		return 2;
	}

	lines = g_strsplit (out, "\n", -1);
	for (iter = lines; iter && *iter; iter++) {
		if (G_LIKELY (**iter)) {
			if (!g_str_has_prefix (*iter, "commit ")) {
				g_print ("git-rev-list: %s\n", *iter);
			}
		}
	}
	g_free (out);

	return 0;
}

