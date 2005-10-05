/***************************************************************************
 *   Copyright (C) 2004 by Thomas Nagy                                     *
 *   tnagy2^8@yahoo.fr                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>

#include <caml/mlvalues.h>
#include <caml/callback.h>

char* solve_equation(const char * eq)
{
	static value * solve_equation_closure = NULL;
	if (solve_equation_closure == NULL)
	{
		solve_equation_closure = caml_named_value("solve_equation");
	}

	return strdup(String_val(callback(*solve_equation_closure, copy_string(eq)) ));
}

