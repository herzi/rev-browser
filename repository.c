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

#include "marshallers.h"
#include "revision-list.h"

struct _RepositoryPrivate {
	GSequence* commits_per_day;
};

enum {
	NEW_DATE,
	N_SIGNALS
};

static guint repository_signals[N_SIGNALS] = {0};

#define PRIV(i) REPOSITORY(i)->_private

/* Commits-per-Day Implementation */
typedef struct {
	gchar* day;
	gint   num_commits;
} CommitsPerDay;

static CommitsPerDay*
commits_per_day_new (gchar const* day)
{
	CommitsPerDay* result = g_slice_new (CommitsPerDay);
	result->day = g_strdup (day);
	result->num_commits = 1;
	return result;
}

static void
commits_per_day_free (CommitsPerDay* self)
{
	g_free (self->day);
	g_slice_free (CommitsPerDay, self);
}

static gint
commits_per_day_compare (gconstpointer v1,
			 gconstpointer v2)
{
	gchar const* one = v1 ? ((CommitsPerDay*)v1)->day : "";
	gchar const* two = v2 ? ((CommitsPerDay*)v2)->day : "";
	return strcmp (one, two);
}

/* GType Implementation */

G_DEFINE_TYPE (Repository, repository, G_TYPE_OBJECT);

static void
repository_init (Repository* self)
{
	PRIV(self) = G_TYPE_INSTANCE_GET_PRIVATE (self, TYPE_REPOSITORY, RepositoryPrivate);
	PRIV(self)->commits_per_day = g_sequence_new ((GDestroyNotify)commits_per_day_free);
}

static void
repository_finalize (GObject* object)
{
	g_sequence_free (PRIV(object)->commits_per_day);

	G_OBJECT_CLASS (repository_parent_class)->finalize (object);
}

static void
repository_class_init (RepositoryClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = repository_finalize;

	repository_signals[NEW_DATE] = g_signal_new ("new-date", TYPE_REPOSITORY,
						     0, 0,
						     NULL, NULL,
						     repo_cclosure_marshal_VOID__INT_STRING,
						     G_TYPE_NONE, 2,
						     G_TYPE_INT,
						     G_TYPE_STRING);

	g_type_class_add_private (self_class, sizeof (RepositoryPrivate));
}

/* Public API implementation */

static void
dump (gpointer data,
      gpointer user_data G_GNUC_UNUSED)
{
	CommitsPerDay* cpd = data;
	g_print ("  %s => %d\n",
		 cpd->day,
		 cpd->num_commits);
}

static gpointer
my_g_sequence_find_data (GSequence       * sequence,
			 gpointer          data,
			 GCompareDataFunc  cmp_func,
			 gpointer          cmp_data)
{
	GSequenceIter* front  = g_sequence_get_begin_iter (sequence);
	GSequenceIter* back   = g_sequence_get_end_iter (sequence);

	while (front != back) {
		GSequenceIter* middle = g_sequence_range_get_midpoint (front, back);

		gint cmp = cmp_func (g_sequence_get (middle),
				     data,
				     cmp_data);

		if (cmp < 0) {
			if (middle == front)
				return NULL; /* not found - has to be prepended */

			front = middle;
		} else if (cmp > 0) {
			back = middle;
		} else {
			return g_sequence_get (middle);
		}
	}

	return NULL;
}

static void
repository_parse_line (Repository * self,
		       gchar const* line)
{
		if (G_LIKELY (*line)) {
			if (!g_str_has_prefix (line, "commit ")) {
				gchar** words = g_strsplit (line, " ", 2);
				CommitsPerDay* cpd = commits_per_day_new (words[0]);
				CommitsPerDay* match = my_g_sequence_find_data (PRIV(self)->commits_per_day,
										cpd,
										(GCompareDataFunc)commits_per_day_compare,
										NULL);

				if (match) {
					match->num_commits += cpd->num_commits;
					commits_per_day_free (cpd);
				} else {
					GSequenceIter* iter;
					iter = g_sequence_insert_sorted (PRIV(self)->commits_per_day,
									 cpd,
									 (GCompareDataFunc)commits_per_day_compare,
									 NULL);
					g_signal_emit (self,
						       repository_signals[NEW_DATE],
						       0,
						       g_sequence_iter_get_position (iter),
						       cpd->day);
				}

				g_strfreev (words);
			}
		}
}

Repository*
repository_new (void)
{
	Repository* self;
	gchar     **lines  = NULL;
	gchar     **iter;

	lines = revision_list_get_lines ();
	self = g_object_new (TYPE_REPOSITORY, NULL);
	for (iter = lines; iter && *iter; iter++) {
		repository_parse_line (self, *iter);
	}
	g_strfreev (lines);
	return self;
}

struct GHFuncAndUserData {
	GHFunc   func;
	gpointer user_data;
};

static void
translate_from_cpd_to_expected (gpointer key,
				gpointer user_data)
{
	CommitsPerDay const* cpd = key;
	struct GHFuncAndUserData* data = user_data;
	data->func (cpd->day, GINT_TO_POINTER (cpd->num_commits), data->user_data);
}

void
repository_foreach (Repository* self,
		    GHFunc      func,
		    gpointer    user_data)
{
	struct GHFuncAndUserData data = {func, user_data};
	g_sequence_foreach (PRIV(self)->commits_per_day,
			    translate_from_cpd_to_expected,
			    &data);
}

gint
repository_get_n_dates (Repository const* self)
{
	g_return_val_if_fail (IS_REPOSITORY (self), 0);

	return g_sequence_get_length (self->_private->commits_per_day);
}

