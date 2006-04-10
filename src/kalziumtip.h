#ifndef KALZIUMTIP_H
#define KALZIUMTIP_H
/***************************************************************************

                           The header for the tooltip class of Kalzium
                             -------------------
    begin                : June 2003
    copyright            : (C) 2003, 2004, 2005 by Zack Rusin <zrusin@kde.org>
						Sami Kyostil <skyostil@kempele.fi>
                   				Aaron J. Seigo <aseigo@kde.org>
						Martin Pfeiffer <hubipete@gmx.net>
   
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

		KalziumTip( QWidget * parent = 0, const char * name = 0, WFlags f = 0 );

		void showTip( QPoint, Element* element, int visibleWidth, int visibleHeight ); //show the tip
		void hide(); // hide it

	protected:
		void paintEvent(QPaintEvent * e);

     		void plainMask();
    		void dissolveMask();
    	// if the mouse pointer hovers us, send the event to the parent
    	// so we are some-what 'mouse transparent'
    	void mouseMoveEvent(QMouseEvent * e);
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
		QPixmap m_noElemIcon; //icon for element wich have no image

    		MaskEffect m_maskEffect; //inidicates to show mask effect or plain
    		QSimpleRichText* m_richText;

    		int m_dissolveSize;
    		int m_dissolveDelta;

    		QTimer m_frameTimer; //timer for the dissolve effect
    		bool m_dirty;	//indicates repainting the tooltip internal

		const Element* m_tippedElement; //the element the tip is about
};

#endif

