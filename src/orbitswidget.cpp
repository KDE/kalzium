/***************************************************************************
    begin                : Jun 23 2003
    copyright            : (C) 2003, 2004, 2005 by Carsten Niehaus
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

//QT-Includes
#include <QPainter>
#include <QRegExp>

#include <math.h>

static QStringList hulllist;

/**
 * @return the delta of the x-coordinate
 * @param r is the radius of the circle
 * @param angle is the n'st circle out of num
 * @param num is the number of circles
 */
static double translateToDX( double r, double angle, int num )
{
	return r * sin( M_PI * angle / num * 2 );
}

/**
 * @return the delta of the y-coordinate
 * @param r is the radius of the circle
 * @param angle is the n'st circle out of num
 * @param num is the number of circles
 */
static double translateToDY( double r, double angle, int num )
{
	return r * cos( M_PI * angle / num * 2 );
}

OrbitsWidget::OrbitsWidget( QWidget *parent ) : QWidget( parent )
{
	if ( hulllist.count() == 0 )
	{
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
		hulllist.append( "2 8 18 3" );
		hulllist.append( "2 8 18 4" );
		hulllist.append( "2 8 18 5" );
		hulllist.append( "2 8 18 6" );
		hulllist.append( "2 8 18 7" );
		hulllist.append( "2 8 18 8" );//Krypton
		hulllist.append( "2 8 18 8 1" );
		hulllist.append( "2 8 18 8 2" );//Rubidium
		hulllist.append( "2 8 18 9 2" );
		hulllist.append( "2 8 18 10 2" );//Zirconium
		hulllist.append( "2 8 18 12 1" );
		hulllist.append( "2 8 18 13 1" );
		hulllist.append( "2 8 18 14 1" );//Techneticum
		hulllist.append( "2 8 18 15 1" );
		hulllist.append( "2 8 18 16 1" );
		hulllist.append( "2 8 18 18" );  //Palladium
		hulllist.append( "2 8 18 18 1" );
		hulllist.append( "2 8 18 18 2" );
		hulllist.append( "2 8 18 18 3" );//Indium
		hulllist.append( "2 8 18 18 4" );
		hulllist.append( "2 8 18 18 5" );
		hulllist.append( "2 8 18 18 6" );
		hulllist.append( "2 8 18 18 7" );
		hulllist.append( "2 8 18 18 8" );//Xenon
		hulllist.append( "2 8 18 18 8 1" );//Caesium
		hulllist.append( "2 8 18 18 8 2" );//Barium
		hulllist.append( "2 8 18 18 9 2" );
		hulllist.append( "2 8 18 20 8 2" );//Cerium
		hulllist.append( "2 8 18 21 8 2" );
		hulllist.append( "2 8 18 22 8 2" );
		hulllist.append( "2 8 18 23 8 2" );
		hulllist.append( "2 8 18 24 8 2" );
		hulllist.append( "2 8 18 25 8 2" );
		hulllist.append( "2 8 18 25 9 2" );//Gadolinium
		hulllist.append( "2 8 18 27 8 2" );//Terbium
		hulllist.append( "2 8 18 28 8 2" );
		hulllist.append( "2 8 18 29 8 2" );
		hulllist.append( "2 8 18 30 8 2" );
		hulllist.append( "2 8 18 31 8 2" );
		hulllist.append( "2 8 18 32 8 2" );//Ytterbium
		hulllist.append( "2 8 18 32 9 2" );//Lutetium
		hulllist.append( "2 8 18 32 10 2" );//Hafnium
		hulllist.append( "2 8 18 32 11 2" );
		hulllist.append( "2 8 18 32 12 2" );
		hulllist.append( "2 8 18 32 13 2" );
		hulllist.append( "2 8 18 32 14 2" );
		hulllist.append( "2 8 18 32 15 2" );//Irdium
		hulllist.append( "2 8 18 32 17 1" );
		hulllist.append( "2 8 18 32 18 1" );
		hulllist.append( "2 8 18 32 18 2" );//Mercury
		hulllist.append( "2 8 18 32 18 3" );
		hulllist.append( "2 8 18 32 18 4" );
		hulllist.append( "2 8 18 32 18 5" );
		hulllist.append( "2 8 18 32 18 6" );
		hulllist.append( "2 8 18 32 18 7" );
		hulllist.append( "2 8 18 32 18 8" );//Radon
		hulllist.append( "2 8 18 32 18 8 1" );//Francium
		hulllist.append( "2 8 18 32 18 8 2" );//Radium
		hulllist.append( "2 8 18 32 18 9 2" );//Actinium
		hulllist.append( "2 8 18 32 20 8 2" );//Thorium
		hulllist.append( "2 8 18 32 21 8 2" );
		hulllist.append( "2 8 18 32 22 8 2" );//Uran
		hulllist.append( "2 8 18 32 23 8 2" );
		hulllist.append( "2 8 18 32 24 8 2" );//Plutonium
		hulllist.append( "2 8 18 32 25 8 2" );
		hulllist.append( "2 8 18 32 26 8 2" );//Cm
		hulllist.append( "2 8 18 32 27 8 2" );
		hulllist.append( "2 8 18 32 28 8 2" );//Cf
		hulllist.append( "2 8 18 32 29 8 2" );
		hulllist.append( "2 8 18 32 30 8 2" );//Fm
		hulllist.append( "2 8 18 32 31 8 2" );
		hulllist.append( "2 8 18 32 32 8 2" );//Nobelium
		hulllist.append( "2 8 18 32 32 9 2" );//Lawrencium	
		hulllist.append( "2 8 18 32 32 10 2" );	
		hulllist.append( "2 8 18 32 32 11 2" );//Dubnium (105)
		hulllist.append( "2 8 18 32 32 12 2" );	
		hulllist.append( "2 8 18 32 32 13 2" );//Bohrium
		hulllist.append( "2 8 18 32 32 14 2" );	
		hulllist.append( "2 8 18 32 32 15 2" );//Mt
		hulllist.append( "2 8 18 32 32 16 2" );//Darmstadtium
		hulllist.append( "2 8 18 32 32 17 2" );//Roentgenium
	}
}

void OrbitsWidget::setElementNumber( const int num )
{
	Elemno = num;
	
	getNumberOfOrbits();

	update();
}

void OrbitsWidget::getNumberOfOrbits()
{
	numOfElectrons.clear();
	QString o;
	if ( ( Elemno > 0 ) && ( Elemno <= hulllist.count() ) )
		o = hulllist[ Elemno-1 ];

	QRegExp digit( "\\d+" );
	int pos = 0;
	while ( ( pos = digit.indexIn( o, pos ) ) > -1 )
	{
		numOfElectrons.append( digit.cap( 0 ).toInt() );
		pos = pos + digit.matchedLength();
	}
}

void OrbitsWidget::paintEvent(  QPaintEvent* )
{
	QPainter DC;
	DC.begin( this );
	DC.setPen( Qt::black );

	const int num = numOfElectrons.count();
	if ( num == 0 ) return; // no orbits, do nothing
	
	int h=height();
	int w=width();
	int w_c=h/10;
	int h_c=h/10;
	
	//the radius of the current orbit
	int r;

	//make sure the biggest orbit fits in the widget
	if ( h < w )
		r = ( h-2*h_c )/2;
	else r = ( w-2*w_c )/2;
	
	//the radius of an 'electron'
	int r_electron = r/20;
	
	QBrush brush( Qt::yellow );

	int d = 2*r; //Diameter
	int	ddx = d/(2*num);//difference to the previous circle

	intList::Iterator it = numOfElectrons.end();
	--it;

	for ( int i = 0 ; i < num ; ++i )
	{
		int mx = w_c+ddx*i; //the x-coordinate for the current circle
		int my = h_c+ddx*i; //the y-coordinate for the current circle

		DC.setBrush( Qt::NoBrush );
		
		//draw the big ellipses in concentric circles
		DC.drawEllipse( mx , my , d , d);

		DC.setBrush( brush );
		
		for ( int e = 0 ; e < *it ; ++e )
		{
			int x = (int)translateToDX(  ( double )d/2 , ( double )e , *it);
			int y = (int)translateToDY(  ( double )d/2 , ( double )e , *it);

			DC.drawEllipse( x + mx + d/2 - r_electron, 
					y + mx + d/2 - r_electron,
					2*r_electron ,
					2*r_electron );
			
		}
		--it;
		d = d-2*ddx;
	}
}


#include "orbitswidget.moc"
