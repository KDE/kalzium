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
	Elemno = ElemNo;
	
	hulllist.append( "1" );
	hulllist.append( "2" );     //Helium
	hulllist.append( "2 1" );
	hulllist.append( "2 2" );
	hulllist.append( "2 3" );
	hulllist.append( "2 4" );
	hulllist.append( "2 5" );
	hulllist.append( "2 6" );
	hulllist.append( "2 7" );
	hulllist.append( "2 8" );   //Neon
	hulllist.append( "2 8 1" );
	hulllist.append( "2 8 2" );
	hulllist.append( "2 8 3" );
	hulllist.append( "2 8 4" );
	hulllist.append( "2 8 5" );
	hulllist.append( "2 8 6" );
	hulllist.append( "2 8 7" );
	hulllist.append( "2 8 8" );  //Argon
	hulllist.append( "2 8 8 1" );
	hulllist.append( "2 8 8 2" );//Calcium
	hulllist.append( "2 8 9 2" );
	hulllist.append( "2 8 10 2" );
	hulllist.append( "2 8 11 2" );
	hulllist.append( "2 8 13 1" );
	hulllist.append( "2 8 13 2" );//Manganese
	hulllist.append( "2 8 14 2" );
	hulllist.append( "2 8 15 2" );
	hulllist.append( "2 8 16 2" );
	hulllist.append( "2 8 18 1" );//Copper
	hulllist.append( "2 8 18 2" );
	hulllist.append( "2 8 16 2 1" );
	hulllist.append( "2 8 16 2 2" );
	hulllist.append( "2 8 16 2 3" );
	hulllist.append( "2 8 16 2 4" );
	hulllist.append( "2 8 16 2 5" );
	hulllist.append( "2 8 16 2 6" );//Krypton
	//to be continued...
	
	getNumberOfOrbits();	
}

void OrbitsWidget::getNumberOfOrbits()
{
	QRegExp rxb( "\\d{1,2}" );
	QRegExp numx( "[0-9]" );
	int pos = 0;
	QString o = getNumber();
	kdDebug() << "Orbits sind: " << o << endl;
	
	num = 1;

	bool cont = true;
	
	while ( cont )
	{
		QString temp_ = o.left( o.find( rxb ) );
		
		kdDebug() << "Die pos ist: " << pos << endl;
		kdDebug() << "Das ding heisst: " << temp_ << endl;

		o = o.right( o.length() - o.find( rxb ) );

		kdDebug() << "Nun heißt o: " << o << endl;

//		numOfElectrons.append(o.left( pos ).toInt());

		num++;

		if ( num > 5 )//!o.contains( numx ) )
			cont = false;
	}
}

QString OrbitsWidget::getNumber()
{
	return *hulllist.at( Elemno+1 );
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

	kdDebug() << "Number of Orbits: " << num << endl;
	
	int d = 2*r; //Durchmesser
	int	ddx = d/(2*num);//Änderung zum Vorgänger

	int foo = 0;
	
//X 	for ( int i = 0 ; i < num ; ++i )
//X 	{
//X 		DC.setBrush( Dense7Pattern  );
//X 
//X 		int mx = w_c+ddx*i;
//X 		int my = h_c+ddx*i;
//X 
//X 		//draw the big ellipses in concentric circles
//X 		DC.drawEllipse( mx , my , d , d);
//X 
//X 		DC.setBrush( Qt::SolidPattern );
//X 		for ( int e = 0 ; e < 19; ++e )
//X 		{
//X 			kdDebug() << "run: " << i << " , " <<e << "Durchmesser: " << d/2 << endl;
//X 			int x = translateToDX(  ( double )d/2 , ( double )e );
//X 			int y = translateToDY(  ( double )d/2 , ( double )e );
//X 
//X 			kdDebug() << x << endl;
//X 			kdDebug() << y << endl;
//X 
//X 			DC.drawEllipse( x + mx + d/2 - r_electron, 
//X 					y + mx + d/2 - r_electron,
//X 					2*r_electron ,
//X 					2*r_electron );
//X 		}
//X 		d = d-2*ddx;
//X 	}
}



