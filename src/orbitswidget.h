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
#include <qstringlist.h>
#include <qwidget.h>
#include <math.h>

class OrbitsWidget : public QWidget
{
	Q_OBJECT

	public:
		OrbitsWidget( int Elemno , QWidget *parent=0 , const char *name =0 );

	private:
		/*
		 * the elementnumber we are looking at
		 */
		int Elemno; 
	
		int num; //the number of hulls the atom has
		
		typedef QValueList<int> intList;
		intList numOfElectrons;

		QStringList hulllist;

		QString getNumber();
		
		inline double translateToDX( const double r , const double angle )
		{
			return( r * sin( M_PI_4 * angle / 3) );
		}
		
		inline double translateToDY( const double r , const double angle )
		{
			return( r * cos( M_PI_4 * angle / 3) );
		}

		/*
		 * sets everything up
		 */
		void getNumberOfOrbits();

	protected slots:
		virtual void paintEvent(  QPaintEvent* );
};

#endif // ORBITSWIDGET_H

