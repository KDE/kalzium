/***************************************************************************
                          eleminfo.cpp  -  description
                             -------------------
    begin                : Sat Dec 8 2001
    copyright            : (C) 2001, 2002 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <eleminfo.h>

//C++-Includes
#include "stdlib.h"
#include "time.h"

void position(int n, int& h, int& v)
{
    //Hydrogen
    if (n == 1)
    {
	h=0; v=0;
    }


    //Helium
    if (n == 2)
    {
	h=680; v=0;
    }


    //Lithium
    if (n == 3)
    {
	h=0; v=40;
    }


    //Beryllium
    if (n == 4)
    {
	h=40; v=40;
    }


    //Boron-->Neon or Aluminum --> Argon
    if ((n >= 5 && n <= 10) || (n >= 13 && n <= 18))
	for (int i = 1; i <= (6-(10-n)); i++)
	{
	    h=((i*40)+440);
	    v = 40;
	    if (n >= 13)
	    {
	    	v=80;
    		h=(h-320);
	    }
	}


    //Sodium
    if (n == 11)
    {
	h=0; v=80;
    }


    //Magnesium
    if (n == 12)
    {
	h=40; v=80;
    }


    //Potassium --> Uuo without La and Ac
    if ((n >= 19 && n <= 56) || (n >= 72 && n <= 88) || n >= 104)
	for (int i = 1; i <= 18; i++)
	{
	    int f = n;
	    if (n > 18)
		    f = n-18;
	    if (n > 36)
		    f = n-36;
	    if (n > 54)
		    f = n-54;
	    if (n > 71)
		    f = n-68;
	    if (n > 86)
		    f = n-86;
	    if (n > 103)
		    f = n-100;

	    h=((f*40)-40);

	    v = 120;
	    if (n >= 37)
		    v=160;
	    if (n >= 55)
		    v=200;
	    if (n >= 87)
		    v=240;
	}


    //Lanthanum --> Lutetium and Actinum --> Lawrencium
    if ((n >= 57 && n <= 71) || (n >= 89 && n <= 103))
	for (int i = 1; i <= 14; i++)
	{
	    int f = n;
	    if (n > 56)
		    f = n-55;
	    if (n > 88)
		    f = n-87;
	    h=(f*40);
	    v = 290;
	    if (n >= 89)
		    v=330;
	}
    v += 10;
}

void periodNrpos(int i, int& h, int& v)
{
    if ( i == 1 )
    {
	h = 10; v = 25;
    }
    if ( i == 2 )
    {
	h = 50; v = 65;
    }
    if ( i > 2 && i < 13 )
    {
	h = (40*i)-30; v = 145;
    }
    if ( i > 12 && i < 18 )
    {
	h = (40*i)-30; v = 65;
    }
    if ( i == 18)
    {
	h = 690; v = 25;
    }
}

void periodName(int n, QString& name)
{
    if (n == 1)
	    name = "IA";
    if (n == 2)
	    name = "IIA";
    if (n == 3)
	    name = "IIIB";
    if (n == 4)
	    name = "IVB";
    if (n == 5)
	    name = "VB";
    if (n == 6)
	    name = "VIB";
    if (n == 7)
	    name = "VIIB";
    if (n == 8)
	    name = "VIII";
    if (n == 9)
	    name = "VIII";
    if (n == 10)
	    name = "VIII";
    if (n == 11)
	    name = "IB";
    if (n == 12)
	    name = "IIB";
    if (n == 13)
	    name = "IIIA";
    if (n == 14)
	    name = "IVA";
    if (n == 15)
	    name = "VA";
    if (n == 16)
	    name = "VIA";
    if (n == 17)
	    name = "VIIA";
    if (n == 18)
	    name = "VIIIA";
}

void periodOldName(int n, QString& name)
{
    if (n == 1)
	    name = "1A";
    if (n == 2)
	    name = "2A";
    if (n == 3)
	    name = "3A";
    if (n == 4)
	    name = "4A";
    if (n == 5)
	    name = "5A";
    if (n == 6)
	    name = "6A";
    if (n == 7)
	    name = "7A";
    if (n == 8)
	    name = "8";
    if (n == 9)
	    name = "8";
    if (n == 10)
	    name = "8";
    if (n == 11)
	    name = "1B";
    if (n == 12)
	    name = "2B";
    if (n == 13)
	    name = "3B";
    if (n == 14)
	    name = "4B";
    if (n == 15)
	    name = "5B";
    if (n == 16)
	    name = "6B";
    if (n == 17)
	    name = "7B";
    if (n == 18)
	    name = "0";
}

void shuffle(int& a, int& b, int& c)
{
    srandom( time( 0 ) );
     a = rand()%3;
    do
    {
        b = rand()%3;
    } while ( b == a);
    do
    {
        c = rand()%3;
    } while ( c != b && c != a);
}
