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
#include <qstring.h>

class infoDialog : public infoDlg
{
	Q_OBJECT

	public:
		infoDialog( ElementInfo Eleminfo , QString symbolArray[9][18] , QWidget *parent=0, const char *name=0 );
		QString neighbourArray[3][3];
		QString helpArray[9][18];

	private:
		void getNeighbours( int );

	private slots:
		void lookup() const;
};


#endif
