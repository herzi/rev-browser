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

#include "repository.h"

#include "revision-list.h"

Repository*
repository_new (void)
{
	GHashTable* self;
	gchar     **lines  = NULL;
	gchar     **iter;

	lines = revision_list_get_lines ();
	self = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
	for (iter = lines; iter && *iter; iter++) {
		if (G_LIKELY (**iter)) {
			if (!g_str_has_prefix (*iter, "commit ")) {
				gchar** words = g_strsplit (*iter, " ", 2);
				gsize count = GPOINTER_TO_SIZE (g_hash_table_lookup (self, words[0]));
				count++;
				g_hash_table_insert (self,
						     g_strdup (words[0]),
						     GSIZE_TO_POINTER (count));
				g_strfreev (words);
			}
		}
	}
	g_strfreev (lines);
	return self;
}

void
repository_foreach (Repository* self,
		    GHFunc    * func,
		    gpointer    user_data)
{
	g_hash_table_foreach (self,
			      func,
			      user_data);
}

void
repository_unref (Repository* self)
{
	g_hash_table_unref (self);
}

