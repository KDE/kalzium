#ifndef ORBITSWIDGET_H
#define ORBITSWIDGET_H
/***************************************************************************

                           orbitswidget.h  -  description
                             -------------------
    begin                : June 2003
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


#include <qwidget.h>
#include <qstringlist.h>
#include <math.h>

class OrbitsWidget : public QWidget
{
	Q_OBJECT

	public:
		OrbitsWidget( const int Elemno , QWidget *parent=0 , const char *name =0 );

	private:
		/*
		 * the elementnumber we are looking at
		 */
		int Elemno; 
	
		int num; //the number of hulls the atom has
		
		typedef QValueList<int> intList;
		intList numOfElectrons;

		QStringList hulllist;

		const QString& getNumber() const;
		
		/*
		 * the returns the delta of the x-coordinate
		 * @param r is the radius of the circle
		 * @param angle is the n'st circle out of num
		 * @param num is the number of circles
		 */
		inline double translateToDX( const double r , const double angle , const int num )
		{
			return( r * sin( M_PI * angle / num * 2 ) );
		}
		
		/*
		 * the returns the delta of the y-coordinate
		 * @param r is the radius of the circle
		 * @param angle is the n'st circle out of num
		 * @param num is the number of circles
		 */
		inline double translateToDY( const double r , const double angle , const int num )
		{
			return( r * cos( M_PI * angle / num * 2 ) );
		}

		/*
		 * sets everything up
		 */
		void getNumberOfOrbits();

	protected slots:
		virtual void paintEvent(  QPaintEvent* );
};

#endif // ORBITSWIDGET_H

