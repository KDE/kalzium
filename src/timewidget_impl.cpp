/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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

#include "timewidget_impl.h"

#include <QLabel>
#include <QSlider>
#include <QTextEdit>
#include <QList>

#include <kdebug.h>
#include <klocale.h>

#include <element.h>

#include "kalziumdataobject.h"
#include "prefs.h"

    TimeWidgetImpl::TimeWidgetImpl( QWidget *parent )
: QWidget( parent )
{
    setupUi( this );
//X 
//X     m_list = KalziumDataObject::instance()->ElementList;
//X 
//X     m_htmlBegin = "";
//X     m_htmlEnd = "";
//X 
//X     connect( time_box, SIGNAL( valueChanged( int ) ),
//X             this, SLOT( setNewTime( int ) ) );
}

//X void TimeWidgetImpl::setNewTime( int newtime )
//X {
    //this method is currently unused. That is because I have not
    //yet entered the data for a contect-widget.
    //I would like to have something like: Slidervalue on 1934, the
    //html says "in 1994 ... happened"
//X     m_htmlBegin = "In the year ";
//X     m_htmlEnd = "!";
//X 
//X     text->setText( m_htmlBegin + QString::number(newtime) + m_htmlEnd );
//X }

#include "timewidget_impl.moc"
