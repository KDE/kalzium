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

#include <openbabel/mol.h>
#include <openbabel/obiter.h>

/// This is needed to ensure that the forcefields are set up right with GCC vis
#ifdef __KDE_HAVE_GCC_VISIBILITY
  #define HAVE_GCC_VISIBILITY
#endif
#include <openbabel/forcefield.h>

namespace Avogadro{
  class PeriodicTableView;
}

/**
 * @author Carsten Niehaus
 */
class MoleculeDialog : public KDialog
{
	Q_OBJECT

	public:
		MoleculeDialog( QWidget  * parent );
		~MoleculeDialog();

	private:
		QString  m_path;///to store the path were the molecules are located
    QList<int> m_elementsIndex; // Index storing the element combo index
    Avogadro::PeriodicTableView *m_periodicTable;
    OpenBabel::OBForceField* m_forceField;
		KDialog *m_helpWindow;

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
     * Set view/edit mode
     */
    void setViewEdit(int mode);

		void slotHelp();

    /**
     * Current element has changed
     */
    void slotElementChanged(int element);

    /**
     * Custom element has been selected
     */
    void slotCustomElementChanged(int);

    /**
     * Update the statistical information about the current molecule
     */
    void slotUpdateStatistics();

    /**
     * Geometry optimisation
     */
     void slotGeometryOptimize();

};

#endif // MOLECULEVIEW_H
