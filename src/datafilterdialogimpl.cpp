/***************************************************************************

        datafilterdialog.cpp  -  description
                             -------------------
    begin                : June 2003 
    copyright            : (C) 2003 by Carsten Niehaus
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

#include <klocale.h>
#include <kdebug.h>

#include <qlabel.h>

#include "datafilterdialogimpl.h"



Datafilterdialogimpl::Datafilterdialogimpl( QWidget* parent, const char* name )
    : Datafilterdialog( parent, name )
{
}

void Datafilterdialogimpl::slotSetFilterText( int id )
{
	kdDebug() << "id is: " << id << endl;
	switch ( id )
	{
	case 0:
		ExplanationText->setText( i18n( "<b>Metals</b>: This filter will only show the metals like iron and lithium" ) );
		break;
	case 1:
		ExplanationText->setText( i18n( "Gases" ) );
		break;
	case 2:
		ExplanationText->setText( i18n( "Groups" ) );
		break;
	}
}

