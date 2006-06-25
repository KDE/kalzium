#ifndef LATTICEDIALOG_H
#define LATTICEDIALOG_H
/***************************************************************************
 *   Copyright (C) 2006 by Carsten Niehaus
 *   toscano.pino@tiscali.it                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#include <kdialog.h>

#include "ui_latticeviewer.h"

#include <openbabel/generic.h>

class KalziumGLWidget;

/**
 * @author Carsten Niehaus
 */
class LatticeDialog : public KDialog
{
	Q_OBJECT
	public:
		/**
		 * Constructor
		 */
		LatticeDialog( QWidget* parent );
		/**
		 * dtor
		 */
		~LatticeDialog(){};

	private slots:
		void slotSetLattice( int lattice );

		/**
		 * update the data of the unitcell
		 */
		void slotUpdateUnitcell();

		/**
		 * update all strings and items in the dialog
		 */
		void slotUpdateView();

	private:
		Ui_latticeView ui;

		///the unitcell displayed
		OpenBabel::OBUnitCell * m_unitCell;

		//this object stores the position of the atoms in the unitcell
		OpenBabel::OBMol * m_molecule;
};


#endif // LATTICEDIALOG_H
