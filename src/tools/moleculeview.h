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

#ifndef MOLECULEVIEW_H
#define MOLECULEVIEW_H

#include <QDialog>
#include <avogadro/qtgui/molecule.h>

#include "ui_moleculeviewerwidget.h"

namespace OpenBabel
{
class OBForceField;
}
namespace Avogadro
{
namespace QtGui
{
class PeriodicTableView;
}
}
class QSettings;

/**
 * @author Carsten Niehaus
 */
class MoleculeDialog : public QDialog
{
    Q_OBJECT

public:
    MoleculeDialog(QWidget * parent);
    ~MoleculeDialog();

    void loadMolecule(const QString &filename);

private:
    QString m_path;///to store the path were the molecules are located
    QList<int> m_elementsIndex; // Index storing the element combo index
    Avogadro::QtGui::PeriodicTableView *m_periodicTable;
    OpenBabel::OBForceField* m_forceField;
    QSettings *m_drawSettings;

    Ui::moleculeViewerForm ui;

private slots:
    /**
     * Load a molecule
     */
    void slotLoadMolecule();

    /**
     * Update the currently active scene plugin.
     */
    void slotUpdateScenePlugin();

    /**
     * Get a new molecule using hot new stuff
     */
    void slotDownloadNewStuff();

    /**
    * Save a molecule
    */
    void slotSaveMolecule();

    /**
    * Set view/edit mode
    */
    void setViewEdit(int mode);

    /**
    * Update the statistical information about the current molecule
    */
    void slotUpdateStatistics();

    /**
    * Geometry optimization
    */
    void slotGeometryOptimize();

    /// Clears the view
    void clearAllElementsInEditor();

private:
    // workaround for broken copy-operator of QtGui::Molecule
    // would be removed after next Avogadro release greater 0.9
    Avogadro::QtGui::Molecule m_molecule;
};

#endif // MOLECULEVIEW_H
