/***************************************************************************

        infodialog.h  -  description
                             -------------------
    begin                : Tue Apr 2 20:43:44 2002 UTC
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

#ifndef _INFODIALOG_H_
#define _INFODIALOG_H_

class QWidget;

#include "elementkp.h"
#include "infodlg.h"

class infoDialog : public infoDlg
{
	Q_OBJECT

	public:
		infoDialog( ElementInfo Eleminfo , QWidget *parent=0, const char *name=0 );
};
		
#endif
