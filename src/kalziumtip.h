#ifndef KALZIUMTIP_H
#define KALZIUMTIP_H
/***************************************************************************

                           kalziumtip.h  -  description
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
#include <qbitmap.h>
#include <qpixmap.h>
#include <qtimer.h>

#include "element.h"

class QPaintEvent;
class QSimpleRichText;
class QTimer;

/**
 * @short tooltip for kalzium
 * @author Martin Pfeiffer
 */

class KalziumTip : public QWidget
{
	Q_OBJECT	

	public:
		enum MaskEffect { Plain, Dissolve };

		KalziumTip(QWidget * parent);

		void showTip(QPoint, Element* element);
		void hide();

	protected:
		void paintEvent(QPaintEvent * e);
    		//void mousePressEvent(QMouseEvent * e);

    		void plainMask();
    		void dissolveMask();
    	
		void displayInternal();
		void display();	

	protected slots:
		void internalUpdate();
		
	private:
		QBitmap m_mask;
		QPoint m_mousePointer;
    		QPixmap m_pixmap;
    		QPixmap m_icon;
    		MaskEffect m_maskEffect; //inidicates to show mask effect or plain
    		QSimpleRichText* m_richText;

    		int m_dissolveSize;
    		int m_dissolveDelta;

    		QTimer m_frameTimer;
    		bool m_dirty;	//indicates repainting the tooltip internal

		const Element* m_tippedElement; //the element the tip is about
};


#endif

    		
    		
            		
            		
            		
            		
            		
            		
    		


