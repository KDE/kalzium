/***************************************************************************
                         serachdlg.h  -  description
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

#ifndef SEARCHDLG_H
#define SEARCHDLG_H

class QLabel;
class QRadioButton;
class QStringList;

class QWidget;

#include "searchdialog.h"

class SearchDlg : public SearchDialogUI 
{
    Q_OBJECT

    public: 
        SearchDlg(QWidget *parent, const char *name );
	
	private:
		QStringList checkBoxes;
		void fillCheckBoxList();

		QStringList nameList,
					symbolList;

		void fillDataStringLists();
		
	private slots:
		void slotApplyChanges();
		void slotExportData();
};

#endif // SEARCHDLG_H
