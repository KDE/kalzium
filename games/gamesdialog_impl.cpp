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
#include "gamesdialog_impl.h"

GamesDialog_Impl::GamesDialog_Impl( QWidget * parent )
	: QWidget( parent )
{
	ui.setText( this );
}

#include "gamesdialog_impl.moc"
