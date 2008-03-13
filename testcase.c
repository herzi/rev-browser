/* This file is part of rev-browser
 *
 * AUTHORS
 *     Sven Herzberg  <sven@imendio.com>
 *
 * Copyright (C) 2008  Sven Herzberg
 *
 * This work is provided "as is"; redistribution and modification
 * in whole or in part, in any medium, physical or electronic is
 * permitted without restriction.
 *
 * This work is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In no event shall the authors or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 */

#include "testcase.h"

struct _TestcasePrivate {
	GdkPixmap* gdk_pixmap;
	GdkPixmap* cairo_pixmap;
};

#define PRIV(i) (TESTCASE(i)->_private)

/* GType Implementation */

G_DEFINE_TYPE (Testcase, testcase, G_TYPE_OBJECT);

static void
testcase_init (Testcase* self)
{
	PRIV(self) = G_TYPE_INSTANCE_GET_PRIVATE (self,
						  TYPE_TESTCASE,
						  TestcasePrivate);

	PRIV(self)->gdk_pixmap   = gdk_pixmap_new (NULL,
						   100, 80,
						   24);
	PRIV(self)->cairo_pixmap = gdk_pixmap_new (NULL,
						   100, 80,
						   24);
}

static void
testcase_finalize (GObject* object)
{
	g_object_unref (PRIV(object)->gdk_pixmap);
	g_object_unref (PRIV(object)->cairo_pixmap);

	G_OBJECT_CLASS (testcase_parent_class)->finalize (object);
}

static void
testcase_class_init (TestcaseClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = testcase_finalize;

	g_type_class_add_private (self_class, sizeof (TestcasePrivate));
}

/* Public API Implementation */

Testcase*
testcase_new (void)
{
	return g_object_new (TYPE_TESTCASE,
			     NULL);
}

