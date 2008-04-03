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

struct _RepositoryPrivate {
	GHashTable* num_commits_per_day;
};

#define PRIV(i) REPOSITORY(i)->_private

/* GType Implementation */

G_DEFINE_TYPE (Repository, repository, G_TYPE_OBJECT);

static void
repository_init (Repository* self)
{
	PRIV(self) = G_TYPE_INSTANCE_GET_PRIVATE (self, TYPE_REPOSITORY, RepositoryPrivate);
	PRIV(self)->num_commits_per_day = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
}

static void
repository_finalize (GObject* object)
{
	g_hash_table_unref (PRIV(object)->num_commits_per_day);

	G_OBJECT_CLASS (repository_parent_class)->finalize (object);
}

static void
repository_class_init (RepositoryClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = repository_finalize;

	g_type_class_add_private (self_class, sizeof (RepositoryPrivate));
}

/* Public API implementation */

Repository*
repository_new (void)
{
	Repository* self;
	gchar     **lines  = NULL;
	gchar     **iter;

	lines = revision_list_get_lines ();
	self = g_object_new (TYPE_REPOSITORY, NULL);
	for (iter = lines; iter && *iter; iter++) {
		if (G_LIKELY (**iter)) {
			if (!g_str_has_prefix (*iter, "commit ")) {
				gchar** words = g_strsplit (*iter, " ", 2);
				gsize count = GPOINTER_TO_SIZE (g_hash_table_lookup (PRIV(self)->num_commits_per_day, words[0]));
				count++;
				g_hash_table_insert (PRIV(self)->num_commits_per_day,
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
		    GHFunc      func,
		    gpointer    user_data)
{
	g_hash_table_foreach (PRIV(self)->num_commits_per_day,
			      func,
			      user_data);
}

