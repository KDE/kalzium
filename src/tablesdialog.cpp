/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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

#include "tablesdialog.h"

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>
#include <kcombobox.h>
#include <kstdaction.h>
#include <ktoolinvocation.h>

#include <QLayout>
#include <QHeaderView>

#include "ui_numbers.h"

TablesDialog::TablesDialog( QWidget *parent )
    : KDialogBase( IconList, 
			"tablesdialog", 
			Help|Close, 
			Close,
			parent, 
			"tablesdialog",
			false, //non modal
			false, 
			KGuiItem(i18nc("Next element", "Next"), "1rightarrow"), 
			KGuiItem(i18nc("Previous element", "Previous"), "1leftarrow"))
{
	QFrame *frame = addPage("Numbers", "numbers", BarIcon("numbers"));
	QVBoxLayout *layout = new QVBoxLayout( frame );
	layout->setMargin( 0 );
	
	Ui::Numbers *n = new Ui::Numbers();
	//this line crashes...
//	n->tableWidget->verticalHeader()->hide();
	QWidget *foo = new QWidget();
	n->setupUi( foo );
	layout->addWidget( foo );
}

TablesDialog::~TablesDialog()
{
}

#include "tablesdialog.moc"

