/***************************************************************************
 *  Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
 *  Copyright (C) 2007-2008 by Marcus D. Hanwell <marcus@cryos.org>
 *  Copyright (C) 2016 by Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include <avogadro/qtgui/elementtranslator.h>
#include <avogadro/qtgui/periodictableview.h>
#include <avogadro/qtgui/molecule.h>
#include <avogadro/qtgui/scenepluginmodel.h>
// #include <avogadro/qtplugins/openbabel/openbabel.h>FIXME:Avogadro2

#include <QFileInfo>
#include <QGLFormat>
#include <QSettings>
#include <QDebug>
#include <kfiledialog.h>
#include <kjob.h>
#include <kmessagebox.h>
#include <KLocale>
#include <QUrl>
#include <knewstuff3/downloaddialog.h>
#include <kio/job.h>

#include "iowrapper.h"

#include <openbabel/mol.h>
#include <openbabel/obiter.h>
// This is needed to ensure that the forcefields are set up right with GCC vis
#ifdef __KDE_HAVE_GCC_VISIBILITY
  #define HAVE_GCC_VISIBILITY
#endif
#include <openbabel/forcefield.h>
#include <QStandardPaths>

using namespace OpenBabel;
using namespace Avogadro::QtGui;

MoleculeDialog::MoleculeDialog(QWidget * parent)
    : KDialog(parent)
    , m_path(QString())
    , m_periodicTable(nullptr)
    , m_addHydrogens(false)
{
    // use multi-sample (anti-aliased) OpenGL if available
    QGLFormat defFormat = QGLFormat::defaultFormat();
    defFormat.setSampleBuffers(true);
    QGLFormat::setDefaultFormat(defFormat);

    setCaption(i18n("Molecular Editor"));
    setButtons(User3 | User2 | User1 | Close);

    setDefaultButton(User1);

    setButtonGuiItem(User1, KGuiItem(i18n("Load Molecule"), "document-open", i18n("Loading a molecule")));

    setButtonGuiItem(User2, KGuiItem(i18n("Download New Molecules"), "get-hot-new-stuff", i18n("Download new molecule files")));

    setButtonGuiItem(User3, KGuiItem(i18n("Save Molecule"), "document-save", i18n("Saving a molecule")));

    ui.setupUi(mainWidget());

    //default to atom symbols
    ui.labelsCombo->setCurrentIndex(1);
    ui.glWidget->setLabels(1);

    // Attempt to set up the UFF forcefield
    m_forceField = OBForceField::FindForceField("UFF");
    if (!m_forceField) {
        ui.optimizeButton->setEnabled(false);
    }

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

    ui.styleCombo->setModel(
        qobject_cast<QAbstractItemModel*>(&ui.glWidget->sceneModel()));

    connect(ui.tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(setViewEdit(int)));

    connect(ui.labelsCombo, SIGNAL(activated(int)),
            ui.glWidget, SLOT(setLabels(int)));

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
    connect(ui.clearDrawingButton, SIGNAL(clicked()),
            this, SLOT(clearAllElementsInEditor()));

    connect(ui.glWidget->molecule(), &Avogadro::QtGui::Molecule::changed,
            this, &MoleculeDialog::slotUpdateStatistics);

    connect(this, SIGNAL(user1Clicked()),
            this, SLOT(slotLoadMolecule()));
    connect(this, SIGNAL(user2Clicked()),
            this, SLOT(slotDownloadNewStuff()));
    connect(this, SIGNAL(user3Clicked()),
            this, SLOT(slotSaveMolecule()));

    // Check that we have managed to load up some tools and engines
    int nTools = ui.glWidget->tools().size();
    if (!nTools) {
        QString error = i18n("No tools loaded - it is likely that the Avogadro plugins could not be located.");
        KMessageBox::error(this, error, i18n("Kalzium"));
    }
}

void MoleculeDialog::slotLoadMolecule()
{
    // Check that we have managed to load up some tools and engines
    int nTools = ui.glWidget->tools().size();


    if (!nTools) {
        QString error = i18n("No tools loaded - it is likely that the Avogadro plugins could not be located. "
            "No molecules can be viewed until this issue is resolved.");
        KMessageBox::information(this, error);
    }

    m_path = QStandardPaths::locate(QStandardPaths::DataLocation, "data/molecules/") + "data/molecules/";
    m_path = QFileInfo(m_path).absolutePath();

    QString commonMoleculeFormats = i18n("Common molecule formats");
    QString allFiles = i18n("All files");

    QString filename = KFileDialog::getOpenFileName(
                       QUrl::fromLocalFile(m_path),
                       "*.cml *.xyz *.ent *.pdb *.alc *.chm *.cdx *.cdxml *.c3d1 *.c3d2"
                       " *.gpr *.mdl *.mol *.sdf *.sd *.crk3d *.cht *.dmol *.bgf"
                       " *.gam *.inp *.gamin *.gamout *.tmol *.fract"
                       " *.mpd *.mol2|" + commonMoleculeFormats + "\n"
                       "* *.*|" + allFiles,
                       this,
                       i18n("Choose a file to open"));

    loadMolecule(filename);
}

void MoleculeDialog::loadMolecule(const QString &filename)
{
    if (filename.isEmpty()) {
        return;
    }

    qDebug() << "Filename to load: " << filename;

    auto tmpMol = IoWrapper::readMolecule(filename);
    auto molecule = new Avogadro::QtGui::Molecule(*tmpMol);
    delete tmpMol;

    // Check that a valid molecule object was returned
    if (!molecule) {
        qCritical() << "Could not load molecule, aborting.";
        return;
    }

    if (molecule->atomCount() != 0) {
        disconnect(ui.glWidget->molecule(), 0, this, 0);
        ui.glWidget->setMolecule(molecule);
        ui.glWidget->update();
        slotUpdateStatistics();
        connect(molecule, &Avogadro::QtGui::Molecule::changed,
                this, &MoleculeDialog::slotUpdateStatistics);
    }
    ui.glWidget->resetCamera();
    ui.glWidget->update();
}

void MoleculeDialog::clearAllElementsInEditor()
{
    ui.glWidget->molecule()->clearBonds();
    ui.glWidget->molecule()->clearAtoms();
    ui.glWidget->update();
}

void MoleculeDialog::slotSaveMolecule()
{
    QString commonMoleculeFormats = i18n("Common molecule formats");
    QString allFiles = i18n("All files");
    QString filename = KFileDialog::getSaveFileName(QUrl(),
                       "*.cml *.xyz *.ent *.pdb *.alc *.chm *.cdx *.cdxml *.c3d1 *.c3d2"
                       " *.gpr *.mdl *.mol *.sdf *.sd *.crk3d *.cht *.dmol *.bgf"
                       " *.gam *.inp *.gamin *.gamout *.tmol *.fract"
                       " *.mpd *.mol2|" + commonMoleculeFormats + "\n"
                       "* *.*|" + allFiles,
                       this,
                       i18n("Choose a file to save to"));

    if (!filename.contains('.')) {
        filename.append(".cml");
    }

   IoWrapper io;
   io.writeMolecule(filename, ui.glWidget->molecule());
}

void MoleculeDialog::setViewEdit(int mode)
{
    if (mode == 0) {
        ui.glWidget->setActiveTool("Navigator");
    } else if (mode == 1) {
        ui.glWidget->setActiveTool("Editor");
        if (ui.glWidget->activeTool()) {
//             ui.glWidget->activeTool()->readSettings(*m_drawSettings);//FIXME Avogadro2: settings
        }
    } else if (mode == 2) {
        ui.glWidget->setActiveTool("MeasureTool");
    }
}

MoleculeDialog::~MoleculeDialog()
{
    delete m_drawSettings;
}

void MoleculeDialog::slotUpdateStatistics()
{
    Molecule* mol = ui.glWidget->molecule();
    if (!mol) {
        return;
    }
    const std::string name = mol->data(QString("name").toStdString()).toString();
    ui.nameLabel->setText(QString::fromStdString(name));
    ui.weightLabel->setText(i18nc("This 'u' stands for the chemical unit (u for 'units'). Most likely this does not need to be translated at all!", "%1 u", mol->mass()));
    ui.formulaLabel->setText(IoWrapper::getPrettyFormula(mol));
    ui.glWidget->update();
}

void MoleculeDialog::slotDownloadNewStuff()
{
    qDebug() << "Kalzium new stuff";

    KNS3::DownloadDialog dialog(this);
    dialog.exec();
    // list of changed entries
    QString destinationDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir dir(destinationDir);
    if (!dir.exists()) {
        destinationDir = QDir::homePath();
    }
    bool anyError = false;
    bool anySuccess = false;
    bool moreOneInstalledFile = false;
    QString exactlyOneFile;
    foreach (const KNS3::Entry& entry, dialog.changedEntries()) {
        // care only about installed ones
        if (entry.status() == KNS3::Entry::Installed) {
            qDebug() << "Changed Entry: " << entry.installedFiles();
            foreach (const QString &origFile, entry.installedFiles()) {
                const QString destFile = destinationDir + '/' + QFileInfo(origFile).fileName();
                KJob *job;// = KIO::file_move(QUrl::fromLocalFile(origFile), QUrl::fromPath(destFile));//TODO:KF5
                const bool success = job->exec();
                if (success) {
                    if (exactlyOneFile.isEmpty()) {
                        exactlyOneFile = destFile;
                    } else {
                        moreOneInstalledFile = true;
                    }
                    anySuccess = true;
                } else {
                    KMessageBox::error(this, i18n("Failed to download molecule %1 to %2.", entry.name(), destFile));
                    anyError = true;
                }
            }
        }
    }
    if (anySuccess) {
        if (anyError) {
            KMessageBox::information(this, i18n("The molecules that could be downloaded have been saved to %1.", destinationDir));
        } else {
            KMessageBox::information(this, i18n("The molecules have been saved to %1.", destinationDir));
        }
        if (!moreOneInstalledFile) {
            loadMolecule(exactlyOneFile);
        }
    }
}

void MoleculeDialog::elementCombo()
{
    ui.elementCombo->addItem(ElementTranslator::name(1) + " (1)");
    m_elementsIndex.append(1);
    ui.elementCombo->addItem(ElementTranslator::name(5) + " (5)");
    m_elementsIndex.append(5);
    ui.elementCombo->addItem(ElementTranslator::name(6) + " (6)");
    m_elementsIndex.append(6);
    ui.elementCombo->addItem(ElementTranslator::name(7) + " (7)");
    m_elementsIndex.append(7);
    ui.elementCombo->addItem(ElementTranslator::name(8) + " (8)");
    m_elementsIndex.append(8);
    ui.elementCombo->addItem(ElementTranslator::name(9) + " (9)");
    m_elementsIndex.append(9);
    ui.elementCombo->addItem(ElementTranslator::name(15) + " (15)");
    m_elementsIndex.append(15);
    ui.elementCombo->addItem(ElementTranslator::name(16) + " (16)");
    m_elementsIndex.append(16);
    ui.elementCombo->addItem(ElementTranslator::name(17) + " (17)");
    m_elementsIndex.append(17);
    ui.elementCombo->addItem(ElementTranslator::name(35) + " (35)");
    m_elementsIndex.append(35);
    ui.elementCombo->addItem(i18nc("Other element", "Other..."));
    m_elementsIndex.append(0);
    ui.elementCombo->setCurrentIndex(2);
}

void MoleculeDialog::slotElementChanged(int element)
{
    // Check if other was chosen
    if (m_elementsIndex[element] == 0) {
        if (!m_periodicTable) {
            m_periodicTable = new PeriodicTableView(this);
            connect(m_periodicTable, SIGNAL(elementChanged(int)),
                    this, SLOT(slotCustomElementChanged(int)));
        }
        m_periodicTable->show();
        return;
    }

    m_drawSettings->setValue("currentElement", m_elementsIndex[element]);
//     ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);//FIXME:Avogadro2
}

void MoleculeDialog::slotCustomElementChanged(int element)
{
    // Set the element so we can draw with it
    m_drawSettings->setValue("currentElement", element);
    if (ui.glWidget->activeTool()) {
//         ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);//FIXME:Avogadro2
    }

    // Check to see if we already have this in the comboBox list
    // If not, we get back -1 and need to create a new item
    int comboItem = m_elementsIndex.indexOf(element);
    if (comboItem != -1) {
        ui.elementCombo->setCurrentIndex(comboItem);
        return; // we found it in the list, so we're done
    }

    // Find where we should put the new entry
    // (i.e., in order by atomic number)
    int position = 0;
    foreach (int entry, m_elementsIndex) {
        // Two cases: entry > index -- insert the new element before this one
        // Or... we hit the "Other" menu choice -- also insert here
        if (entry > element || entry == 0) {
            break;
        }

        ++position;
    }

    // And now we set up a new entry into the combo list
    QString entryName(ElementTranslator::name(element)); // (e.g., "Hydrogen")
    entryName += " (" + QString::number(element) + ')';

    m_elementsIndex.insert(position, element);
    ui.elementCombo->insertItem(position, entryName);
    ui.elementCombo->setCurrentIndex(position);
}

void MoleculeDialog::slotBondOrderChanged(int bond)
{
    m_drawSettings->setValue("bondOrder", bond+1);
    if (ui.glWidget->activeTool()) {
//         ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);//FIXME:Avogadro2
    }
}

void MoleculeDialog::slotAddHydrogensChanged(int hydrogens)
{
    m_drawSettings->setValue("addHydrogens", hydrogens);
    if (ui.glWidget->activeTool()) {
//         ui.glWidget->toolGroup()->activeTool()->readSettings(*m_drawSettings);//FIXME:Avogadro2
    }
}

void MoleculeDialog::slotAdjustHydrogens()
{
    // Add/remove hydrogens from the molecule
    if (!m_addHydrogens) {
        ui.hydrogensButton->setText(i18n("Remove hydrogens"));
//         ui.glWidget->molecule()->addHydrogens();//FIXME:Avogadro2
        m_addHydrogens = true;
    } else {
        ui.hydrogensButton->setText(i18n("Add hydrogens"));
//         ui.glWidget->molecule()->removeHydrogens();//FIXME:Avogadro2
        m_addHydrogens = false;
    }
//     ui.glWidget->molecule()->update();//FIXME:Avogadro2
}

void MoleculeDialog::slotGeometryOptimize()
{
    // Perform a geometry optimization
    if (!m_forceField) {
        return;
    }

    Molecule* molecule = ui.glWidget->molecule();
    OpenBabel::OBMol obmol;//(molecule->OBMol());//FIXME:Avogadro2

    // Warn the user if the force field cannot be set up for the molecule
    if (!m_forceField->Setup(obmol)) {
        KMessageBox::error(this,
                        i18n("Could not set up force field for this molecule"),
                        i18n("Kalzium"));
        return;
    }

    // Reasonable default values for most users
    m_forceField->SteepestDescentInitialize(500, 1.0e-5);
    // Provide some feedback as the optimization runs
//     while (m_forceField->SteepestDescentTakeNSteps(5)) {//FIXME:Avogadro2
//         m_forceField->UpdateCoordinates(obmol);
//         molecule->setOBMol(&obmol);
//         molecule->update();
//     }
}
