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
#include "datafilterdialogimpl.h"

#include <qcheckbox.h>
#include <qframe.h>
#include <qlabel.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <kpushbutton.h>
#include <qlayout.h>
#include <qwhatsthis.h>

Datafilterdialogimpl::Datafilterdialogimpl( QWidget* parent, const char* name )
    : Datafilterdialog( parent, name )
{
}

