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

	m_unitCell = 0;

	QWidget *page = new QWidget( this );
	setMainWidget( page );

	ui.setupUi( page );

	OpenBabel::OBMol * mol = OpenBabel2Wrapper::readMolecule( "/home/kde4/test.cml" );
	ui.glWidget->slotSetMolecule(mol);

	connect( ui.quality, SIGNAL( activated( int ) ), 
			ui.glWidget, SLOT( slotSetDetail( int ) ) );
	
	connect( ui.lattice, SIGNAL( activated( int ) ), 
			this, SLOT( slotSetLattice( int ) ) );

	connect( ui.updateButton, SIGNAL( clicked() ), 
			this, SLOT( slotUpdateView() ) );
}

void LatticeDialog::slotUpdateView()
{
}

void LatticeDialog::slotSetLattice( int lattice )
{
	ui.va->setReadOnly( false );
	ui.vb->setReadOnly( false );
	ui.vc->setReadOnly( false );
	ui.aa->setReadOnly( false );
	ui.ab->setReadOnly( false );
	ui.ac->setReadOnly( false );

	switch ( ui.lattice->currentIndex() )
	{
		case 0://triclinic all angles != 90
			break;
		case 1://monooclinic alpha != 90, beta, gamma = 90
			ui.ab->setValue( 90.0 );
			ui.ac->setValue( 90.0 );
			ui.ab->setReadOnly( true );
			ui.ac->setReadOnly( true );
			break;
		case 2://Orthorhombic
			break;
		case 3://Tetragonal A=B!=C, angles = 90°
			ui.vb->setReadOnly( true );
			ui.aa->setValue( 90.0 );
			ui.ab->setValue( 90.0 );
			ui.ac->setValue( 90.0 );
			ui.aa->setReadOnly( true );
			ui.ab->setReadOnly( true );
			ui.ac->setReadOnly( true );
			break;
		case 4://Rhombohedral
			break;
		case 5://Hexagonal
			break;
		case 6://Cubic
			break;
	}

	m_lattice = ( LATTICE )lattice;
}

double LatticeDialog::volume()
{
	const double a = ui.va->value();
	const double b = ui.vb->value();
	const double c = ui.vc->value();
	const double aa = ui.aa->value();
	const double ab = ui.ab->value();
	const double ac = ui.ac->value();

	double result = 0.0;
	
	switch ( ui.lattice->currentIndex() )
	{
		case 0://triclinic
			result = a * b * c * sqrt(1
						- pow(cos( aa ), 2)
						- pow(cos( ab ), 2)
						- pow(cos( ac ), 2)
						+ 2 * cos( aa ) * cos( ab ) * cos( ac )
					);
			break;
		case 1://monooclinic
			result = a*b*c*sin( ab );
			break;
		case 2://Orthorhombic
			result = a * b * c;
			break;
		case 3://Tetragonal
			result = a * a * c;
			break;
		case 4://Rhombohedral
			result = pow( a, 3 ) * sqrt(1
						- pow(cos( aa ), 2)
						- pow(cos( ab ), 2)
						- pow(cos( ac ), 2)
						+ 2 * cos( aa ) * cos( ab ) * cos( ac )
					);
			break;
		case 5://Hexagonal
			result = pow( 3, 1/3 ) * a * a * c / 2;
			break;
		case 6://Cubic
			result = pow( a, 3 );
			break;
	}

	return result;
}

#include "latticedialog.moc"

