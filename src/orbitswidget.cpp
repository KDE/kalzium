/***************************************************************************

                          orbitswidget.cpp  -  description
                             -------------------
    begin                : Jun 23 2003
    copyright            : (C) 2003 by Carsten Niehaus
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

#include "orbitswidget.h"

//KDE-Includes
#include <kstandarddirs.h>
#include <ksimpleconfig.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <kconfig.h>
#include <knuminput.h>

//QT-Includes
#include <qlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qlistview.h>
#include <qregexp.h>

OrbitsWidget::OrbitsWidget( QString Orbits , QWidget *parent, const char *name) : QWidget( parent, name )
{
	orbits = Orbits;
}

int OrbitsWidget::getNumberOfOrbits()
{
	kdDebug() << orbits << endl;
	QRegExp rxb( "([a-z])([0-9]+)" );
	QRegExp rxm( "([a-z]{2}) ",false );
	int pos = 0;
	int count = 0;
	while (  ( pos = rxb.search( orbits, pos ) ) != -1 ) {
		count++;
		pos += rxb.matchedLength();
		kdDebug() << pos << " , " << count << endl;
	}

	return count;
}

void OrbitsWidget::paintEvent(  QPaintEvent* )
{
	QPainter DC;

	int h=this->height();
	int w=this->width();
	int w_c=h/10;
	int h_c=h/10;
	
	//the radius of the current orbit
	int r;

	//the radius of an 'electron'
	int r_electron;

	//make sure the biggest orbit fits in the widget
	if ( h < w )
		r = ( h-2*h_c )/2;
	else r = ( w-2*w_c )/2;
	
	r_electron = r/10;
	
	DC.begin( this );
	DC.drawRect( w_c , h_c , 2*r, 2*r );

	DC.drawEllipse( w_c , h_c , 2*r, 2*r );

	int num = getNumberOfOrbits();
	kdDebug() << "Nummer der Orbits: " << num << endl;

		//Draws an ellipse with center at ( x + w/2, y + h/2 ) and size ( w, h ). 
}


