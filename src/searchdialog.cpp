#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './searchdialog.ui'
**
** Created: Sam Jun 14 14:57:02 2003
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "searchdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qcheckbox.h>
#include <kpushbutton.h>
#include <qlabel.h>
#include <kcombobox.h>
#include <klineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a SearchDialogUI as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SearchDialogUI::SearchDialogUI( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "SearchDialogUI" );
    setSizeGripEnabled( TRUE );
    SearchDialogUILayout = new QGridLayout( this, 1, 1, 11, 6, "SearchDialogUILayout"); 

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 

    DataTable = new QTable( this, "DataTable" );
    DataTable->setNumRows( 0 );
    DataTable->setNumCols( 0 );
    layout9->addWidget( DataTable );

    layout8 = new QVBoxLayout( 0, 0, 6, "layout8"); 

    layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 

    checkBox1 = new QCheckBox( this, "checkBox1" );
    layout6->addWidget( checkBox1 );

    checkBox2 = new QCheckBox( this, "checkBox2" );
    layout6->addWidget( checkBox2 );

    checkBox3 = new QCheckBox( this, "checkBox3" );
    layout6->addWidget( checkBox3 );

    checkBox4 = new QCheckBox( this, "checkBox4" );
    layout6->addWidget( checkBox4 );

    checkBox5 = new QCheckBox( this, "checkBox5" );
    layout6->addWidget( checkBox5 );

    checkBox6 = new QCheckBox( this, "checkBox6" );
    layout6->addWidget( checkBox6 );

    checkBox7 = new QCheckBox( this, "checkBox7" );
    layout6->addWidget( checkBox7 );

    checkBox8 = new QCheckBox( this, "checkBox8" );
    layout6->addWidget( checkBox8 );

    checkBox9 = new QCheckBox( this, "checkBox9" );
    layout6->addWidget( checkBox9 );

    checkBox10 = new QCheckBox( this, "checkBox10" );
    layout6->addWidget( checkBox10 );

    checkBox11 = new QCheckBox( this, "checkBox11" );
    layout6->addWidget( checkBox11 );

    checkBox12 = new QCheckBox( this, "checkBox12" );
    layout6->addWidget( checkBox12 );

    checkBox13 = new QCheckBox( this, "checkBox13" );
    layout6->addWidget( checkBox13 );

    checkBox14 = new QCheckBox( this, "checkBox14" );
    layout6->addWidget( checkBox14 );

    checkBox15 = new QCheckBox( this, "checkBox15" );
    layout6->addWidget( checkBox15 );

    checkBox16 = new QCheckBox( this, "checkBox16" );
    layout6->addWidget( checkBox16 );
    layout8->addLayout( layout6 );
    QSpacerItem* spacer = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout8->addItem( spacer );

    ApplyButton = new KPushButton( this, "ApplyButton" );
    layout8->addWidget( ApplyButton );
    layout9->addLayout( layout8 );

    SearchDialogUILayout->addLayout( layout9, 0, 0 );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonExport = new QPushButton( this, "buttonExport" );
    buttonExport->setAutoDefault( TRUE );
    Layout1->addWidget( buttonExport );

    SearchDialogUILayout->addLayout( Layout1, 2, 0 );

    layout6_2 = new QHBoxLayout( 0, 0, 6, "layout6_2"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout6_2->addWidget( textLabel1 );

    KindCB = new KComboBox( FALSE, this, "KindCB" );
    layout6_2->addWidget( KindCB );

    WhatCB = new KComboBox( FALSE, this, "WhatCB" );
    layout6_2->addWidget( WhatCB );

    ValueLE = new KLineEdit( this, "ValueLE" );
    layout6_2->addWidget( ValueLE );
    QSpacerItem* spacer_2 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6_2->addItem( spacer_2 );

    kPushButton2 = new KPushButton( this, "kPushButton2" );
    layout6_2->addWidget( kPushButton2 );

    SearchDialogUILayout->addLayout( layout6_2, 1, 0 );
    languageChange();
    resize( QSize(801, 623).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ValueLE, SIGNAL( returnPressed() ), this, SLOT( slotFilterData() ) );
    connect( buttonExport, SIGNAL( clicked() ), this, SLOT( slotExportData() ) );
    connect( ApplyButton, SIGNAL( clicked() ), this, SLOT( slotApplyChanges() ) );
    connect( kPushButton2, SIGNAL( clicked() ), this, SLOT( slotFilterData() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
SearchDialogUI::~SearchDialogUI()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SearchDialogUI::languageChange()
{
    setCaption( tr2i18n( "Search data" ) );
    checkBox1->setText( tr2i18n( "Electronegativity" ) );
    checkBox2->setText( tr2i18n( "Density" ) );
    checkBox3->setText( tr2i18n( "Orbits" ) );
    checkBox4->setText( tr2i18n( "Name" ) );
    checkBox5->setText( tr2i18n( "Symbol" ) );
    checkBox6->setText( tr2i18n( "Weight" ) );
    checkBox7->setText( tr2i18n( "Block" ) );
    checkBox8->setText( tr2i18n( "Acidic Behaviour" ) );
    checkBox9->setText( tr2i18n( "Date of discovery" ) );
    checkBox10->setText( tr2i18n( "Group" ) );
    checkBox11->setText( tr2i18n( "Meltingpoint" ) );
    checkBox12->setText( tr2i18n( "Boilingpoint" ) );
    checkBox13->setText( tr2i18n( "Ionization energy" ) );
    checkBox14->setText( tr2i18n( "Atom radius" ) );
    checkBox15->setText( tr2i18n( "Oxydationstages" ) );
    checkBox16->setText( tr2i18n( "Elementnumber" ) );
    ApplyButton->setText( tr2i18n( "Apply" ) );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonExport->setText( tr2i18n( "&Export Data" ) );
    buttonExport->setAccel( QKeySequence( tr2i18n( "Alt+E" ) ) );
    textLabel1->setText( tr2i18n( "Filter:" ) );
    kPushButton2->setText( tr2i18n( "&Ok" ) );
}

void SearchDialogUI::slotExportData()
{
    qWarning( "SearchDialogUI::slotExportData(): Not implemented yet" );
}

void SearchDialogUI::slotApplyChanges()
{
    qWarning( "SearchDialogUI::slotApplyChanges(): Not implemented yet" );
}

void SearchDialogUI::slotFilterData()
{
    qWarning( "SearchDialogUI::slotFilterData(): Not implemented yet" );
}

#include "searchdialog.moc"
