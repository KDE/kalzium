/***********************************************************************************
* Mass calculator: Plasmoid to calculate mass of a Molecule.
* Copyright (C) 2009, 2010 Etienne Rebetez, etienne.rebetez@oberwallis.ch
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
***********************************************************************************/

// Qt
#include <QtGui/QApplication>
#include <QGraphicsLinearLayout>
#include <QClipboard>

// KDE
#include <KLocale>
#include <KConfigDialog>
#include <psetables.h>

// Plasma
#include <plasma/theme.h>

// local
#include "Molmasscalculator.h"

Molmasscalculator::Molmasscalculator ( QObject *parent, const QVariantList &args )
        : Plasma::PopupApplet ( parent, args ),
        m_widget( 0 ),
        m_PeriodWidget( 0 ),
        m_lineedit( 0 ),
        m_MassLabel( 0 ),
        m_switchButton( 0 )
{
    // Some Applet settings
    setBackgroundHints ( DefaultBackground );
    setAspectRatioMode ( Plasma::IgnoreAspectRatio );
    setHasConfigurationInterface ( true );
    setAcceptDrops ( false );
    setAssociatedApplication("kalzium");

    // Needed for the Popupapplet
    setPopupIcon ( "kalzium" );
    setMinimumSize ( 300, 55 );
}

Molmasscalculator::~Molmasscalculator()
{
    if (!hasFailedToLaunch()) {
        saveConfig();
    }

    delete m_switchButton;
    delete m_MassLabel;
    delete m_lineedit;
    delete m_PeriodWidget;
    delete m_widget;
}

void Molmasscalculator::init()
{
    // checking if the Dataengine is availiable
    if (!dataEngine( "kalzium" )->isValid()) {
        setFailedToLaunch(true, i18n("Dataengine \"kalzium\" not found"));
    }

    // loads the configuration
    loadConfig();
}

//Sends the requests to the Dataengine
void Molmasscalculator::ParseMolecule( QString strInput)
{
    if ( !strInput.isEmpty() ) {
        m_molecule = dataEngine ( "kalzium" )->query ( QString ( "Molecule:Parser:" ) + strInput );
        newCalculatedMass();
    }
}

//Without parameters the lineEdit Text is taken.
void Molmasscalculator::ParseMolecule()
{
    ParseMolecule(m_lineedit->text());
}

// Adding a new element to the Lineedit and Parse it.
void Molmasscalculator::appendElement ( QString ElementSymbol )
{
    QString molecule;
    molecule = m_lineedit->text();
    molecule.append ( ElementSymbol );

    ParseMolecule(molecule);
}

// Sets the new Mass and Molecule after a calculation.
void Molmasscalculator::newCalculatedMass()
{
    if ( m_molecule["molMass"].toString() != "" ) {

        //Set new MassLabel Text
        m_MassLabel->setText ( m_molecule["molMass"].toString() + " u" );

        //Sets the niceMolecule string in the Lineedit	  //Configuration Option?
        m_lineedit->setText ( m_molecule["niceMolecule"].toString() );

        //Copy new Mass to Clipboard
        if ( m_copyToClipboard and m_molecule["molMass"].toString() != "" )
            QApplication::clipboard()->setText ( m_molecule["molMass"].toString() );

    } else {
        m_MassLabel->setText ( i18n ( "Invalid Molecule" ) );
    }
}

QGraphicsWidget *Molmasscalculator::graphicsWidget()
{
    if ( !m_widget )
    {
        m_widget = new QGraphicsWidget(this);

        QGraphicsLinearLayout *MainLayout = new QGraphicsLinearLayout ( Qt::Vertical , m_widget );
        QGraphicsLinearLayout *TopLayout = new QGraphicsLinearLayout ( Qt::Horizontal );

        Plasma::Label *MoleculeLabel = new Plasma::Label();
        MoleculeLabel->setText ( i18n ( "Molecule:" ) );

        // Adding Masslabel to Plasmoid
        m_MassLabel = new Plasma::Label;
        m_MassLabel->setAlignment ( Qt::AlignCenter );
        m_MassLabel->setScaledContents ( true );
        m_MassLabel->setStyleSheet ( "font-size:18px" );

        m_lineedit = new Plasma::LineEdit();
        m_lineedit->setClearButtonShown ( true );
        m_lineedit->setMinimumWidth ( 100 );
        m_lineedit->setText ( i18n ( "C2H5OH" ) );
        connect ( m_lineedit, SIGNAL ( editingFinished() ),this, SLOT ( ParseMolecule() ) );
        connect ( m_lineedit, SIGNAL ( returnPressed() ),this, SLOT ( ParseMolecule() ) );

        m_switchButton = new Plasma::IconWidget();
        connect ( m_switchButton, SIGNAL ( clicked() ), this, SLOT ( toggleTable() ) );

        TopLayout->addItem ( MoleculeLabel );
        TopLayout->addItem ( m_lineedit );
        TopLayout->addItem ( m_switchButton );
        TopLayout->setSpacing(0);
        TopLayout->setContentsMargins(0,0,0,0);
        TopLayout->setAlignment(m_switchButton, Qt::AlignCenter);

        // Create the Periodic Table
        m_PeriodWidget = new PeriodicGrid( config().readEntry("tableType", 0 ), this );

        MainLayout->addItem ( TopLayout );
        MainLayout->addItem ( m_MassLabel );
        MainLayout->addItem ( m_PeriodWidget );
        MainLayout->setSpacing(0);
        MainLayout->setContentsMargins(0,0,0,0);

        //set sizes
        managePeriodSystem();

        // Calculate the demo molecule.
        ParseMolecule();
    }

    return m_widget;
}


//Resets the size of the plasmoid if the periodsystem is shown or not.
void Molmasscalculator::managePeriodSystem()
{
    QPixmap newIcon;
    int x, y;

    if ( m_showPeriodicTable ) {
        newIcon = KIconLoader::global()->loadIcon( "arrow-down", KIconLoader::NoGroup, KIconLoader::SizeSmall );
        m_PeriodWidget->show();
        x = pseTables::instance()->getTabletype(m_PeriodWidget->getCurrentPseTyp())->coordsMax().x() * 33;
        y = pseTables::instance()->getTabletype(m_PeriodWidget->getCurrentPseTyp())->coordsMax().y() * 34;
    } else {
        newIcon = KIconLoader::global()->loadIcon( "arrow-right", KIconLoader::NoGroup, KIconLoader::SizeSmall );
        m_PeriodWidget->hide();
        x = 300;
        y = 60;
    }
    m_switchButton->setIcon(newIcon);
    resize( x, y );
    m_widget->setPreferredSize( x, y );
    m_widget->resize( x, y );
}

//The configuration interface.
void Molmasscalculator::createConfigurationInterface ( KConfigDialog* parent )
{
    parent->setButtons ( KDialog::Ok | KDialog::Cancel | KDialog::Apply );

    QWidget *widget = new QWidget;

    m_ui.setupUi ( widget );

    parent->addPage ( widget, i18n ( "General" ), "kalzium" );

    m_ui.showPeriodic->setChecked ( m_showPeriodicTable );
    m_ui.pasteToCliboard->setChecked ( m_copyToClipboard );

    foreach(QString thisTable, pseTables::instance()->tables()) {
         m_ui.tabletyp->addItem(thisTable);
    }

    m_ui.tabletyp->setCurrentIndex(m_PeriodWidget->getCurrentPseTyp());

    connect ( parent, SIGNAL ( applyClicked() ), this, SLOT ( configChanged() ) );
    connect ( parent, SIGNAL ( okClicked() ), this, SLOT ( configChanged() ) );
}

void Molmasscalculator::configChanged()
{
    if ( m_ui.showPeriodic->isChecked() != m_showPeriodicTable ) {
        m_showPeriodicTable = m_ui.showPeriodic->isChecked();
        managePeriodSystem();
    }

    if ( m_ui.pasteToCliboard->checkState() != m_copyToClipboard ) {
        m_copyToClipboard = m_ui.pasteToCliboard->checkState();
    }

    if ( m_ui.tabletyp->currentIndex() != m_PeriodWidget->getCurrentPseTyp()) {
        m_PeriodWidget->setCurrentPseTyp(m_ui.tabletyp->currentIndex());
	managePeriodSystem();
    }
}

void Molmasscalculator::toggleTable()
{
    if (m_showPeriodicTable ) {
        m_showPeriodicTable = false;
    } else {
        m_showPeriodicTable = true;
    }
    managePeriodSystem();
}

void Molmasscalculator::saveConfig()
{
    config().writeEntry("showPeriodicTable",m_showPeriodicTable);
    config().writeEntry("copyToClipboard",m_copyToClipboard);
    config().writeEntry("tableType",m_PeriodWidget->getCurrentPseTyp());
}

void Molmasscalculator::loadConfig()
{
    m_showPeriodicTable = config().readEntry("showPeriodicTable", true );
    m_copyToClipboard = config().readEntry("copyToClipboard", false );
}

#include "Molmasscalculator.moc"
