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
	}
}

void SearchDlg::slotApplyChanges()
{
	DataTable->setNumCols( 2 );
	DataTable->setNumRows( 109 );

	QHeader *header = DataTable->horizontalHeader();
	header->setLabel(  0, QObject::tr(  "Name" ) );
	header->setLabel(  1, QObject::tr(  "Symbol" ) );
	header->setMovingEnabled( TRUE );
	
	int i = 0;
//X 	for( QStringList::Iterator it = nameList.begin() ; it != nameList.end() ; ++it , i++ )
//X 	{
//X 		//itererate through the checkboxes and check if they are checked or not
//X 		DataTable->setText( i,0,"foo");
//X 		kdDebug() << i << endl;
//X 	}
	
	for ( int i = 0 ; i < 109 ; i++ )
	{
		DataTable->setText( i,0,"foo" );
		kdDebug() << "i ist: " << i << endl;
	}

}

void SearchDlg::slotExportData()
{
	//TODO
}

#include "searchdlg.moc"
