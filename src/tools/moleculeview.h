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

#include <kdialog.h>

#include "ui_moleculeviewerwidget.h"

namespace OpenBabel
{
class OBForceField;
}
namespace Avogadro
{
class PeriodicTableView;
}
class QSettings;

/**
 * @author Carsten Niehaus
 */
class MoleculeDialog : public KDialog
{
    Q_OBJECT

public:
    MoleculeDialog(QWidget * parent);
    ~MoleculeDialog();

    void loadMolecule(const QString &filename);

private:
    QString m_path;///to store the path were the molecules are located
    QList<int> m_elementsIndex; // Index storing the element combo index
    Avogadro::PeriodicTableView *m_periodicTable;
    OpenBabel::OBForceField* m_forceField;
    QSettings *m_drawSettings;
    bool m_addHydrogens;

    Ui::moleculeViewerForm ui;

    /**
    * Set up the element combo box
    */
    void elementCombo();

private slots:
    /**
     * Load a molecule
     */
    void slotLoadMolecule();

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
    * Current element has changed
    */
    void slotElementChanged(int element);

    /**
    * Custom element has been selected
    */
    void slotCustomElementChanged(int);

    /**
    * Bond order has been changed
    */
    void slotBondOrderChanged(int);

    /**
    * Automatic hydrogen addition on drawing
    */
    void slotAddHydrogensChanged(int);

    /**
    * Update the statistical information about the current molecule
    */
    void slotUpdateStatistics();

    /**
    * Add/remove hydrogens
    */
    void slotAdjustHydrogens();

    /**
    * Geometry optimization
    */
    void slotGeometryOptimize();

    /// Clears the view
    void clearAllElementsInEditor();
};

#endif // MOLECULEVIEW_H
