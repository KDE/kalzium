/***************************************************************************
                         searchdlg.h  -  description
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
		
typedef QValueList<int> IntValueList;

class SearchDlg : public SearchDialogUI 
{
    Q_OBJECT

    public: 
        SearchDlg(QWidget *parent, const char *name, bool modal = true );
		
	
	private:
		QString beautifyOrbits( QString orbits ) const;
		typedef QValueList<QCheckBox*> checkBoxesList;
		checkBoxesList checkBoxes;
		void fillCheckBoxList();
		void slotFilter();

		QStringList nameList,
					symbolList,
					blockList,
					acidList,
					groupList,
					orbitList,
					OxidationList,
					IEList,
					ARList,
					ENList,
					BPList,
					MPList,
					WeightList,
					DensityList;
		
		typedef QValueList<QStringList> StrList;
		StrList StringLists;

		void fillDataStringLists();
		
		IntValueList parseDashes(QString);
		IntValueList parseRange(QString);
		

	private slots:
		void slotApplyChanges();
	
		/*
		 * export into a .csv-file
		 */
		void slotExportData();
		void slotFilterData();
};


#endif // SEARCHDLG_H
