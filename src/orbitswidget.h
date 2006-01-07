#ifndef ORBITSWIDGET_H
#define ORBITSWIDGET_H
/***************************************************************************

                           orbitswidget.h  -  description
                             -------------------
    begin                : June 2003
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


#include <qwidget.h>
#include <math.h>

/**
 * @short the widget which displays the Bohr-orbit of the element
 * @author Carsten Niehaus
 */
class OrbitsWidget : public QWidget
{
	Q_OBJECT

	public:
		OrbitsWidget( QWidget *parent=0 , const char *name =0 );

		void setElementNumber( const int num );

	private:
		/**
		 * the elementnumber we are looking at
		 */
		int Elemno; 
	
		/**
		 * the number off hulls the atom has
		 */
		int num;
		
		typedef QValueList<int> intList;
		intList numOfElectrons;

		const QString& getNumber() const;
		
		/**
		 * @return the delta of the x-coordinate
		 * @param r is the radius of the circle
		 * @param angle is the n'st circle out of num
		 * @param num is the number of circles
		 */
		inline double translateToDX( const double r , const double angle , const int num )
		{
			return( r * sin( M_PI * angle / num * 2 ) );
		}
		
		/**
		 * @return the delta of the y-coordinate
		 * @param r is the radius of the circle
		 * @param angle is the n'st circle out of num
		 * @param num is the number of circles
		 */
		inline double translateToDY( const double r , const double angle , const int num )
		{
			return( r * cos( M_PI * angle / num * 2 ) );
		}

		/**
		 * sets everything up
		 */
		void getNumberOfOrbits();

	protected slots:
		virtual void paintEvent(  QPaintEvent* );
};

#endif // ORBITSWIDGET_H

