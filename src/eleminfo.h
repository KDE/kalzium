/***************************************************************************
                          eleminfo.h  -  description
                             -------------------
    begin                : Mon Dec 10 2001
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

#include <qstring.h>

void position(int, int&, int&);

void periodNrpos(int, int&, int&);

void periodName(int, QString&);

void shuffle(int&, int&, int&);
