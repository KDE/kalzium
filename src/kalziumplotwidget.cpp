/***************************************************************************

                          KalziumGraph.cpp  -  description
                             -------------------
    begin                : Wed Oct 23 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#include "kalziumplotwidget.h"

//KDE-Includes
#include <klocale.h>
#include <kdebug.h>

//QT-Includes
#include <qpainter.h>

KalziumPlotWidget::KalziumPlotWidget( QWidget *parent, const char *name) : QWidget( parent, name )
{
}

void KalziumPlotWidget::paintEvent( QPaintEvent* )
{
	QPainter p;
	p.begin( this );

	p.drawRect( 0,0,this->height(),this->width() ); //debug
	
	p.end();
}
