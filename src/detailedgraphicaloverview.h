#ifndef DETAILEDGRAPHICALOVERVIEW_H
#define DETAILEDGRAPHICALOVERVIEW_H
/***************************************************************************
    copyright            : (C) 2004 by Carsten Niehaus
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

class Element;


/**
 * @short The widget which displays the most important information
 * in one widget like a lot people know it from school
 * @author Carsten Niehaus
 */
class DetailedGraphicalOverview : public QWidget
{
	Q_OBJECT

	public:
		DetailedGraphicalOverview( Element *el, QWidget *parent, const char *name=0 );
		DetailedGraphicalOverview( QWidget *parent, const char *name=0 );

		/**
		 * sets the elements whose data will be used to @p el
		 */
		void setElement( Element* el ){
			m_element = el;
			update();
		}

		/**
		 * draw the small symbol which symbolises the
		 * relevence for humans
		 */
		void drawBiologicalSymbol( QPainter *p );
	
		//calculation of the corners
		int x1,x2,y1,y2,h_t;

	private:
		/**
		 * the element whose data will be used
		 */
		Element *m_element;

	protected:
		virtual void paintEvent( QPaintEvent* );

		/**
		 * initialization method
		 */
		virtual void init( Element* );
};

#endif // DETAILEDGRAPHICALOVERVIEW_H

