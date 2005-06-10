#ifndef KALZIUMTIP_H
#define KALZIUMTIP_H
/***************************************************************************

                           The header for the tooltip class of Kalzium
                             -------------------
    begin                : June 2003
    copyright            : (C) 2003, 2004, 2005 by Martin Pfeiffer
    email                : hubipete@gmx.net
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
#include <qtimer.h>

class QPaintEvent;
class QSimpleRichText;
class QPixmap;
class Element;
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

		void showTip(QPoint, Element* element); //show the tip
		void hide(); // hide it

	protected:
		void paintEvent(QPaintEvent * e);

     		void plainMask();
    		void dissolveMask();
    	
		void displayInternal();
		void display();	
		
		void loadIcon(); //loads the elements icon
	
	protected slots:
		void internalUpdate();
		
	private:
		QBitmap m_mask;
		QPoint m_mousePointer;
    		QPixmap m_pixmap;
    		QPixmap m_icon; //icon shown on the tip

    		MaskEffect m_maskEffect; //inidicates to show mask effect or plain
    		QSimpleRichText* m_richText;

    		int m_dissolveSize;
    		int m_dissolveDelta;

    		QTimer m_frameTimer; //timer for the dissolve effect
    		bool m_dirty;	//indicates repainting the tooltip internal

		const Element* m_tippedElement; //the element the tip is about
};

#endif

