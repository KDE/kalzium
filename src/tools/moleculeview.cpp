/***************************************************************************
 *  Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
 *  Copyright (C) 2007-2008 by Marcus D. Hanwell <marcus@cryos.org>
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
#include "../../libavogadro-kalzium/src/elementtranslate.h"
#include "../../libavogadro-kalzium/src/periodictableview.h"

#include <QGLFormat>
#include <QUndoStack>
#include <QSettings>
#include <kdebug.h>
#include <kfiledialog.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <KLocale>
#include <KTextBrowser>
#include <knewstuff2/engine.h>

#include <openbabel2wrapper.h>

using namespace OpenBabel;
using namespace Avogadro;

// Handles localization/translation of element names
// e.g., nitrogen in English is azote in French
extern Avogadro::ElementTranslator Avogadro::elementTranslator;

MoleculeDialog::MoleculeDialog( QWidget * parent )
	: KDialog( parent ), m_periodicTable(0), m_addHydrogens(false)
{
	// use multi-sample (anti-aliased) OpenGL if available
	QGLFormat defFormat = QGLFormat::defaultFormat();
	defFormat.setSampleBuffers(true);
	QGLFormat::setDefaultFormat(defFormat);

	setCaption( i18n( "Molecular Editor" ) );
	setButtons( User3 | User2 | User1 | Close );

	setDefaultButton( User1 );

	setButtonGuiItem( User1, KGuiItem( i18n( "Load Molecule" ), "document-open", i18n( "Loading a molecule" ) ) );

	setButtonGuiItem( User2, KGuiItem( i18n( "Download New Molecules" ), "get-hot-new-stuff", i18n( "Download new molecule files" ) ) );

  setButtonGuiItem( User3, KGuiItem( i18n( "Save Molecule" ), "document-save", i18n( "Saving a molecule" ) ) );
	
	ui.setupUi(mainWidget());

	ui.qualityCombo->setCurrentIndex(2); //default to high quality
	
	//default to atom symbols
	ui.labelsCombo->setCurrentIndex(2);
	ui.glWidget->setLabels(2);
	
	//default to balls-and-sticks
	ui.styleCombo->setCurrentIndex(0); 
	ui.glWidget->setStyle(0);
	
	m_path = QString( "" );

  // Attempt to set up the UFF forcefield
  m_forceField = OBForceField::FindForceField("UFF");
  if (!m_forceField)
    ui.optimizeButton->setEnabled(false);

  // Need the undo stack even though we aren't using it just yet - atom deletion
  // doesn't work without it...
  QUndoStack* tmp = new QUndoStack(this);
  ui.glWidget->setUndoStack(tmp);

  // Set up the elements combo and bond order combo
  elementCombo();
  ui.bondCombo->addItem(i18n("Single"));
  ui.bondCombo->addItem(i18n("Double"));
  ui.bondCombo->addItem(i18n("Triple"));
  // Initialise the draw settings
  m_drawSettings = new QSettings;
  m_drawSettings->setValue("currentElement", 6);
  m_drawSettings->setValue("bondOrder", 1);
  m_drawSettings->setValue("addHydrogens", 0);
	

  connect(ui.tabWidget, SIGNAL(currentChanged(int)),
      this, SLOT(setViewEdit(int)));

  // Visualisation parameters
	connect( ui.qualityCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( setQuality( int ) ) );
	connect( ui.styleCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( setStyle( int ) ) );
  connect( ui.style2Combo, SIGNAL(activated( int )),
      ui.glWidget , SLOT( setStyle2( int ) ) );
	connect( ui.labelsCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( setLabels( int ) ) );

  // Editing parameters
  connect(ui.elementCombo, SIGNAL(currentIndexChanged(int)),
      this, SLOT(slotElementChanged(int)));
  connect(ui.bondCombo, SIGNAL(currentIndexChanged(int)),
      this, SLOT(slotBondOrderChanged(int)));
  connect(ui.hydrogenBox, SIGNAL(stateChanged(int)),
      this, SLOT(slotAddHydrogensChanged(int)));
  connect(ui.hydrogensButton, SIGNAL(clicked()),
      this, SLOT(slotAdjustHydrogens()));
  connect(ui.optimizeButton, SIGNAL(clicked()),
      this, SLOT(slotGeometryOptimize()));
      
  connect(ui.glWidget->molecule(), SIGNAL(updated()),
      this, SLOT(slotUpdateStatistics()));

	connect( this, SIGNAL( user1Clicked() ), 
			this, SLOT( slotLoadMolecule() ) );
	connect( this, SIGNAL( user2Clicked() ), 
			this, SLOT( slotDownloadNewStuff() ) );
  connect( this, SIGNAL( user3Clicked() ),
      this, SLOT( slotSaveMolecule() ) );

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
  if(!nEngines || !nTools)
    KMessageBox::error(this, error, i18n("Kalzium"));
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

  m_path = KGlobal::dirs()->findResourceDir( "appdata", "data/molecules/" ) +
    "data/molecules/";

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

  // Check that a valid molecule object was returned
  if (!molecule)
    return;

	if (molecule->NumAtoms() != 0)
	{
    disconnect(ui.glWidget->molecule(), 0, this, 0);
		molecule->Center();
		ui.glWidget->setMolecule( molecule );
		ui.glWidget->update();
		slotUpdateStatistics();
    connect(molecule, SIGNAL(updated()), this, SLOT(slotUpdateStatistics()));
	}
}

void MoleculeDialog::slotSaveMolecule()
{
  KUrl url("kfiledialog:///kalzium");
  QString commonMoleculeFormats = i18n( "Common molecule formats" );
  QString allFiles = i18n( "All files" );
  QString filename = KFileDialog::getSaveFileName(url,
    "*.cml *.xyz *.ent *.pdb *.alc *.chm *.cdx *.cdxml *.c3d1 *.c3d2"
      " *.gpr *.mdl *.mol *.sdf *.sd *.crk3d *.cht *.dmol *.bgf"
      " *.gam *.inp *.gamin *.gamout *.tmol *.fract"
      " *.mpd *.mol2|"+commonMoleculeFormats+"\n"
      "* *.*|"+allFiles,
    this,
    i18n( "Choose a file to save to" ) );

  if(!filename.contains('.')) filename.append(".cml");

  OpenBabel2Wrapper::writeMolecule( filename, ui.glWidget->molecule() );
}

void MoleculeDialog::setViewEdit(int mode)
{
    if (mode == 0) ui.glWidget->toolGroup()->setActiveTool("Navigate");
    else if (mode == 1) {
      ui.glWidget->toolGroup()->setActiveTool("Draw");
      ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);
    }
    else if (mode == 2) ui.glWidget->toolGroup()->setActiveTool("Measure");
}

MoleculeDialog::~MoleculeDialog( )
{
    delete m_drawSettings;
}

void MoleculeDialog::slotUpdateStatistics()
{
	Avogadro::Molecule* mol = ui.glWidget->molecule();
	if ( !mol ) return;

	ui.nameLabel->setText( mol->GetTitle() );
	ui.weightLabel->setText( i18nc( "This 'u' stands for the chemical unit (u for 'units'). Most likely this does not need to be translated at all!", "%1 u", mol->GetMolWt() ) );
	ui.formulaLabel->setText( OpenBabel2Wrapper::getPrettyFormula( mol ) );
//	ui.glWidget->update();
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

void MoleculeDialog::elementCombo()
{
  ui.elementCombo->addItem(elementTranslator.name(1) + " (1)");
  m_elementsIndex.append(1);
  ui.elementCombo->addItem(elementTranslator.name(5) + " (5)");
  m_elementsIndex.append(5);
  ui.elementCombo->addItem(elementTranslator.name(6) + " (6)");
  m_elementsIndex.append(6);
  ui.elementCombo->addItem(elementTranslator.name(7) + " (7)");
  m_elementsIndex.append(7);
  ui.elementCombo->addItem(elementTranslator.name(8) + " (8)");
  m_elementsIndex.append(8);
  ui.elementCombo->addItem(elementTranslator.name(9) + " (9)");
  m_elementsIndex.append(9);
  ui.elementCombo->addItem(elementTranslator.name(15) + " (15)");
  m_elementsIndex.append(15);
  ui.elementCombo->addItem(elementTranslator.name(16) + " (16)");
  m_elementsIndex.append(16);
  ui.elementCombo->addItem(elementTranslator.name(17) + " (17)");
  m_elementsIndex.append(17);
  ui.elementCombo->addItem(elementTranslator.name(35) + " (35)");
  m_elementsIndex.append(35);
  ui.elementCombo->addItem(tr("Other..."));
  m_elementsIndex.append(0);
  ui.elementCombo->setCurrentIndex(2);
}

void MoleculeDialog::slotElementChanged(int element)
{
  // Check if other was chosen
  if (m_elementsIndex[element] == 0)
  {
    if (!m_periodicTable)
    {
      m_periodicTable = new PeriodicTableView(this);
      connect(m_periodicTable, SIGNAL(elementChanged(int)),
              this, SLOT(slotCustomElementChanged(int)));
    }
    m_periodicTable->show();
    return;
  }

  m_drawSettings->setValue("currentElement", m_elementsIndex[element]);
  ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);
}

void MoleculeDialog::slotCustomElementChanged(int element)
{
  // Set the element so we can draw with it
  m_drawSettings->setValue("currentElement", element);
  ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);

  // Check to see if we already have this in the comboBox list
  // If not, we get back -1 and need to create a new item
  int comboItem = m_elementsIndex.indexOf(element);
  if (comboItem != -1)
  {
    ui.elementCombo->setCurrentIndex(comboItem);
    return; // we found it in the list, so we're done
  }

  // Find where we should put the new entry
  // (i.e., in order by atomic number)
  int position = 0;
  foreach(int entry, m_elementsIndex)
  {
    // Two cases: entry > index -- insert the new element before this one
    // Or... we hit the "Other" menu choice -- also insert here
    if (entry > element || entry == 0)
      break;

    ++position;
  }

  // And now we set up a new entry into the combo list
  QString entryName(elementTranslator.name(element)); // (e.g., "Hydrogen")
  entryName += " (" + QString::number(element) + ')';

  m_elementsIndex.insert(position, element);
  ui.elementCombo->insertItem(position, entryName);
  ui.elementCombo->setCurrentIndex(position);
}

void MoleculeDialog::slotBondOrderChanged(int bond)
{
  m_drawSettings->setValue("bondOrder", bond+1);
  ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);
}

void MoleculeDialog::slotAddHydrogensChanged(int hydrogens)
{
  m_drawSettings->setValue("addHydrogens", hydrogens);
  ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);
}

void MoleculeDialog::slotAdjustHydrogens()
{
  // Add/remove hydrogens from the molecule
  if (!m_addHydrogens) {
    ui.hydrogensButton->setText(i18n("Remove hydrogens"));
    ui.glWidget->molecule()->AddHydrogens(false, false);
    m_addHydrogens = true;
  }
  else {
    ui.hydrogensButton->setText(i18n("Add hydrogens"));
    ui.glWidget->molecule()->DeleteHydrogens();
    m_addHydrogens = false;
  }
  ui.glWidget->molecule()->update();
}

void MoleculeDialog::slotGeometryOptimize()
{
  // Perform a geometry optimisation
  if (!m_forceField)
    return;

  Avogadro::Molecule* molecule = ui.glWidget->molecule();

  // Warn the user if the force field cannot be set up for the molecule
  if (!m_forceField->Setup(*molecule))
  {
    KMessageBox::error(this, i18n("Kalzium"),
      i18n("Could not set up force field for this molecule"));
    return;
  }

  // Reasonable default values for most users
  m_forceField->SteepestDescentInitialize(500, 1.0e-5);
  // Provide some feedback as the optimisation runs
  while (m_forceField->SteepestDescentTakeNSteps(5))
  {
    m_forceField->UpdateCoordinates(*molecule);
    molecule->update();
  }
}

#include "moleculeview.moc"
