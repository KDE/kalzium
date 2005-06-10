/***************************************************************************
 *   Copyright (C) 2005 by Martin Pfeiffer                                 *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "kalziumtip.h"
#include "element.h"

#include <qpixmap.h>
#include <qimage.h>
#include <qsimplerichtext.h>
#include <qpainter.h>
#include <kdialog.h>
#include <qapplication.h>
#include <kstandarddirs.h>
#include <kdebug.h>


KalziumTip::KalziumTip(QWidget* parent) : QWidget(parent) 
{
	setFocusPolicy(NoFocus); //the widget don't get the keyboard focus
	setBackgroundMode(NoBackground); // widget has no background
	resize(0,0);
	hide(); //initailly hide it

	connect(&m_frameTimer, SIGNAL(timeout()), SLOT(internalUpdate()));
}

void KalziumTip::showTip(QPoint mouse, Element* element)
{
	m_mousePointer = mouse;
	
	if( element == m_tippedElement )
		move(m_mousePointer); //do not paint again if already painted
	else
	{
		if ( element !=  0)
			m_tippedElement = element;

		loadIcon(); //load icon	
		display();
	}
}

void KalziumTip::paintEvent(QPaintEvent* e)
{
	if (m_dirty)
    	{
        	displayInternal();
        	m_dirty = false;
    	}

	QPainter p(this);
   	p.drawPixmap(e->rect().topLeft(), m_pixmap, e->rect());
}

void KalziumTip::display()
{
	if( !m_tippedElement )
		return;
	
	delete m_richText;
	
	QString elementname = m_tippedElement->elname();
	QString number = QString( "Number: %1" )
			.arg( QString::number(m_tippedElement->number()) );
	QString mass = QString( "Mass: %1" )
			.arg( QString::number(m_tippedElement->mass()) );				

	m_richText = new QSimpleRichText("<qt><h1>" + elementname + "</h1><p>"
						    + number + "</p><p>"
						    + mass  +"</p></qt>", font());


        m_richText->setWidth(400);

	m_maskEffect = isVisible() ? Plain : Dissolve;
    	m_dissolveSize = 24;
    	m_dissolveDelta = -1;

    	displayInternal();
	m_frameTimer.start(500/30);	

	move(m_mousePointer);
	show();
}

void KalziumTip::displayInternal()
{
    	if (!m_richText)
        	return;
    	

	// determine text rectangel sizes
	QRect textRect(0,0,0,0);
	textRect.setWidth(m_richText->widthUsed());
	textRect.setHeight(m_richText->height());

	//moves the rect
	textRect.moveBy(-textRect.left(), -textRect.top());
	// add the new coordinates
	textRect.addCoords(0,0,2,2);

	int margin = KDialog::marginHint();
	int height = QMAX(m_icon.height(), textRect.height()) + 2 * margin;
    	int textX = 2 + m_icon.width() + 2 * margin;
    	int width = textX + textRect.width() + margin;
    	int textY = (height - textRect.height()) / 2;

	//resize pixmap, mask and widget
	m_mask.resize(width, height);
	m_pixmap.resize(width, height);
	resize(width, height);

	// create and set transparency mask
    	switch(m_maskEffect)
    	{
        	case Plain:
            		plainMask();
            		break;

        	case Dissolve:
            		dissolveMask();
            		break;
    	}

	// draw background
    	QPainter bufferPainter(&m_pixmap);
    	bufferPainter.setPen(Qt::black);
    	bufferPainter.setBrush(backgroundColor());
    	bufferPainter.drawRoundRect(0, 0, width, height,
                                1600 / width, 1600 / height);

    	// draw icon if present
    	if (!m_icon.isNull())
    	{
        	bufferPainter.drawPixmap(margin,
                                 	margin,
                                 	m_icon, 0, 0,
                                 	m_icon.width(), m_icon.height());
    	}
	
	// draw text shadow
        QColorGroup cg = colorGroup();
        cg.setColor(QColorGroup::Text, cg.background().dark(115));
        int shadowOffset = QApplication::reverseLayout() ? -1 : 1;
        m_richText->draw(&bufferPainter, 5 + textX + shadowOffset, textY + 1, QRect(), cg);

        // draw text
        cg = colorGroup();
        m_richText->draw(&bufferPainter, 5 + textX, textY, rect(), cg);
}

void KalziumTip::dissolveMask()
{
    QPainter maskPainter(&m_mask);

    m_mask.fill(Qt::black);

    maskPainter.setBrush(Qt::white);
    maskPainter.setPen(Qt::white);
    maskPainter.drawRoundRect(m_mask.rect(), 1600 / m_mask.rect().width(),
                              1600 / m_mask.rect().height());

    m_dissolveSize += m_dissolveDelta;

    if (m_dissolveSize > 0)
    {
        maskPainter.setRasterOp(Qt::EraseROP);

        int x, y, s;
        const int size = 16;
	const int heightsize = size + height();

        for (y = 0; y < heightsize; y += size)
        {
            x = width();
            s = m_dissolveSize * x / 128;
            for (; x > -size; x -= size, s -= 2)
            {
                if (s < 0)
                {
                    s = 0;
                }
                maskPainter.drawEllipse(x - s / 2, y - s / 2, s, s);
            }
        }
    }
    else if (m_dissolveSize < 0)
    {
        m_frameTimer.stop();
        m_dissolveDelta = 1;
    }

    setMask(m_mask);
}

void KalziumTip::hide()
{
	m_frameTimer.stop();
    	m_tippedElement = 0;
    	QWidget::hide();
}

void KalziumTip::plainMask()
{
    	QPainter maskPainter(&m_mask);

    	m_mask.fill(Qt::black);

    	maskPainter.setBrush(Qt::white);
    	maskPainter.setPen(Qt::white);
    	maskPainter.drawRoundRect(m_mask.rect(), 1600 / m_mask.rect().width(),
                              1600 / m_mask.rect().height());
    	setMask(m_mask);
    	m_frameTimer.stop();
}

void KalziumTip::internalUpdate()
{
	m_dirty = true;
    	update();
}

void KalziumTip::loadIcon()
{
	if ( !locate(  "data" , "kalzium/elempics/" + m_tippedElement->symbol() + ".jpg" ).isEmpty() )
	{
		QPixmap pic ( locate( "data" , "kalzium/elempics/" + m_tippedElement->symbol() + ".jpg" ) );
		QImage img = pic.convertToImage();
		img = img.smoothScale ( 128, 128, QImage::ScaleMin );
		pic.convertFromImage( img );
		m_icon = pic;
	}
	else
	{
		QPixmap pic ( locate( "data" , "kalzium/hi128-app-kalzium.png" ) );
		m_icon = pic;
	}
}

#include "kalziumtip.moc"
