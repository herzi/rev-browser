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

#include "repository.h"

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
	Repository* repository;

	g_type_init ();

	repository = repository_new ();
	repository_foreach (repository, (GHFunc)print_revs, NULL);
	g_object_unref (repository);

	return 0;
}

