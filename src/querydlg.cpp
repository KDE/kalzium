/***************************************************************************

        querydlg.cpp  -  description
                             -------------------
    begin                : Apr 17 2003 
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

#include <kdebug.h>



#include "querydlg.h"

/**
 * this class will provide a searchinterface
 */

QueryDlg::QueryDlg( QWidget *parent, const char *name)
    : KDialog( parent,name)
{
}


#include "querydlg.moc"
