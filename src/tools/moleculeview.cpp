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
#include "moleculeview.h"
#include "../../libavogadro-kalzium/src/toolgroup.h"

#include <QMessageBox>
#include <QGLFormat>
#include <kdebug.h>
#include <kfiledialog.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <KLocale>
#include <KTextBrowser>
#include <knewstuff2/engine.h>

#include <openbabel2wrapper.h>

using namespace OpenBabel;

MoleculeDialog::MoleculeDialog( QWidget * parent )
	: KDialog( parent )
{
	// use multi-sample (anti-aliased) OpenGL if available
	QGLFormat defFormat = QGLFormat::defaultFormat();
	defFormat.setSampleBuffers(true);
	QGLFormat::setDefaultFormat(defFormat);

	setCaption( i18n( "Molecular Viewer" ) );
	setButtons( Help | User2 | User1 | Close );

	setDefaultButton( User1 );

	setButtonGuiItem( User1, KGuiItem( i18n( "Load molecule" ), "document-open", i18n( "Loading a molecule" ) ) );

	setButtonGuiItem( User2, KGuiItem( i18n( "Download New Molecules" ), "get-hot-new-stuff", i18n( "Download new molecule files" ) ) );
	
	ui.setupUi(mainWidget());

	ui.qualityCombo->setCurrentIndex(2); //default to high quality
	
	//default to atom symbols
	ui.labelsCombo->setCurrentIndex(2);
	ui.glWidget->setLabels(2);
	
	//default to balls-and-sticks
	ui.styleCombo->setCurrentIndex(0); 
	ui.glWidget->setStyle(0);
	
	m_path = QString( "" );
	
	m_helpWindow = NULL;

	connect( ui.qualityCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( setQuality( int ) ) );
	connect( ui.styleCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( setStyle( int ) ) );
	connect( ui.labelsCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( setLabels( int ) ) );
	connect( this, SIGNAL( helpClicked() ), 
			this, SLOT( slotHelp() ) );

	connect( this, SIGNAL( user1Clicked() ), 
			this, SLOT( slotLoadMolecule() ) );

	connect( this, SIGNAL( user2Clicked() ), 
			this, SLOT( slotDownloadNewStuff() ) );

	// Check that we have managed to load up some tools and engines
	int nEngines = ui.glWidget->engines().size() - 1;
        int nTools = ui.glWidget->toolGroup()->tools().size();
	QString error;
        if(!nEngines && !nTools)
          error = i18n("No tools or engines loaded - it is likely that the Avogadro plugins could not be located.");
        else if(!nEngines)
          error = i18n("No engines loaded - it is likely that the Avogadro plugins could not be located.");
        else if(!nTools)
          error = i18n("No tools loaded - it is likely that the Avogadro plugins could not be located.");
        /// FIXME: QMessageBox used for now - KMessageBox was causing freezes
        if(!nEngines || !nTools)
          QMessageBox::warning(this, i18n("Kalzium"), error);
}

void MoleculeDialog::slotHelp()
{
	if( m_helpWindow == NULL )
	{
	    m_helpWindow = new KDialog( this );
	    m_helpWindow->setMinimumSize( 500, 300 );
	    m_helpWindow->showButton( KDialog::Cancel, false );
	    KTextBrowser *helpText = new KTextBrowser;
	    helpText->setHtml( i18nc(
		"Help text for the molecular viewer",
		"<b>Rotate View</b><br/>"
		"Click and hold the right mouse button, and drag the mouse to view the molecule from a different perspective. "
		"You can also rotate the view around an atom. To do so, position your "
		"mouse pointer over the atom before you click the right mouse button.<br/>"
		"<br>"
		"<b>Zoom View</b><br/>"
		"To get a closer look, either simply use your scrool wheel, or click and "
		"hold the middle mouse button to be able to control the zoom more precisely. "
		"Then drag your mouse to zoom. If you want to zoom in to or out from a specific "
		"point in the molecule, point at it first, and then zoom. "
		"If you are using a touchpad, the middle mouse button can be simulated by pressing the "
		"left <i>and</i> the right mouse button at the same time.<br/>"
		"<br/>"
		"<b>Display Options</b><br/>"
		"The options are mostly self-explaining. Changing the quality will either use less "
		"or more polygons to render the molecule. If you wonder what the \"Van der Waals\" "
		"style does, take a look at this article: <a href=\"http://en.wikipedia.org/wiki/Van_der_Waals_radius\">Van der Waals radius</a>.<br/>"
		"<br/>"
		"<b>Getting more molecules</b><br/>"
		"It is easy to obtain more molecules for this viewer, as it supports various common file "
		"formats. Here's a list of good public databanks:"
		"<ul>"
		"<li><a href=\"http://www.rcsb.org/pdb\">The Protein Data Bank (PDB)</a> "
		"<i>(For starters, we suggest viewing <a href=\"http://www.rcsb.org/pdbstatic/tutorials/tutorial.html\">this</a> tutorial</i>)</li>"
		"<li><a href=\"http://www.ncbi.nlm.nih.gov/Structure\">The National Center for Biotechnology Information</a></li>"
		"<li><a href=\"http://www.dspace.cam.ac.uk/handle/1810/724\">The World Wide Molecular Matrix (WWMM)</a></li>"
		"</ul>"
	    ) );
	    helpText->setOpenExternalLinks( true );
	    m_helpWindow->setMainWidget( helpText );
	    m_helpWindow->setCaption( i18nc( "Window title for the molecular 3D viewer's help", "Molecular Viewer Help" ) );
	    m_helpWindow->show();
	}
	else
	{
		if( !m_helpWindow->isVisible() )
		{
		    m_helpWindow->show();
		}
		m_helpWindow->activateWindow();
	}
}
	
void MoleculeDialog::slotLoadMolecule()
{
	// Check that we have managed to load up some tools and engines
	int nEngines = ui.glWidget->engines().size() - 1;
        int nTools = ui.glWidget->toolGroup()->tools().size();
	QString error;
        if(!nEngines && !nTools)
          error = i18n("No tools or engines loaded - it is likely that the Avogadro plugins could not be located. No molecules can be viewed until this issue is resolved.");
        else if(!nEngines)
          error = i18n("No engines loaded - it is likely that the Avogadro plugins could not be located. No molecules can be viewed until this issue is resolved.");
        else if(!nTools)
          error = i18n("No tools loaded - it is likely that the Avogadro plugins could not be located. No molecules can be viewed until this issue is resolved.");
        if(!nEngines || !nTools)
          KMessageBox::information(this, error);

        m_path = KGlobal::dirs()->findResourceDir( "appdata", "data/molecules/" ) + "data/molecules/";

	QString commonMoleculeFormats = i18n( "Common molecule formats" );
	QString allFiles = i18n( "All files" );

	QString filename = KFileDialog::getOpenFileName( 
			m_path,
			"*.cml *.xyz *.ent *.pdb *.alc *.chm *.cdx *.cdxml *.c3d1 *.c3d2"
			" *.gpr *.mdl *.mol *.sdf *.sd *.crk3d *.cht *.dmol *.bgf"
			" *.gam *.inp *.gamin *.gamout *.tmol *.fract"
			" *.mpd *.mol2|"+commonMoleculeFormats+"\n"
			"* *.*|"+allFiles,
			this,
			i18n( "Choose a file to open" ) );

	if( filename.isEmpty() ) return;

	kDebug() << "Filename to load: " << filename;

	Avogadro::Molecule* molecule = OpenBabel2Wrapper::readMolecule( filename );

	if (molecule->NumAtoms() != 0)
	{
		molecule->Center();
		ui.glWidget->setMolecule( molecule );
		ui.glWidget->update();
		updateStatistics();
	}
}

MoleculeDialog::~MoleculeDialog( )
{
}

void MoleculeDialog::updateStatistics()
{
	Avogadro::Molecule* mol = ui.glWidget->molecule();
	if ( !mol ) return;

	ui.nameLabel->setText( mol->GetTitle() );
	ui.weightLabel->setText( i18nc( "This 'u' stands for the chemical unit (u for 'units'). Most likely this does not need to be translated at all!", "%1 u", mol->GetMolWt() ) );
	ui.formulaLabel->setText( OpenBabel2Wrapper::getPrettyFormula( mol ) );
	ui.glWidget->update();
}

void MoleculeDialog::slotDownloadNewStuff()
{
    kDebug() << "Kalzium new stuff";
    KNS::Entry::List entries = KNS::Engine::download();
    // list of changed entries
    foreach(KNS::Entry::Entry* entry, entries) {
        // care only about installed ones
        if (entry->status() == KNS::Entry::Installed) {
            kDebug() << "Files downloaded: " << entry->installedFiles();
        }
    }
    qDeleteAll(entries);
}

#include "moleculeview.moc"
