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
#include <qtextedit.h>

#include <knuminput.h>

TimeWidgetIMPL::TimeWidgetIMPL( QWidget *parent, const char* name )
	: TimeWidget( parent, name )
{
	text->setAlignment( text->alignment() | Qt::WordBreak );
	text->setTextFormat( Qt::RichText );
	text->setReadOnly( true );
	text->setPaletteBackgroundColor( paletteBackgroundColor() );
	text->setFrameStyle( QFrame::NoFrame );
}

void TimeWidgetIMPL::slotChanged( int value )
{
	( void )value;
	text->hide();
	verticalSpacer->changeSize( 21, 5, QSizePolicy::Fixed, QSizePolicy::Expanding );
}

#include "timewidget_impl.moc"
