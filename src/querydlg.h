/***************************************************************************

        querydlg.h  -  description
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

#ifndef _QUERYDLG_H_
#define _QUERYDLG_H_

#include <kdialogbase.h>
#include <qtabwidget.h>
#include <qframe.h>
#include <kdialog.h>

class QVButtonGroup;
class QComboBox;
class QGridLayout;
class QLabel;
class QString;

class QueryDlg : public KDialog
{
	Q_OBJECT

	public:
		QueryDlg( QWidget *parent, const char *name );

};

#endif
