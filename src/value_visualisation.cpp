/***************************************************************************
                         ValueVisualisation.cpp  -  description
                             -------------------
    begin                : Fr Mar 22 2002 
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

//KDE-Includes
#include <kconfig.h>
#include <klocale.h>

//QT-Includes
#include <qcanvas.h>
#include <qfont.h>
#include <qlayout.h>
#include <qwhatsthis.h>

#include "eleminfo.h"
#include "elementkp.h"
#include "kalzium.h"
#include "value_visualisation.h"
#include "value_visualisation.moc"


ValueVisualisation::ValueVisualisation (QWidget *parent, const char *name, Kalzium *kalzium_tmp)  : KDialog (parent,name)
{
    kalzium = kalzium_tmp;
    this->setCaption(i18n("AR"));
    QCanvas *canvas = new QCanvas( this );
    canvas->resize( 1000 , 1000 );
//    int x = ( canvas->width() )/2;
    int h = 0, v = 0;
    QCanvasEllipse *ball[ 85 ];
    QBrush brush( blue );
    for ( int i = 0 ; i < 86 ; i++ )
    {
        position(i+1,h,v); //get position
        ball[i] = new QCanvasEllipse( kalzium->element[i]->Data.AR*0.2, kalzium->element[i]->Data.AR*0.2 , canvas );

        ball[i]->setBrush( brush );
        ball[i]->setPen( NoPen );
        ball[i]->move( 50+h*1.125 , v*2.5 );
        ball[i]->show();
    }
       
/*    QCanvasLine *line = new QCanvasLine( canvas );
    line->setPoints( 40,40,800,40 );
    line->setBrush( brush );
    line->show();
    */
    QCanvasView *view = new QCanvasView(   canvas, this, "", QCanvasView::WStyle_Customize | QCanvasView::WStyle_NoBorderEx );


    view->resize( 1000, 1000 );
    view->setVScrollBarMode( QCanvasView::AlwaysOff );
    view->setHScrollBarMode( QCanvasView::AlwaysOff );
    view->show();
}

