/***************************************************************************

copyright            : (C) 2004, 2005, 2006, 2007 by Carsten Niehaus
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
#include "kalziumdataobject.h"
#include "kalziumutils.h"

//KDE-Includes
#include <kdebug.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include <kglobal.h>
#include <kstandarddirs.h>

//QT-Includes
#include <QFile>
#include <QPainter>
#include <QSvgRenderer>
#include <QRect>

#include <element.h>
#include "prefs.h"

DetailedGraphicalOverview::DetailedGraphicalOverview( QWidget *parent )
: QWidget( parent )
{
	setAttribute( Qt::WA_NoBackground, true );

	m_element = 0;
	setMinimumSize( 300, 200 );

	// last operation: setting the background color and scheduling an update()
	setBackgroundColor( QColor() );
}

void DetailedGraphicalOverview::setElement( int el )
{
	m_element = KalziumDataObject::instance()->element( el );
	update();
}

void DetailedGraphicalOverview::setBackgroundColor( const QColor& bgColor )
{
	m_backgroundColor = bgColor.isValid() ? bgColor : Qt::green;
	
	//this check is needed because a QBrush( QPixmap() ) constructs 
	//with a black brush. But black is a really bad color here ...
	if ( bgColor == QColor( 0, 0, 0 ) )
		m_backgroundColor = Qt::white;
	
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
        pm.fill( palette().background().color() );
        p.drawText( 0, 0, w, h, Qt::AlignCenter | Qt::TextWordWrap, i18n( "No element selected" ) );
    } else if ( Prefs::colorschemebox() == 2) { //The iconic view is the 3rd view (0,1,2,...)
        pm.fill( palette().background().color() );

        QString pathname = KGlobal::dirs()->findResourceDir( "appdata", "data/iconsets/" ) + "data/iconsets/";

        int enumii = m_element->dataAsVariant( ChemicalDataObject::atomicNumber ).toInt();

        QString filename = pathname + "school" + '/' + QString::number( enumii )  + ".svg";

        QSvgRenderer svgrenderer;
        if ( QFile::exists(filename) && svgrenderer.load(filename) ) {
            QSize size = svgrenderer.defaultSize();
            size.scale( w, h, Qt::KeepAspectRatio );

            QRect bounds( QPoint( 0, 0 ), size );
            bounds.moveCenter( QPoint( w/2, h/2 ) );
            svgrenderer.render( &p, bounds );
        } else {
            p.drawText( 0, 0, w, h, Qt::AlignCenter | Qt::TextWordWrap, i18n( "No graphic found" ) );
        }
    } else
    {
        h_t = 20; //height of the texts

        x1 =  0;
        y1 =  0;

        x2 = w;
        y2 = h;

        p.setBrush( m_backgroundColor );
        p.drawRect( x1, y1, x2 - 1, y2 - 1 );

        p.setBrush( Qt::black );
        p.setBrush(Qt::NoBrush);

        QFont fA = KGlobalSettings::generalFont();
        QFont fB = KGlobalSettings::generalFont();
        QFont fC = KGlobalSettings::generalFont();

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
        int xB = xA - fmB.width( m_element->dataAsString( ChemicalDataObject::atomicNumber ) );
        int yB = yA + fmB.height()/2;

        //Element Symbol
        p.setFont( fA );
        p.drawText( xA, yA , m_element->dataAsString( ChemicalDataObject::symbol) ); 

        //Atomic number
        p.setFont( fB );
        p.drawText( xB, yB, m_element->dataAsString( ChemicalDataObject::atomicNumber ) );

        QRect rect( 0, 20, w/2, h );

        p.setFont( fC );

        int size = KalziumUtils::maxSize(m_element->dataAsString( ChemicalDataObject::name), rect , fC, &p);


        //Name and other data
        fC.setPointSize( size );
        p.setFont( fC );

        //Name
        p.drawText( 1, 0, w/2, h, Qt::AlignLeft, m_element->dataAsString( ChemicalDataObject::name) );

        //Oxidationstates
        p.setFont( fC );

        //Mass
        QString massString = i18nc( "For example '1.0079u', the mass of an element in units", "%1 u", m_element->dataAsString( ChemicalDataObject::mass ) );
        int size3 = KalziumUtils::maxSize( massString, rect , fC, &p);
        fC.setPointSize( size3 );
        p.setFont( fC );
        int offset = KalziumUtils::StringHeight( massString, fC, &p );
        p.drawText( w/2, 
                h-offset, 
                w/2, 
                offset, 
                Qt::AlignRight, 
                massString
                );
    }

    p.end();

    p.begin( this );
    p.drawPixmap( 0, 0, pm );
    p.end();
}

#include "detailedgraphicaloverview.moc"
