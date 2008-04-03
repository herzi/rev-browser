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

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _Repository        Repository;
typedef struct _RepositoryPrivate RepositoryPrivate;
typedef struct _RepositoryClass   RepositoryClass;

#define TYPE_REPOSITORY         (repository_get_type ())
#define REPOSITORY(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_REPOSITORY, Repository))
#define REPOSITORY_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_REPOSITORY, RepositoryClass))
#define IS_REPOSITORY(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), TYPE_REPOSITORY))
#define IS_REPOSITORY_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), TYPE_REPOSITORY))
#define REPOSITORY_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), TYPE_REPOSITORY, RepositoryClass))

Repository* repository_new     (void);
void        repository_foreach (Repository* self,
				GHFunc      func,
				gpointer    user_data);

struct _Repository {
	GObject            base_instance;
	RepositoryPrivate* _private;
};

struct _RepositoryClass {
	GObjectClass       base_class;
};

G_END_DECLS

#endif /* !REPOSITORY_H */
