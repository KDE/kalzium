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
#include <qlabel.h>
#include <qpainter.h>
#include <qregexp.h>
#include <qvaluelist.h>

OrbitsWidget::OrbitsWidget( int ElemNo , QWidget *parent, const char *name) : QWidget( parent, name )
{
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));
	config.setGroup(QString::number(ElemNo+1));
	orbits=config.readEntry("Orbits","0");
}

int OrbitsWidget::getNumberOfOrbits()
{
//X 	kdDebug() << orbits << endl;
	QRegExp rxb( "([a-z])([0-9]+)" );
	QRegExp rxm( "([a-z]{2}) ",false );
	QString numEl = orbits;
	int pos = 0;
	int count = 0;
	while (  ( pos = rxb.search( orbits, pos ) ) != -1 ) {
		count++;
		pos += rxb.matchedLength();
		numEl=numEl.left( pos-1 );
//		kdDebug() << "Current: " << numEl << endl;
		numOfElectrons.append( getNumber( numEl ) );
	}

	return count;
}

int OrbitsWidget::getNumber( QString /*cut*/ )
{
	return 3;
}

void OrbitsWidget::paintEvent(  QPaintEvent* )
{
	
	QPainter DC;
	DC.begin( this );

	DC.setBrush( Dense7Pattern  );
	DC.setBrush( Qt::darkRed );
	
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
	
	r_electron = r/20; //diameter of an electron-circle
	
	DC.drawRect( w_c , h_c , 2*r, 2*r ); //only to see the borders, will be removed

	int num = getNumberOfOrbits();
	kdDebug() << "Number of Orbits: " << num << endl;
	
	int d = 2*r; //Durchmesser
	int	ddx = d/(2*num);//Änderung zum Vorgänger

	for ( int i = 0 ; i < num ; ++i )
	{
		
		DC.setBrush( Dense7Pattern  );
//X 		kdDebug() << "Differenz: " << ddx << endl;
//X 		kdDebug() << "Durchmesser: " << d << endl;

		int mx = w_c+ddx*i;
		int my = h_c+ddx*i;

		//draw the big ellipses in concentric circles
		DC.drawEllipse( mx , my , d , d);

		DC.setBrush( Qt::SolidPattern );
		for ( int e = 0 ; e < 4 ; ++e )
		{
			kdDebug() << "Durchlauf: " << i << " , " <<e << "Durchmesser: " << d/2 << endl;
			int x = translateToDX(  ( double )d/2 , ( double )e );
			int y = translateToDY(  ( double )d/2 , ( double )e );
			
			kdDebug() << x << endl;
			kdDebug() << y << endl;
			DC.drawEllipse( x + mx + d/2 - r_electron, 
							x + mx + d/2 - r_electron,
							2*r_electron ,
							2*r_electron );
			
			
		}
		d = d-2*ddx;
	}
}



