/***************************************************************************
                          kalzium_i18n.h  -  description
                             -------------------
    begin                : Die Jan  5 17:59:34 CET 2001
    copyright            : (C) 2002 by Carsten Niehaus
    email                : cniehaus@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KALZIUM_I18N_H
#define KALZIUM_I18N_H


/**
* This is a dummy class whose sole purpose is to provide strings for translators.
* These strings are read from data files, but they must be present in a source file
* or they won't be included in the template translation file.  It is redundant to define
* these strings twice; perhaps we should just get rid of the data files and define the
**/

class Kalzium_i18n {
public: 
	Kalzium_i18n();
	~Kalzium_i18n();
	void dummyFunction(void);
};

#endif
