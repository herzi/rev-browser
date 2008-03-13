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

#ifndef TESTCASE_H
#define TESTCASE_H

#include <gdk/gdk.h>

G_BEGIN_DECLS

typedef struct _Testcase        Testcase;
typedef struct _TestcasePrivate TestcasePrivate;
typedef struct _TestcaseClass   TestcaseClass;

#define TYPE_TESTCASE         (testcase_get_type ())
#define TESTCASE(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), TYPE_TESTCASE, Testcase))
#define TESTCASE_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_TESTCASE, TestcaseClass))
#define IS_TESTCASE(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), TYPE_TESTCASE))
#define IS_TESTCASE_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), TYPE_TESTCASE))
#define TESTCASE_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), TYPE_TESTCASE, TestcaseClass))

GType      testcase_get_type         (void);
Testcase*  testcase_new              (void);
GdkPixmap* testcase_get_pixmap_gdk   (Testcase const* self);
GdkPixmap* testcase_get_pixmap_cairo (Testcase const* self);

struct _Testcase {
	GObject          base_instance;
	TestcasePrivate* _private;
};

struct _TestcaseClass {
	GObjectClass     base_class;
};

G_END_DECLS

#endif /* !TESTCASE_H */
