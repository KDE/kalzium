#ifndef ORBITSWIDGET_H
#define ORBITSWIDGET_H
/***************************************************************************

                           KalziumGraph.h  -  description
                             -------------------
    begin                : Wed Oct 23 2002
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


#include <qvaluelist.h>
#include <qwidget.h>
#include <math.h>

class OrbitsWidget : public QWidget
{
	Q_OBJECT

	public:
		OrbitsWidget( int Elemno , QWidget *parent=0 , const char *name =0 );

	private:
		typedef QValueList<int> intList;
		intList numOfElectrons;

		int getNumber( QString );
		
		QString orbits;

		inline double translateToDX( const double r , const double angle )
		{
			return( r * sin( M_PI_4 * angle ) );
		}
		
		inline double translateToDY( const double r , const double angle )
		{
			return( r * cos(  M_PI_4 * angle ) );
		}

		/*
		 * returns the number of orbits which has to be drawn
		 */
		int getNumberOfOrbits();

	protected slots:
		virtual void paintEvent(  QPaintEvent* );
};

#endif // ORBITSWIDGET_H

