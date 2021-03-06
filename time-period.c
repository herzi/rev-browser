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

#include "time-period.h"

guint
time_period_get_difference (Date      * start,
			    Date      * end,
			    TimePeriod  stepping)
{
	guint result = 0;

	g_return_val_if_fail (IS_DATE (start), 0);
	g_return_val_if_fail (IS_DATE (end), 0);
	// FIXME: g_return_val_if_fail (start <= end, 0);

	result = date_get_year (end) - date_get_year (start);

	if (stepping == TIME_PERIOD_YEAR) {
		return result;
	}

	result *= 12;
	result += date_get_month (end) - date_get_month (start);

	if (stepping != TIME_PERIOD_MONTH) {
		g_warning ("%s(): git invalid stepping value %d",
			   G_STRFUNC, stepping);
		return 0;
	}

	return result;
}

