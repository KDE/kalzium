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

//QT-Includes
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

#include "searchdlg.h"

SearchDlg::SearchDlg (QWidget *parent, const char *name )  
    : SearchDialogUI (parent,name)
{
	fillDataStringLists();
}

void SearchDlg::fillCheckBoxList()
{
	checkBoxes.append( "checkBox1" );
	checkBoxes.append( "checkBox2" );
	checkBoxes.append( "checkBox3" );
	checkBoxes.append( "checkBox4" );
	checkBoxes.append( "checkBox5" );
	checkBoxes.append( "checkBox6" );
	checkBoxes.append( "checkBox7" );
	checkBoxes.append( "checkBox8" );
	checkBoxes.append( "checkBox9" );
	checkBoxes.append( "checkBox10" );
	checkBoxes.append( "checkBox11" );
	checkBoxes.append( "checkBox12" );
	checkBoxes.append( "checkBox13" );
	checkBoxes.append( "checkBox14" );
	checkBoxes.append( "checkBox15" );
	checkBoxes.append( "checkBox16" );
}

void SearchDlg::fillDataStringLists()
{
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));
	for( int i = 0  ; i < 109 ;  ++i )
	{
		config.setGroup(QString::number(i+1));
		nameList.append( config.readEntry("Name", "Unknown") );
		symbolList.append( config.readEntry( "Symbol", "Unknown" ) );
		ARList.append( config.readDoubleNumEntry( "AR", -1 ) );
		ENList.append( config.readDoubleNumEntry( "EN", -1 ) );
		BPList.append( config.readDoubleNumEntry( "BP", -1 ) );
		MPList.append( config.readDoubleNumEntry( "MP", -1 ) );
		IEList.append( config.readDoubleNumEntry( "IE", -1 ) );
	}
}

void SearchDlg::slotApplyChanges()
{
	DataTable->setNumCols( 7 );
	DataTable->setNumRows( 109 );

	QHeader *header = DataTable->horizontalHeader();
	header->setLabel(  0, QObject::tr(  "Name" ) );
	header->setLabel(  1, QObject::tr(  "Symbol" ) );
	header->setLabel(  2, QObject::tr(  "Atomic radius" ) );
	header->setLabel(  3, QObject::tr(  "Electronegativity" ) );
	header->setLabel(  4, QObject::tr(  "Boilingpoint" ) );
	header->setLabel(  5, QObject::tr(  "Meltingpoint" ) );
	header->setLabel(  6, QObject::tr(  "Ion.energy" ) );
	header->setMovingEnabled( TRUE );
	
	int i = 0;
	QStringList::Iterator it = nameList.begin();
	DoubleList::Iterator it2 = IEList.begin();
	DoubleList::Iterator it3 = ARList.begin();
	DoubleList::Iterator it4 = ENList.begin();
	DoubleList::Iterator it5 = BPList.begin();
	DoubleList::Iterator it6 = MPList.begin();
	QStringList::Iterator it7 = symbolList.begin();
	for( int i = 0  ; it != nameList.end() ; i++ )
	{
		//itererate through the checkboxes and check if they are checked or not
		DataTable->setText( i, 0 , *it );
		DataTable->setText( i, 1 , *it7 );
		DataTable->setText( i, 2 , QString::number( *it3 ));
		DataTable->setText( i, 3 , QString::number( *it4 ));
		DataTable->setText( i, 4 , QString::number( *it5 ));
		DataTable->setText( i, 5 , QString::number( *it6 ));
		DataTable->setText( i, 6 , QString::number( *it2 ));
		++it;
		++it2;
		++it3;
		++it4;
		++it5;
		++it6;
		++it7;
	}
}

void SearchDlg::slotFilterData()
{
	//TODO
}

void SearchDlg::slotExportData()
{
	//TODO
}

#include "searchdlg.moc"
