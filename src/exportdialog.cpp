/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "exportdialog.h"
#include "element.h"

#include <qcheckbox.h>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kglobal.h>
#include <klistview.h>
#include <klistviewsearchline.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>

#include <qfile.h>
#include <qlabel.h>
#include <qheader.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qstringlist.h>
#include <qtoolbutton.h>
#include <qbuttongroup.h>

ExportDialog::ExportDialog( KalziumDataObject *data, QWidget *parent, const char *name)
    : KDialogBase( Plain, i18n( "Export Data" ), User1|Close, Close, parent, name, false, false, KGuiItem( "Export" ) )
{
	QVBoxLayout *vbox = new QVBoxLayout( plainPage(), 0, KDialog::spacingHint() );
	
	QButtonGroup *group = new QButtonGroup( 1, Qt::Horizontal, i18n( "Selectable Datasets" ), plainPage() );

	vbox->addWidget( group );

	QStringList names;
	names.append( i18n( "Atomic Radius" ) );
	names.append( i18n( "Covalent Radius" ) );
	names.append( i18n( "van der Waals Radius" ) );
	names.append( i18n( "Atomic Mass" ) );
	names.append( i18n( "Density" ) );
	names.append( i18n( "Boiling point" ) );
	names.append( i18n( "Melting point" ) );
	
	QStringList::Iterator iterator = names.begin();
	QStringList::Iterator itEnd = names.end();

	for ( ; iterator != itEnd; ++iterator )
	{
		new QCheckBox( *iterator, group, "names-item" );
	}

	m_data = data;
}

ExportDialog::~ExportDialog()
{
}

QString ExportDialog::generateCSV()
{
	QString foo;
	return foo;

}

void ExportDialog::slotUser1()
{
	EList foo  = m_data->ElementList;
	EList::Iterator it = foo.begin();
	const EList::ConstIterator itEnd = foo.end();

//X 	for ( ; it != itEnd; ++it )
//X 	{
//X 	}

}

#include "exportdialog.moc"
