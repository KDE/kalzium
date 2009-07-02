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

#include <QTimer>

#include <kdebug.h>
#include <klocale.h>

#include <element.h>

#include "kalziumdataobject.h"
#include "prefs.h"

    TimeWidgetImpl::TimeWidgetImpl( QWidget *parent )
: QWidget( parent )
{
    setupUi( this );
    m_timer = new QTimer( this );
    connect( Play, SIGNAL (clicked()),
             this, SLOT( play()));
    connect( m_timer, SIGNAL(timeout()),
             this, SLOT( tick()) );
    m_mode = 0;
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

void TimeWidgetImpl::play(void)
{

    if ( m_mode == 1)   //Currently playing
    {
        //The Mode is 'Play' so stop
        stop();
        return;
    }

    //The mode is not 'play'
    //If the slider is at the maximum position bring it to the minimum
    if ((time_slider) -> value() == time_slider -> maximum())
        time_slider -> setValue ( time_slider -> minimum () );

    //start the timer at 200 milisecond time interval with single shot disabled
    m_timer -> start( 200 );

    m_mode = 1;          //start playing
    Play-> setText(i18n("Pause"));
}

void TimeWidgetImpl::stop(void)
{
    //Currently playing, stop the timer.
    m_timer -> stop();
    Play -> setText(i18n("Play"));
    m_mode = 0;         //Stop
}

void TimeWidgetImpl::tick(void)
{
    int speed = Speed -> value();
    int increment = speed;
    int temp = time_slider -> value();
    int max = time_slider -> maximum();
    if (temp + increment > max)
        stop();
    time_slider -> setValue ( temp + increment );
}
#include "timewidget_impl.moc"
