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

#ifndef TIME_PERIOD_H
#define TIME_PERIOD_H

#include "date.h"

G_BEGIN_DECLS

typedef enum {
	TIME_PERIOD_YEAR,
	TIME_PERIOD_MONTH
} TimePeriod;

guint time_period_get_difference (Date      * start,
				  Date      * end,
				  TimePeriod  stepping);

G_END_DECLS

#endif /* !TIME_PERIOD_H */
