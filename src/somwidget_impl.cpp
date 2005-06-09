/***************************************************************************
    copyright            : (C) 2005 by Carsten Niehaus
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

#include "somwidget_impl.h"

#include <qdom.h>
#include <qfile.h>
#include <qlabel.h>

#include <kdebug.h>
#include <kfiledialog.h>
#include <knuminput.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include <math.h>

SOMWidgetIMPL::SOMWidgetIMPL( QWidget *parent, const char* name )
	: SOMWidget( parent,name )
{
}

#include "somwidget_impl.moc"
