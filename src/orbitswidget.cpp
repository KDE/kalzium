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

//QT-Includes
#include <qpainter.h>
#include <qregexp.h>

OrbitsWidget::OrbitsWidget( const int ElemNo , QWidget *parent, const char *name) : QWidget( parent, name )
{
	Elemno = ElemNo;
	
//	hulllist.append( "1 2 3" );
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
//	hulllist.append( "1 2 3  4 5" );
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
//	hulllist.append( "1 2 3  4  5 6" );
	//to be continued...
	
	getNumberOfOrbits();	
}

void OrbitsWidget::getNumberOfOrbits()
{
	numOfElectrons.clear();
	QRegExp rxb( "\\s" ); //space
	QString o = getNumber();

	num = 1;
	int pos = 0;
	int cut = 0;
	bool cont = true;

	if ( !o.contains( rxb ) ) //only true for H and He
		numOfElectrons.append( o.toInt() );
	else //every other element
	{
		while ( cont )
		{

			pos = o.find( rxb );
			cut = o.length()-pos-1;
			numOfElectrons.append(o.left( pos ).toInt());
			o = o.right( cut );
			num++;

			if ( !o.contains( rxb ) )
			{
				numOfElectrons.append( o.toInt() );
				cont = false;
			}
		}
	}
}

QString OrbitsWidget::getNumber()
{
	return *hulllist.at( Elemno-1 );
}

void OrbitsWidget::paintEvent(  QPaintEvent* )
{
	
	QPainter DC;
	DC.begin( this );

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
	
	int d = 2*r; //Diameter
	int	ddx = d/(2*num);//difference to the previous circle

	numOfElectrons.prepend( 999 );
	intList::Iterator it = numOfElectrons.end();
	it--;

	for ( int i = 0 ; i < num ; ++i )
	{
		DC.setBrush( Dense7Pattern  );

		int mx = w_c+ddx*i; //the x-coordinate for the current circle
		int my = h_c+ddx*i; //the y-coordinate for the current circle

		//draw the big ellipses in concentric circles
		DC.drawEllipse( mx , my , d , d);

		DC.setBrush( Qt::SolidPattern );
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
