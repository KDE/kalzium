/***************************************************************************
                         quizdlg.cpp  -  description
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

//KDE-Includes
#include <kdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>
#include <kprogress.h>
#include <kdebug.h>
#include <klineedit.h>
#include <kcombobox.h>

//QT-Includes
#include <qmessagebox.h>
#include <qbuttongroup.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qradiobutton.h>
#include <qstring.h>
#include <qtable.h>
#include <qwhatsthis.h>
#include <qwidget.h>
#include <qheader.h>
#include <qcheckbox.h>
#include <qvaluelist.h>
#include <qstringlist.h>
#include <qregexp.h>

#include "searchdlg.h"

SearchDlg::SearchDlg (QWidget *parent, const char *name )  
    : SearchDialogUI (parent,name)
{
	fillDataStringLists();
	fillCheckBoxList();
}

void SearchDlg::fillCheckBoxList()
{
	checkBoxes.append( checkBox1 );
	checkBoxes.append( checkBox2 );
	checkBoxes.append( checkBox3 );
	checkBoxes.append( checkBox4 );
	checkBoxes.append( checkBox5 );
	checkBoxes.append( checkBox6 );
	checkBoxes.append( checkBox7 );
	checkBoxes.append( checkBox8 );
	checkBoxes.append( checkBox9 );
	checkBoxes.append( checkBox10 );
	checkBoxes.append( checkBox11 );
	checkBoxes.append( checkBox12 );
	checkBoxes.append( checkBox13 );
	checkBoxes.append( checkBox14 );
}

void SearchDlg::fillDataStringLists()
{
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));
	for( int i = 0  ; i < 109 ;  ++i )
	{
		config.setGroup(QString::number(i+1));
		nameList.append( config.readEntry("Name", "Unknown") );
		symbolList.append( config.readEntry( "Symbol", "Unknown" ) );
		blockList.append( config.readEntry( "Block", "Unknown" ) );
		acidList.append( config.readEntry( "acidbeh", "Unknown" ) );
		groupList.append( config.readEntry( "Group", "Unknown" ) );
		orbitList.append( config.readEntry( "Orbits", "Unknown" ) );
		OxidationList.append( config.readEntry( "Ox", "Unknown" ) );
		ARList.append( config.readEntry( "AR", "Unknown" ) );
		ENList.append( config.readEntry( "EN", "Unknown" ) );
		BPList.append( config.readEntry( "BP", "Unknown" ) );
		MPList.append( config.readEntry( "MP", "Unknown" ) );
		IEList.append( config.readEntry( "IE", "Unknown" ) );
		WeightList.append( config.readEntry( "Weight", "Unknown" ) );
		DensityList.append( config.readEntry( "Density", "Unknown" ) );
	}
}

void SearchDlg::slotApplyChanges()
{
	QStringList::Iterator it = nameList.begin();
	QStringList::Iterator it2 = symbolList.begin();
	QStringList::Iterator it3 = WeightList.begin();
	QStringList::Iterator it4 = ENList.begin();
	QStringList::Iterator it5 = DensityList.begin();
	QStringList::Iterator it6 = orbitList.begin();
	QStringList::Iterator it7 =  blockList.begin();
	QStringList::Iterator it8 =  acidList.begin();
	QStringList::Iterator it9 = groupList.begin();
	QStringList::Iterator it10 = BPList.begin();
	QStringList::Iterator it11 = MPList.begin();
	QStringList::Iterator it12 = IEList.begin();
	QStringList::Iterator it13 = ARList.begin();
	QStringList::Iterator it14 = OxidationList.begin();

	for( int i = 0  ; it != nameList.end() ; i++ )
	{
		//itererate through the checkboxes and check if they are checked or not
		DataTable->setText( i, 0 , *it );
		DataTable->setText( i, 1 , *it2 );
		DataTable->setText( i, 2 , *it3 );
		DataTable->setText( i, 3 , *it4 );
		DataTable->setText( i, 4 , *it5 );
		DataTable->setText( i, 5 , *it6 );
		DataTable->setText( i, 6 , *it7 );
		DataTable->setText( i, 7 , *it8 );
		DataTable->setText( i, 8 , *it9 );
		DataTable->setText( i, 9 , *it10 );
		DataTable->setText( i, 10 , *it11 );
		DataTable->setText( i, 11 , *it12 );
		DataTable->setText( i, 12 , *it13 );
		DataTable->setText( i, 13 , *it14 );
		
		//iterate all iterators
		++it;
		++it2;
		++it3;
		++it4;
		++it5;
		++it6;
		++it7;
		++it8;
		++it9;
		++it10;
		++it11;
		++it12;
		++it13;
		++it14;
	}
	
	int col = 0;
	for ( checkBoxesList::Iterator it = checkBoxes.begin() ; it != checkBoxes.end() ; ++it )
	{
		//show or hide cols
		if ( (**it).isChecked() )
			DataTable->showColumn( col );
		else
			DataTable->hideColumn( col );
		col++;
	}

	if ( !RangeLE->text().isEmpty() ) //only parse if the user entered numbers
		slotFilterData();
}

void SearchDlg::slotFilterData()
{
	bool show;
	if ( ShowHideCB->currentItem() == 0 )
		show = true;
	else show = false;

	for ( int i = 0 ; i < 110 ; ++i )
	{
		if ( show )
			DataTable->hideRow( i );
		else
			DataTable->showRow( i );
	}
	
	IntValueList l = parseRange( RangeLE->text() );
	for ( IntValueList::Iterator it = l.begin(); it != l.end() ; ++it )
	{
		if ( show )
			DataTable->showRow( *it-1 );
		else
			DataTable->hideRow( *it-1 );
	}
}

IntValueList SearchDlg::parseDashes( QString dashedString )
{
	kdDebug() << "SearchDlg::parseDashes()" << endl;
	kdDebug() << "der String ist: " << dashedString << endl;

	IntValueList l;
	QRegExp rx( "-" );
	QString left = dashedString.left( dashedString.find( rx ) );
	QString right = dashedString.right( dashedString.length() - dashedString.find( rx ) - 1 );
	if ( left.toInt() > right.toInt() )
	{
		QMessageBox::information(  this, i18n( "dashedStringproblem" ),
				i18n( "The left value has to be smaller than the right value." ) );
		return l;
	}
	else //the dashedString is syntactically ok
	{
		for ( int i = left.toInt() ; i < right.toInt()+1 ; i++ )
		{
			l.append( i );
		}
	}
	return l;
}

IntValueList SearchDlg::parseRange(QString range)
{
	kdDebug() << "SearchDlg::parseRange()" << endl;

	IntValueList l;
	
	if ( range.contains( ";" ) ) //at least to ranges are given
	{
		kdDebug() << "enthält ;" << endl;
		QRegExp semcol_rx( ";" );
		QRegExp dash_rx( "-" );

		bool hasStillSemicolons = true;
		QString part, rest, left, right;

		int bar = 1;
		while ( hasStillSemicolons )
		{
			kdDebug() << "Durchlauf: " << bar << endl;
			if ( range.contains(";" ) )
			{
				part = range.left( range.find( semcol_rx ) );
				rest = range.right( range.length() - range.find( semcol_rx ) - 1 );
			}
			else
			{
				part = range;
				rest = "";
			}

			kdDebug() << "rest ist: " << rest << endl;
			kdDebug() << "part ist: " << part << endl;

			IntValueList e = parseDashes( part );
			for( IntValueList::Iterator it = e.begin() ; it != e.end() ; ++it )
			{
				l.append( *it );
			}

			range = rest;

			bar++;

			//check if we still need to parse
			if ( !range.isEmpty() )
			{
				kdDebug() << "Range is not empty" << endl;		
				if ( !range.contains( ";" ) )//only one element is left
				{
					l.append( range.toInt() );
					hasStillSemicolons = false;
				}
				else
					hasStillSemicolons = true;
			}
			else
			{
				hasStillSemicolons = false;
			}
			kdDebug() << "range is: " << range << endl;
		}
	}
	else
	{
		kdDebug() << "enthält kein ;" << endl;
		if ( range.contains( "-" ) ) //one range is given
		{
			l = parseDashes( range );
		}
		else //only one element is choosen
		{
			kdDebug() << "enthält nur ein Element" << endl;
			l.append( range.toInt() );
		}
	}

	//Debug here
	int i = 1;
	for ( IntValueList::Iterator it = l.begin() ; it != l.end() ; ++it )
	{
		kdDebug() << "Element " << i << " ist : " << *it << endl;
		++i;
	}
	//end the debug
	
	return l;
}

void SearchDlg::slotExportData()
{
	/*
	 * export into a .csv-file
	 */
}

#include "searchdlg.moc"
