/***************************************************************************
                          fastinfo.h  -  description
                             -------------------
    begin                : Sun 3 Nov 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#ifndef FASTINFO_H
#define FASTINFO_H

/**
 * this class shows the most important information
 * in a small widget in the PSE
 *@author Carsten Niehaus
 */

#include <qframe.h>

class QGridLayout;
class QLabel;
class Kalzium;

/**This is a small widget which shows the most importand data of an element
 *in the gap of the PSE.
 *@author Carsten Niehaus
 */

class Fastinfo : public QFrame{
    Q_OBJECT

    public: 
	Fastinfo(QWidget *parent, const char *name=0,Kalzium *Kalzium_tmp=0);
	QGridLayout *grid;
	void setInfo( int );

	Kalzium *kalzium;

	private:
	QLabel  *elemname,
			*weight,
			*number;
};

#endif
