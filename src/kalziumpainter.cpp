/***************************************************************************
 *   Copyright (C) 2005      by Pino Toscano, toscano.pino@tiscali.it      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "kalziumpainter.h"

#include "kalziumdataobject.h"
#include "kalziumtabletype.h"
//#include "kalziumtabletypefactory.h"
#include "kalziumschemetype.h"
//#include "kalziumschemetypefactory.h"
#include "kalziumgradienttype.h"
//#include "kalziumgradienttypefactory.h"
#include "kalziumnumerationtype.h"
//#include "kalziumnumerationtypefactory.h"
#include "kalziumutils.h"
#include "element.h"
#include "prefs.h"

#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPair>
#include <QPalette>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QStringList>

#include <kdebug.h>
#include <kglobalsettings.h>
#include <kimageeffect.h>
#include <klocale.h>

KalziumPainter::KalziumPainter( KalziumTableType *ktt )
  : m_painter( 0 )
{
	m_ktt = ktt;

	setScheme( 1 );
	setGradient( 0 );
	setNumeration( 1 );

	m_year = 0;
	m_isTimeline = false;

	m_temperature = 0;

	m_legend = true;
	m_legendNeedRepaint = true;

	m_elementsNeedRepaint = true;

	m_mode = NORMAL;
}

void KalziumPainter::begin( QPaintDevice *pd )
{
	end();
	m_painter = new QPainter( pd );
}

void KalziumPainter::end()
{
	if ( m_painter )
	{
		if ( m_painter->isActive() )
			m_painter->end();
	}
}

KalziumTableType* KalziumPainter::currentTableType()
{
	return m_ktt;
}

void KalziumPainter::drawAll()
{
	drawElements();
	drawLegend( true );
	drawNumeration();
}

void KalziumPainter::drawElements()
{
	QPaintDevice *dev = m_painter->device();
	QRect r( 0, 0, dev->width(), dev->height() );
	m_painter->fillRect( r, QApplication::palette().background() );
	for ( int i = 1; i <= KalziumDataObject::instance()->numberOfElements(); i++ )
	{
		drawElement( i );
	}
}

void KalziumPainter::drawElement( int element )
{
	if ( !m_scheme || !m_ktt ) return;

	bool grayedOut = m_isTimeline ? m_year < KalziumDataObject::instance()->element( element )->dataAsVariant( ChemicalDataObject::date ).toDouble() : false;
	QRect rect = m_ktt->elementRect( element );
	Element *el = KalziumDataObject::instance()->element( element );
	const QString symbol = el->dataAsString( ChemicalDataObject::symbol );
//kdDebug() << "ELEMENT: " << element << "  RECT: " << rect << endl;

	switch ( m_mode )
	{
		case NORMAL:
		{
			QBrush c = grayedOut ? QBrush( Qt::lightGray ) : QBrush( m_scheme->elementGradient( element, rect ) );
			QColor textc = grayedOut ? Qt::white : m_scheme->textColor( element );
			m_painter->setPen( QBrush( textc ) );
	
			m_painter->fillRect( rect, c );
			m_painter->drawRect( rect );

			m_painter->drawText( rect, Qt::AlignCenter, symbol );
			break;
		}
		case SOM:
		{
			QColor color;

			const double melting = el->dataAsVariant( ChemicalDataObject::meltingpoint ).toDouble();
			const double boiling = el->dataAsVariant( ChemicalDataObject::boilingpoint ).toDouble();
			const double mass = el->dataAsVariant( ChemicalDataObject::mass ).toDouble();

			if ( m_temperature < melting )
			{
				//the element is solid
				color= Prefs::color_solid();
			}
			else if ( ( m_temperature > melting ) && ( m_temperature < boiling ) )
			{
				//the element is liquid
				color= Prefs::color_liquid();
			}
			else if ( ( m_temperature > boiling ) && ( boiling > 0.0 ) )
			{
				//the element is vaporous
				color= Prefs::color_vapor();
			}
			else
				color = Qt::lightGray;

			m_painter->setPen( Qt::black );

			QFont symbol_font = m_painter->font();
			symbol_font.setPointSize( 10 );
			QFont f = m_painter->font();
			f.setPointSize( 9 );
			m_painter->setFont( f );

			m_painter->fillRect( rect, QBrush( color ) );
			m_painter->drawRect( rect );

			m_painter->drawText( rect, Qt::AlignHCenter | Qt::AlignTop, QString::number( KalziumUtils::strippedValue( mass ) ) );

			m_painter->drawText( rect, Qt::AlignHCenter | Qt::AlignBottom, QString::number( element ) );

			m_painter->setFont( symbol_font );
			m_painter->drawText( rect, Qt::AlignCenter, symbol );

			break;
		}
		case GRADIENT:
		{
			m_painter->setPen( QBrush( Qt::black ) );
			QBrush c = grayedOut ? QBrush( Qt::lightGray ) : QBrush( m_gradient->calculateColor( m_gradient->elementCoeff( element ) ) );

			m_painter->fillRect( rect, c );
			m_painter->drawRect( rect );

			m_painter->drawText( rect, Qt::AlignCenter, symbol );
			break;
		}
	}

}

void KalziumPainter::drawLegend( bool force )
{
	// repaint only if the legend changed or we what to to it explicitely
	if ( !( m_legend && m_legendNeedRepaint || force ) ) return;

	switch ( m_mode )
	{
		case NORMAL:
		case SOM:
		{
			// settings font
			QFont legendFont = KGlobalSettings::generalFont();
			legendFont.setPointSize( legendFont.pointSize() + 1 );
			m_painter->setFont( legendFont );

			int padding = 6;
			QRect legendRect = m_ktt->legendRect();
			m_painter->fillRect( legendRect, QBrush( Qt::lightGray ) );
			int itemheight = ( legendRect.height() - 6 * padding ) / 5;
			int squareside = itemheight - 2;

			legendList items;
			if ( m_mode == SOM )
			{
				items << qMakePair( i18n( "Solid" ), Prefs::color_solid() );
				items << qMakePair( i18n( "Liquid" ), Prefs::color_liquid() );
				items << qMakePair( i18n( "Vaporous" ), Prefs::color_vapor() );
				items << qMakePair( i18n( "Unknown" ), QColor( Qt::lightGray ) );
			}
			else
			{
				items = m_scheme->legendItems();
				if ( m_isTimeline )
					items << qMakePair( i18n( "Not discovered yet" ), QColor( Qt::lightGray ) );
			}

			// we allow max 10 items in the legend
			int numitems = qMin( items.count(), 10 );
			int itemwidth = legendRect.width() - 2 * padding;
			if ( numitems > 5 )
				itemwidth = ( itemwidth - padding ) / 2;

			for ( int i = 0; i < numitems; i++ )
			{
				int x = legendRect.left() + padding + ( i > 4 ? itemwidth + padding : 0 );
				int y = legendRect.top() + padding + ( padding + itemheight ) * ( i % 5 );
				QRect sq( x + 1, y + 1, squareside, squareside );
				m_painter->fillRect( sq, QBrush( items.at(i).second ) );
				m_painter->drawRect( sq );
				QRect text( x + squareside + 5, y, itemwidth - squareside - 5, itemheight);
				m_painter->drawText( text, Qt::AlignLeft | Qt::AlignVCenter, items.at(i).first );
			}

			break;
		}
		case GRADIENT:
		{
			// settings font
			QFont legendFont = KGlobalSettings::generalFont();
			legendFont.setPointSize( legendFont.pointSize() + 1 );
			m_painter->setFont( legendFont );

			int padding = 6;
			int sidepadding = 12;
			QRect legendRect = m_ktt->legendRect();
			m_painter->fillRect( legendRect, QBrush( Qt::lightGray ) );

			QRect text = legendRect;
			text.setHeight( text.height() / 2 - 2 * padding );
			text.setWidth( text.width() - 2 * sidepadding );
			text.translate( sidepadding, padding );

			QSize imgsize( legendRect.width() - 2 * sidepadding, 20 );
			QImage img = KImageEffect::gradient( imgsize, m_gradient->firstColor(), m_gradient->secondColor(), KImageEffect::HorizontalGradient );

			QRect othertexts = text;
			othertexts.moveTo( text.bottomLeft() + QPoint( 0, padding + 4 + imgsize.height() ) );

			m_painter->drawText( text, Qt::AlignHCenter | Qt::AlignBottom, i18n( "Gradient: %1" ).arg( m_gradient->description() ) );
			m_painter->drawPixmap( text.bottomLeft() + QPoint( 0, padding ), QPixmap( img ) );

			m_painter->drawText( othertexts, Qt::AlignRight, QString::number( m_gradient->maxValue() ) );
			m_painter->drawText( othertexts, Qt::AlignLeft, QString::number( m_gradient->minValue() ) );

			break;
		}
	}
	m_legendNeedRepaint = false;
}

void KalziumPainter::drawNumeration()
{
	QStringList numitems = m_numeration->items();
	for ( int i = 0; i < numitems.count(); i++ )
	{
		QRect itemrect = m_ktt->numerationRect( i, m_numeration );
		if ( itemrect.isNull() ) continue;

		m_painter->drawText( itemrect, Qt::AlignCenter, numitems.at( i ) );
	}
}

void KalziumPainter::drawElementSelector( int element )
{
	if ( !m_ktt ) return;

	QRect elemrect = m_ktt->elementRect( element );

	QPen pen;

	// outer circle
	pen.setStyle( Qt::DotLine );
	pen.setWidth( 4 );
	pen.setColor( Qt::blue );
	m_painter->setPen( pen );
	m_painter->drawEllipse( elemrect.left() - 10, elemrect.top() - 10, elemrect.width() + 20, elemrect.height() + 20 );

	// inner circle
	pen.setWidth( 3 );
	pen.setColor( Qt::red );
	m_painter->setPen( pen );
	m_painter->drawEllipse( elemrect.left() - 5, elemrect.top() - 5, elemrect.width() + 10, elemrect.height() + 10 );
}

void KalziumPainter::toggleLegend( bool active )
{
	if ( active == m_legend ) return;

	m_legend = active;
	m_legendNeedRepaint = true;
}

bool KalziumPainter::legendShown() const
{
	return m_legend;
}

void KalziumPainter::setMode( MODE m )
{
	m_mode = m;
}

KalziumPainter::MODE KalziumPainter::mode() const
{
	return m_mode;
}

void KalziumPainter::setScheme( int s )
{
	KalziumSchemeType *tmp = KalziumSchemeTypeFactory::instance()->build( s );
	if ( tmp )
	{
		m_scheme = tmp;
	}
}

KalziumSchemeType* KalziumPainter::scheme() const
{
	return m_scheme;
}

void KalziumPainter::setGradient( int g )
{
	KalziumGradientType *tmp = KalziumGradientTypeFactory::instance()->build( g );
	if ( tmp )
	{
		m_gradient = tmp;
	}
}

KalziumGradientType* KalziumPainter::gradient() const
{
	return m_gradient;
}

void KalziumPainter::setNumeration( int n )
{
	KalziumNumerationType *tmp = KalziumNumerationTypeFactory::instance()->build( n );
	if ( tmp )
	{
		m_numeration = tmp;
	}
}

KalziumNumerationType* KalziumPainter::numeration() const
{
	return m_numeration;
}

void KalziumPainter::setDate( int year )
{
	m_year = year;
}

int KalziumPainter::date() const
{
	return m_year;
}

void KalziumPainter::setTimeline( bool timeline )
{
	m_isTimeline = timeline;
}

bool KalziumPainter::timeline() const
{
	return m_isTimeline;
}

void KalziumPainter::setTemperature( int temp )
{
	m_temperature = temp;
}

int KalziumPainter::temperature() const
{
	return m_temperature;
}
