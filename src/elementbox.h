#ifndef ELEMENTBOX_H
#define ELEMENTBOX_H
/***************************************************************************
 *   Copyright (C) 2003 by Martin Pfeiffer                                 *
 *   hubipete@gmx.net                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include <qwidget.h>
#include <qtimer.h>

class Element;
class QSimpleRichText;
class QString;

/**
 * @author Martin Pfeiffer
 */
class ElementBox : public QWidget
{
	Q_OBJECT

	public:
		ElementBox( QWidget *parent, const char* name = 0 );
		~ElementBox();

		void setElement( Element* element ){
			m_element = element; 
		}

		Element* element(){
			return m_element;
		}

		void setColor( QColor color ){
			m_color = color;
		}

		void setUnderline( QString& underline ){
			m_underline = underline;
		}

	protected slots:
		virtual void paintEvent( QPaintEvent *e );
                virtual void mouseMoveEvent( QMouseEvent *e );
                virtual void mousePressEvent( QMouseEvent *e );
		virtual void focusInEvent( QFocusEvent* e );
		virtual void focusOutEvent( QFocusEvent* e );
	
	signals:
		void mouseMove( int );	
		void mousePress( int );

	private:
		Element* m_element;
		
		QColor m_color;	//the element's box current color

		QSimpleRichText* m_richText;
		QString m_underline;

		QTimer hoverTimer, mouseOverTimer;

		void setCoordinates();
		void setRowCol();
		int m_col;
		int m_row;
		int m_dynVertBorder;
		int m_dynHoriBorder;

		int m_rowCount;
		int m_colCount;
		int m_border;
		int m_lanthanoidDist;
};

#endif

