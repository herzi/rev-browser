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

#include "repository-model.h"

struct _RepositoryModelPrivate {
	Repository* repository;
};

enum {
	PROP_0,
	PROP_REPOSITORY
};

/* GtkTreeIter format:
 * user_data: position
 * user_data2: (unused)
 * user_data3: (unused)
 */

#define PRIV(i) REPOSITORY_MODEL(i)->_private

static GType repository_model_columns[REPOSITORY_MODEL_N_COLUMNS] = {0};

/* GType Implementation */

void implement_gtk_tree_model (GtkTreeModelIface* iface);
G_DEFINE_TYPE_WITH_CODE (RepositoryModel, repository_model, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_MODEL, implement_gtk_tree_model));

static void
repository_model_init (RepositoryModel* self)
{
	PRIV(self) = G_TYPE_INSTANCE_GET_PRIVATE (self, TYPE_REPOSITORY_MODEL, RepositoryModelPrivate);
}

static void
repository_model_constructed (GObject* object)
{
	g_assert (PRIV(object)->repository);

	if (G_OBJECT_CLASS (repository_model_parent_class)->constructed) {
		G_OBJECT_CLASS (repository_model_parent_class)->constructed (object);
	}
}

static void
repository_model_emit_row_changed (Repository     * repository,
				   gint             index,
				   gchar const    * date,
				   RepositoryModel* self)
{
	GtkTreePath* path = gtk_tree_path_new ();
	GtkTreeIter  iter;
	gtk_tree_path_append_index (path, index);

	if (!gtk_tree_model_get_iter (GTK_TREE_MODEL (self),
				      &iter,
				      path))
	{
		g_warning ("%s(): this is strange - now the model is probably broken",
			   G_STRFUNC);
	} else {
		gtk_tree_model_row_changed (GTK_TREE_MODEL (self),
					    path,
					    &iter);
	}

	gtk_tree_path_free (path);
}

static void
repository_model_emit_row_inserted (Repository     * repository,
				    gint             index,
				    gchar const    * date,
				    RepositoryModel* self)
{
	GtkTreePath* path = gtk_tree_path_new ();
	GtkTreeIter  iter;
	gtk_tree_path_append_index (path, index);

	if (!gtk_tree_model_get_iter (GTK_TREE_MODEL (self),
				      &iter,
				      path))
	{
		g_warning ("%s(): this is strange - now the model is probably broken",
			   G_STRFUNC);
	} else {
		gtk_tree_model_row_inserted (GTK_TREE_MODEL (self),
					     path,
					     &iter);
	}

	gtk_tree_path_free (path);
}

static void
repository_model_finalize (GObject* object)
{
	g_signal_handlers_disconnect_by_func (PRIV(object)->repository,
					      repository_model_emit_row_changed,
					      object);
	g_signal_handlers_disconnect_by_func (PRIV(object)->repository,
					      repository_model_emit_row_inserted,
					      object);
	g_object_unref (PRIV(object)->repository);

	G_OBJECT_CLASS (repository_model_parent_class)->finalize (object);
}

static void
repository_get_property (GObject   * object,
			 guint       prop_id,
			 GValue    * value,
			 GParamSpec* pspec)
{
	RepositoryModel* self = REPOSITORY_MODEL (object);

	switch (prop_id) {
	case PROP_REPOSITORY:
		g_value_set_object (value, self->_private->repository);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
repository_model_set_property (GObject     * object,
			       guint         prop_id,
			       GValue const* value,
			       GParamSpec  * pspec)
{
	RepositoryModel* self = REPOSITORY_MODEL (object);

	switch (prop_id) {
	case PROP_REPOSITORY:
		g_return_if_fail (!self->_private->repository);
		PRIV(self)->repository = g_value_dup_object (value);
		g_signal_connect (PRIV(self)->repository, "date-changed",
				  G_CALLBACK (repository_model_emit_row_changed), self);
		g_signal_connect (PRIV(self)->repository, "new-date",
				  G_CALLBACK (repository_model_emit_row_inserted), self);
		g_object_notify (object, "repository");
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
repository_model_class_init (RepositoryModelClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->constructed  = repository_model_constructed;
	object_class->finalize     = repository_model_finalize;
	object_class->get_property = repository_get_property;
	object_class->set_property = repository_model_set_property;

	g_object_class_install_property (object_class, PROP_REPOSITORY,
					 g_param_spec_object ("repository", NULL, NULL,
							      TYPE_REPOSITORY,
							      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	repository_model_columns[REPOSITORY_MODEL_COLUMN_DAY] = G_TYPE_STRING;
	repository_model_columns[REPOSITORY_MODEL_COLUMN_COMMITS] = G_TYPE_INT;

	g_type_class_add_private (self_class, sizeof (RepositoryModelPrivate));
}

/* GtkTreeModel Implementation */

static GtkTreeModelFlags
repository_get_flags (GtkTreeModel* model)
{
	return GTK_TREE_MODEL_LIST_ONLY;
}

static gboolean
repository_model_get_iter (GtkTreeModel* model,
			   GtkTreeIter * iter,
			   GtkTreePath * path)
{
	g_return_val_if_fail (path, FALSE);
	g_return_val_if_fail (gtk_tree_path_get_depth (path) == 1, FALSE);

	return gtk_tree_model_iter_nth_child (model, iter, NULL,
					      gtk_tree_path_get_indices(path)[0]);
}

static gint
repository_model_get_n_columns (GtkTreeModel* model)
{
	return REPOSITORY_MODEL_N_COLUMNS;
}

static void
repository_model_get_value (GtkTreeModel* model,
			    GtkTreeIter * iter,
			    gint          column,
			    GValue      * value)
{
	g_return_if_fail (column >= 0);
	g_return_if_fail (column < REPOSITORY_MODEL_N_COLUMNS);

	g_value_init (value,
		      repository_model_columns[column]);

	switch (column) {
	case REPOSITORY_MODEL_COLUMN_DAY:
		g_value_set_string (value,
				    repository_get_date (PRIV(model)->repository,
							 GPOINTER_TO_INT (iter->user_data)));
		break;
	case REPOSITORY_MODEL_COLUMN_COMMITS:
		g_value_set_int (value,
				 repository_get_commits (PRIV(model)->repository,
							 GPOINTER_TO_INT (iter->user_data)));
		break;
	default:
		g_warning ("%s(): invlid column number %d",
			   G_STRFUNC,
			   column);
		break;
	}
}

static gint
repository_iter_n_children (GtkTreeModel* model,
			    GtkTreeIter * iter)
{
	g_return_val_if_fail (!iter, 0);

	return repository_get_n_dates (PRIV(model)->repository);
}

static gboolean
repository_model_iter_next (GtkTreeModel* model,
			    GtkTreeIter * iter)
{
	iter->user_data = GINT_TO_POINTER (GPOINTER_TO_INT (iter->user_data) + 1);

	return GPOINTER_TO_INT (iter->user_data) < repository_get_n_dates (PRIV(model)->repository);
}

static gboolean
repository_model_iter_nth_child (GtkTreeModel* model,
				 GtkTreeIter * iter,
				 GtkTreeIter * parent,
				 gint          index)
{
	g_return_val_if_fail (!parent, FALSE);
	g_return_val_if_fail (index >= 0, FALSE);

	if (index >= repository_get_n_dates (PRIV(model)->repository)) {
		return FALSE;
	}

	iter->user_data = GINT_TO_POINTER (index);

	return TRUE;
}

void
implement_gtk_tree_model (GtkTreeModelIface* iface)
{
	iface->get_flags       = repository_get_flags;
	iface->get_iter        = repository_model_get_iter;
	iface->get_n_columns   = repository_model_get_n_columns;
	iface->get_value       = repository_model_get_value;
	iface->iter_n_children = repository_iter_n_children;
	iface->iter_next       = repository_model_iter_next;
	iface->iter_nth_child  = repository_model_iter_nth_child;
}

/* Public API Implementation */

GtkTreeModel*
repository_model_new (Repository* repository)
{
	g_return_val_if_fail (IS_REPOSITORY (repository), NULL);

	return g_object_new (TYPE_REPOSITORY_MODEL,
			     "repository", repository,
			     NULL);
}

