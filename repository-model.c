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

/* GType Implementation */

void implement_gtk_tree_model (GtkTreeModelIface* iface);
G_DEFINE_TYPE_WITH_CODE (RepositoryModel, repository_model, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_MODEL, implement_gtk_tree_model));

static void
repository_model_init (RepositoryModel* self)
{}

static void
repository_model_class_init (RepositoryModelClass* self_class)
{}

/* GtkTreeModel Implementation */

static GtkTreeModelFlags
repository_get_flags (GtkTreeModel* model)
{
	return GTK_TREE_MODEL_LIST_ONLY;
}

static gint
repository_iter_n_children (GtkTreeModel* model,
			    GtkTreeIter * iter)
{
	g_return_val_if_fail (!iter, 0);

	return 0;
}

void
implement_gtk_tree_model (GtkTreeModelIface* iface)
{
	iface->get_flags       = repository_get_flags;
	iface->iter_n_children = repository_iter_n_children;
}

/* Public API Implementation */

GtkTreeModel*
repository_model_new (void)
{
	return g_object_new (TYPE_REPOSITORY_MODEL,
			     NULL);
}

