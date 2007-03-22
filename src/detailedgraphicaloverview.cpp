/***************************************************************************

copyright            : (C) 2004, 2005 by Carsten Niehaus
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
#include "detailedgraphicaloverview.h"
#include "element.h"
#include "kalziumutils.h"

//KDE-Includes
#include <kdebug.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include <kglobal.h>

//QT-Includes
#include <qpainter.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qrect.h>

DetailedGraphicalOverview::DetailedGraphicalOverview( Element *el, QWidget *parent, const char *name ) 
: QWidget( parent, name )
{
	init( el );
}

DetailedGraphicalOverview::DetailedGraphicalOverview( QWidget *parent, const char *name ) 
: QWidget( parent, name )
{
	init( 0L );
}

void DetailedGraphicalOverview::init( Element *el )
{
	setBackgroundMode( NoBackground );

	m_element = el;
	setMinimumSize( 300, 200 );
	update();
}

void DetailedGraphicalOverview::paintEvent( QPaintEvent* )
{
	int h = height();
	int w = width();

	QPixmap pm( w, h );

	QPainter p;
	p.begin( &pm );

	p.setBrush(Qt::SolidPattern);

	if ( !m_element )
	{
		pm.fill( paletteBackgroundColor() );
		p.drawText( 0, 0, w, h, Qt::AlignCenter | Qt::WordBreak, i18n( "No element selected" ) );
	}
	else
	{

		h_t = 20; //height of the texts

		x1 =  0;
		y1 =  0;

		x2 = w;
		y2 = h;

		p.setBrush( m_element->elementColor() );
		p.drawRect( x1 , y1 , x2 , y2 );

		p.setBrush( Qt::black );
		p.setBrush(Qt::NoBrush);

		QFont fA = KGlobalSettings::generalFont();
		QFont fB = KGlobalSettings::generalFont();
		QFont fC = KGlobalSettings::generalFont();

		QString strLocalizedMass = KalziumUtils::localizedValue( m_element->mass(), 6 );

		fA.setPointSize( fA.pointSize() + 20 ); //Huge font
		fA.setBold( true );
		fB.setPointSize( fB.pointSize() + 6 ); //Big font
		fC.setPointSize( fC.pointSize() + 4 ); //Big font
		fC.setBold( true );
		QFontMetrics fmA = QFontMetrics( fA );
		QFontMetrics fmB = QFontMetrics( fB );

		//coordinates for element symbol: near the center
		int xA = 4 * w / 10;
		int yA = h / 2;

		//coordinates for the atomic number: offset from element symbol to the upper left
		int xB = xA - fmB.width( QString::number( m_element->number() ) );
		int yB = yA - fmA.height() + fmB.height();

		//Element Symbol
		p.setFont( fA );
		p.drawText( xA, yA , m_element->symbol() ); 

		//Atomic number
		p.setFont( fB );
		p.drawText( xB, yB, QString::number( m_element->number() ));

		QRect rect( 0, 20, w/2, h );
		
		p.setFont( fC );
				
		int size = KalziumUtils::maxSize(m_element->elname(), rect , fC, &p);
		int size2 = KalziumUtils::maxSize(m_element->oxstage(), rect, fC, &p);
		int size3 = KalziumUtils::maxSize( strLocalizedMass , rect , fC, &p);

		//Name and other data
		fC.setPointSize( size );
		p.setFont( fC );
		
		//Name
		p.drawText( 1, 0, w/2, h, Qt::AlignLeft, m_element->elname() );
		
		//Oxidationstates
		fC.setPointSize( size2 );
		p.setFont( fC );
		int offsetOx = KalziumUtils::StringHeight( strLocalizedMass , fC, &p );
		p.drawText( 1, h-offsetOx, w/2, offsetOx, Qt::AlignLeft, m_element->oxstage() );

		//Mass
		fC.setPointSize( size3 );
		p.setFont( fC );
		
		int offset = KalziumUtils::StringHeight( strLocalizedMass, fC, &p );
		p.drawText( w/2, h-offset, w/2, offset, Qt::AlignRight, strLocalizedMass );

		drawBiologicalSymbol( &p );
	}

	p.end();


	bitBlt( this, 0, 0, &pm );
}

void DetailedGraphicalOverview::drawBiologicalSymbol( QPainter *p )
{
	if ( !m_element ) return;
	const int db = h_t;        //diameter of the big circle
	const int ds = db/2;      //diameter of the inner circle

	int d_ds = ( db/2 )-( ds/2 ); //the delta-x/y of the inner circle

	int pos_x = width() - 4 - db;
	int pos_y = 4;

	if ( m_element->biological() > 0 )
	{
		const int radius = db + 8;
		p->setBrush( Qt::SolidPattern );
		p->setBrush( Qt::white );
		p->setPen( Qt::black );
		p->drawRoundRect( QRect( width() - radius, 
					-radius, 
					2 * radius, 
					2 * radius ), 70, 70 );
	}
	
	switch ( m_element->biological() )
	{
		case 0:        //nothing
			break;
		case 1:        //red, red
			p->setBrush( Qt::red );
			p->setBrush(Qt::NoBrush);
			p->setPen( Qt::red );
			p->drawEllipse( pos_x,pos_y,db,db );
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::red );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 2:        //green, red
			p->setBrush( Qt::red );
			p->setBrush(Qt::NoBrush);
			p->setPen( Qt::red );
			p->drawEllipse( pos_x,pos_y,db,db );
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::green );
			p->setPen( Qt::green );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 3:        //green
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::green );
			p->setPen( Qt::green );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 4:        //green, blue
			p->setBrush( Qt::blue );
			p->setBrush(Qt::NoBrush);
			p->setPen( Qt::blue );
			p->drawEllipse( pos_x,pos_y,db,db );
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::green );
			p->setPen( Qt::green );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 5:        //blue
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::blue );
			p->setPen( Qt::blue );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 6:        //blue, blue
			p->setBrush( Qt::blue );
			p->setBrush(Qt::NoBrush);
			p->setPen( Qt::blue );
			p->drawEllipse( pos_x,pos_y,db,db );
			p->setBrush(Qt::SolidPattern);
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
	}
}

#include "detailedgraphicaloverview.moc"
