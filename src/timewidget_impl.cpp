/***************************************************************************
    copyright            : (C) 2005 by Carsten Niehaus
    copyright            : (C) 2005 by Pino Toscano
    email                : kalzium@kde.org
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

#include <qlayout.h>
#include <qsizepolicy.h>
#include <qslider.h>

#include <knuminput.h>

TimeWidgetIMPL::TimeWidgetIMPL( QWidget *parent )
	: QWidget( parent )
{
	setupUi( this );

	connect( time_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( slotChanged( int ) ) );
	connect( time_slider, SIGNAL( valueChanged( int ) ),
	         Number1, SLOT( setValue( int ) ) );
	connect( Number1, SIGNAL( valueChanged( int ) ),
	         time_slider, SLOT( setValue( int ) ) );
	connect( Number1, SIGNAL( valueChanged( int ) ),
	         this, SLOT( slotChanged( int ) ) );
}

int TimeWidgetIMPL::date() const
{
	return time_slider->value();
}

void TimeWidgetIMPL::slotChanged( int value )
{
	// emit this for the world
	emit dateChanged( value );
}

#include "timewidget_impl.moc"
