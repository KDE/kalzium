/***************************************************************************

                  infodialog.cpp  -  description
                             -------------------
    begin                : Tue Apr 8 2003 
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

#include "infodialog.h"

#include <klocale.h>
#include <qlabel.h>

#include <kdebug.h>


infoDialog::infoDialog( ElementInfo Eleminfo , QWidget *parent, const char *name)
    : infoDlg( parent , name , true )
{
    setCaption( i18n( Eleminfo.Name.utf8() ) );

	electro_label->setText( Eleminfo._EN );
	weight_label->setText( Eleminfo.Weight );
	melting_label->setText( Eleminfo._MP );
	boiling_label->setText( Eleminfo._BP );
	name_label->setText( i18n( Eleminfo.Name.utf8() ) );
	symbol_label->setText( Eleminfo.Symbol );
	radius_label->setText( Eleminfo._AR );
	discovered_label->setText( Eleminfo.date );
	elemno_label->setText( QString::number( Eleminfo.number ));
}


#include "infodialog.moc"
