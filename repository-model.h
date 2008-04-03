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

#ifndef REPOSITORY_MODEL_H
#define REPOSITORY_MODEL_H

#include <gtk/gtk.h>
#include "repository.h"

G_BEGIN_DECLS

typedef struct _RepositoryModel        RepositoryModel;
typedef struct _RepositoryModelPrivate RepositoryModelPrivate;
typedef struct _RepositoryModelClass   RepositoryModelClass;

#define TYPE_REPOSITORY_MODEL         (repository_model_get_type ())
#define REPOSITORY_MODEL(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_REPOSITORY_MODEL, RepositoryModel))
#define REPOSITORY_MODEL_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_REPOSITORY_MODEL, RepositoryModelClass))
#define IS_REPOSITORY_MODEL(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), TYPE_REPOSITORY_MODEL))
#define IS_REPOSITORY_MODEL_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), TYPE_REPOSITORY_MODEL))
#define REPOSITORY_MODEL_GET_CLASS(c) (G_TYPE_INSTANCE_GET_CLASS ((i), TYPE_REPOSITORY_MODEL, RepositoryModelClass))

GType         repository_model_get_type (void);
GtkTreeModel* repository_model_new      (Repository* repository);

struct _RepositoryModel {
	GObject                 base_instance;
	RepositoryModelPrivate* _private;
};

struct _RepositoryModelClass {
	GObjectClass            base_class;
};

G_END_DECLS

#endif /* !REPOSITORY_MODEL_H */
