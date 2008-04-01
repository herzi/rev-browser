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

#include "revision-list.h"

#define _XOPEN_SOURCE
#include <time.h>

static void
print_revs (gchar* key,
	    gpointer values,
	    gpointer unused /* max_values */)
{
	// FIXME: provide proper ascii art bars: #+-
	g_print ("%s: %d\n",
		 key,
		 GPOINTER_TO_SIZE (values));
}

int
main (int   argc,
      char**argv)
{
	gchar * out    = NULL;
	gchar **lines  = NULL;
	gchar **iter;

	if (!revision_list_get (&out)) {
		return 1;
	}

	GHashTable* revs = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
	lines = g_strsplit (out, "\n", -1);
	g_free (out);
	for (iter = lines; iter && *iter; iter++) {
		if (G_LIKELY (**iter)) {
			if (!g_str_has_prefix (*iter, "commit ")) {
				gchar** words = g_strsplit (*iter, " ", 2);
				gsize count = GPOINTER_TO_SIZE (g_hash_table_lookup (revs, words[0]));
				count++;
				g_hash_table_insert (revs, g_strdup (words[0]), GSIZE_TO_POINTER (count));
				g_strfreev (words);
			}
		}
	}
	g_strfreev (lines);
	g_hash_table_foreach (revs, (GHFunc)print_revs, NULL);

	return 0;
}

