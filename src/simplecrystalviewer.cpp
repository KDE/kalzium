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

#include "simplecrystalviewer.h"

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

SimpleCrystalViewer::SimpleCrystalViewer( QWidget* parent )
	: KDialog( parent )
{
	setCaption( i18n( "Simple Crystal Viewer" ) );
	setButtons( Close );
	setDefaultButton( Close );

	QWidget *page = new QWidget( this );
	setMainWidget( page );

	QHBoxLayout *hlay = new QHBoxLayout( page );

	QVBoxLayout *vlay = new QVBoxLayout();
	hlay->addLayout( vlay );
	vlay->setMargin( 0 );
	vlay->setSpacing( 5 );
	vlay->addItem( new QSpacerItem( 5, 40, QSizePolicy::Fixed, QSizePolicy::Fixed ) );

	vlay->addWidget( new QLabel( i18n( "Quality:" ), page ) );

	KComboBox *combo = new KComboBox( false, page );
	combo->addItem( "Low" );
	combo->addItem( "Medium" );
	combo->addItem( "High" );
	vlay->addWidget( combo );
	vlay->addItem( new QSpacerItem( 5, 5, QSizePolicy::Fixed, QSizePolicy::Expanding ) );

	m_glWidget = new KalziumGLWidget( page );
	OpenBabel::OBMol * mol = OpenBabel2Wrapper::readMolecule( "/home/kde4/test.cml" );
	m_glWidget->slotSetMolecule(mol);
	hlay->addWidget( m_glWidget );

	connect( combo, SIGNAL( activated( int ) ), m_glWidget, SLOT( slotSetDetail( int ) ) );
}

void SimpleCrystalViewer::slotCrystalChanged( const QString& which )
{
}

#include "simplecrystalviewer.moc"

