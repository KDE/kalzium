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
#include "detailedgraphicaloverview.h"
#include "element.h"
#include "prefs.h"

//KDE-Includes
#include <kdebug.h>
#include <klocale.h>
#include <kglobalsettings.h>

//QT-Includes
#include <qpainter.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qstring.h>
#include <qpixmap.h>

DetailedGraphicalOverview::DetailedGraphicalOverview( Element *el, QWidget *parent, const char *name ) 
	: QWidget( parent, name )
{
	setBackgroundMode( NoBackground );

	e = el;
}

DetailedGraphicalOverview::DetailedGraphicalOverview( QWidget *parent, const char *name ) 
	: QWidget( parent, name )
{
	setBackgroundMode( NoBackground );
}

void DetailedGraphicalOverview::setElement( Element *el )
{
	e = el;
}

void DetailedGraphicalOverview::paintEvent( QPaintEvent* )
{
	if ( !e ) return;

	int h = height();
	int w = width();

	QPixmap pm( w, h );

	QPainter p;
	p.begin( &pm );

	h_t = 20; //height of the texts
	
	x1 =  0;
	y1 =  0;

	x2 = w;
	y2 = h;

	p.setBrush(Qt::SolidPattern);
	p.setBrush( PSEColor( e->block() ));
	p.drawRect( x1 , y1 , x2 , y2 );

	p.setBrush( Qt::black );
	p.setBrush(Qt::NoBrush);

	QFont f1 = KGlobalSettings::generalFont();
	f1.setPointSize( 18 );
	QFont f2 = KGlobalSettings::generalFont();
	f2.setPointSize( 14 );
	QFont f3 = KGlobalSettings::generalFont();
	f2.setPointSize( 13 );

	p.setFont( f1 );
	int h_ = y2/10*3; //a third of the whole widget
	p.drawText( x2/10 * 4, h_ , e->symbol() ); //Symbol

	p.setFont( f2 );
 	p.drawText( x1+4, y2-h_t , x2/2 , h_t , Qt::AlignLeft , i18n( e->elname().utf8() )); //Name
	
	p.drawText( x1+4+h_t , y2-2*h_t , x2-x1-4-h_t , h_t , Qt::AlignRight , e->oxstage() );    //Oxidationszahlen
	p.drawText( x2/2 , y2-h_t , x2/2-4 , h_t , Qt::AlignRight , QString::number( e->weight() )); //Weight

	p.setFont( f3 );
	p.drawText( x2/10*3 , h_-h_t , x2/10, h_t , Qt::AlignRight , QString::number( e->number() ));
	drawBiologicalSymbol( &p );

	p.end();

	bitBlt( this, 0, 0, &pm );
}

void DetailedGraphicalOverview::drawBiologicalSymbol( QPainter *p )
{
	const int db = h_t;        //diameter of the big circle
	const int ds = db/2;      //diameter of the inner circle

	int d_ds = ( db/2 )-( ds/2 ); //the delta-x/y of the inner circle

	int pos_x = x1+4;
	int pos_y = y2 - 5 - 2*h_t;

	switch ( e->biological() )
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

QColor DetailedGraphicalOverview::PSEColor( const QString &block ) const
{
	QColor c;

	if ( block == "s" )
		c = Prefs::block_s();
	else if ( block == "d" )
		c = Prefs::block_p();
	else if ( block == "p" )
		c = Prefs::block_d();
	else if ( block == "f" )
		c = Prefs::block_f();
	return  c;
}


