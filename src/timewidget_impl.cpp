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

#include <tqlayout.h>
#include <tqsizepolicy.h>
#include <tqslider.h>
#include <tqtextedit.h>

#include <knuminput.h>

TimeWidgetIMPL::TimeWidgetIMPL( TQWidget *parent, const char* name )
	: TimeWidget( parent, name )
{
	text->tqsetAlignment( text->tqalignment() | Qt::WordBreak );
	text->setTextFormat( Qt::RichText );
	text->setReadOnly( true );
	text->setPaletteBackgroundColor( paletteBackgroundColor() );
	text->setFrameStyle( TQFrame::NoFrame );
}

void TimeWidgetIMPL::slotChanged( int value )
{
	( void )value;
	text->hide();
	verticalSpacer->changeSize( 21, 5, TQSizePolicy::Fixed, TQSizePolicy::Expanding );
}

#include "timewidget_impl.moc"
