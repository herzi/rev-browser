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
repository_model_finalize (GObject* object)
{
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
repository_set_property (GObject     * object,
			 guint         prop_id,
			 GValue const* value,
			 GParamSpec  * pspec)
{
	RepositoryModel* self = REPOSITORY_MODEL (object);

	switch (prop_id) {
	case PROP_REPOSITORY:
		g_return_if_fail (!self->_private->repository);
		self->_private->repository = g_value_dup_object (value);
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
	object_class->set_property = repository_set_property;

	g_object_class_install_property (object_class, PROP_REPOSITORY,
					 g_param_spec_object ("repository", NULL, NULL,
							      TYPE_REPOSITORY,
							      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

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
	g_return_if_fail (column < gtk_tree_model_iter_n_children (model, NULL));

	g_value_init (value,
		      repository_model_columns[column]);

	// FIXME: set the value properly
}

static gint
repository_iter_n_children (GtkTreeModel* model,
			    GtkTreeIter * iter)
{
	g_return_val_if_fail (!iter, 0);

	return repository_get_n_dates (PRIV(model)->repository);
}

static gboolean
repository_model_iter_nth_child (GtkTreeModel* model,
				 GtkTreeIter * iter,
				 GtkTreeIter * parent,
				 gint          index)
{
	g_return_val_if_fail (!parent, FALSE);
	g_return_val_if_fail (index >= 0, FALSE);
	g_return_val_if_fail (index < repository_get_n_dates (PRIV(model)->repository), FALSE);

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

