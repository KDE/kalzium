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
#include <QPair>
#include <QSlider>
#include <QTextEdit>
#include <QList>
#include <QVariant>

#include <kdebug.h>
#include <knuminput.h>
#include <klocale.h>

#include <math.h>

#include <element.h>
#include <tempunit.h>

#include "kalziumdataobject.h"
#include "prefs.h"

    TimeWidgetImpl::TimeWidgetImpl( QWidget *parent )
: QWidget( parent )
{
    setupUi( this );

    m_list = KalziumDataObject::instance()->ElementList;

    m_htmlBegin = "";
    m_htmlEnd = "";

    connect( time_box, SIGNAL( valueChanged( int ) ),
            this, SLOT( setNewTime( int ) ) );
}

void TimeWidgetImpl::setNewTime( int newtime )
{
    m_htmlBegin = "In the year ";
    m_htmlBegin = "!";

    text->setText( m_htmlBegin + QString::number(newtime) + m_htmlEnd );
}

#include "timewidget_impl.moc"
