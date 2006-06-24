/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include "latticedialog.h"

#include <QFileInfo>
#include <QLabel>
#include <QDir>
#include <QLayout>

#include <kdebug.h>
#include <kcombobox.h>
#include <kglobal.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include "kalziumglwidget.h"
#include "openbabel2wrapper.h"

#include <math.h>

LatticeDialog::LatticeDialog( QWidget* parent )
	: KDialog( parent )
{
	setCaption( i18n( "Simple Crystal Viewer" ) );
	setButtons( Close );
	setDefaultButton( Close );

	m_unitCell = new OpenBabel::OBUnitCell();
	m_molecule = OpenBabel2Wrapper::readMolecule( "/home/kde4/test.cml" );

	QWidget *page = new QWidget( this );
	setMainWidget( page );

	ui.setupUi( page );

	connect( ui.quality, SIGNAL( activated( int ) ), 
			ui.glWidget, SLOT( slotSetDetail( int ) ) );
	
	connect( ui.lattice, SIGNAL( activated( int ) ), 
			this, SLOT( slotSetLattice( int ) ) );

	connect( ui.updateButton, SIGNAL( clicked() ), 
			this, SLOT( slotUpdateView() ) );
	
	slotUpdateView();
}

void LatticeDialog::slotUpdateView()
{
	//this line crashes 
	double volume = m_unitCell->GetCellVolume();
	ui.volumeLabel->setText( QString::number( volume ) );
	ui.glWidget->slotSetMolecule( m_molecule );
//	ui.glWidget->ChooseStylePreset( KalziumGLWidget::PRESET_LINES  );
	ui.glWidget->update();
}

void LatticeDialog::slotSetLattice( int lattice )
{
	OpenBabel::OBUnitCell::LatticeType lt = OpenBabel::OBUnitCell::Undefined;

	//OpenBabel defines as member 0 of the enum LatticeType "Undefined", that
	//is why we need to add +1
	lt = ( OpenBabel::OBUnitCell::LatticeType ) ( lattice+1 );
	m_unitCell->SetLatticeType( lt );

	//as the crystal system changed we now also need 
	//an update of the GUI
	slotUpdateView();
}

#include "latticedialog.moc"

