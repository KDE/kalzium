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
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	caml_startup(argv);

	char * eq = " a CH3(CH2)3COOH + b O2 -> c H2O + d CO2";
	char * eq2= " a CH3(CH2)3COOH + b O2 -> c H2O + d CO";
	char * result = solve_equation(eq);
	char * result2 = solve_equation(eq2);

	printf("solution : %s\n", result);

	printf("solution : %s\n", result2);
	free(result2);

	result2 = solve_equation(eq);
	printf("solution : %s\n", result2);
	
	free(result);
	free(result2);
	
	return 0;
}

