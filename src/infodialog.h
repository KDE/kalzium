/***************************************************************************

        settingsdialog.h  -  description
                             -------------------
    begin                : Tue Apr 2 20:43:44 2002 UTC
    copyright            : (C) 2002 by Robert Gogolok
    email                : mail@robert-gogolok.de
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

#include "infodlg.h"
#include "elementkp.h"
#include <qwidget.h>

class infoDialog : public infoDlg
{
	Q_OBJECT

	public:
		infoDialog( ElementInfo Eleminfo , QWidget *parent=0, const char *name=0 );
	private slots:
		void lookup() const;
};


#endif
