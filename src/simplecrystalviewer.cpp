/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
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

#include "animatedmoviewidget.h"

SimpleCrystalViewer::SimpleCrystalViewer( QWidget* parent )
	: KDialogBase( parent, "simple-crystal-viewer", true, i18n( "Simple Crystal Viewer" ), Close, Close, true )
{
	QWidget *page = new QWidget( this );
	setMainWidget( page );

	QHBoxLayout *hlay = new QHBoxLayout( page );

	QVBoxLayout *vlay = new QVBoxLayout( hlay );
	vlay->setMargin( 0 );
	vlay->setSpacing( 5 );
	hlay->addLayout( vlay );
	vlay->addItem( new QSpacerItem( 5, 40, QSizePolicy::Fixed, QSizePolicy::Fixed ) );

	vlay->addWidget( new QLabel( i18n( "Crystal structure:" ), page ) );

	KComboBox *combo = new KComboBox( false, page );
	vlay->addWidget( combo );
	vlay->addItem( new QSpacerItem( 5, 5, QSizePolicy::Fixed, QSizePolicy::Expanding ) );

	m_movie = new AnimatedMovieWidget( page );
	hlay->addWidget( m_movie );

	connect( combo, SIGNAL( activated( const QString& ) ), this, SLOT( slotCrystalChanged( const QString& ) ) );

	m_basePath = KGlobal::dirs()->findResourceDir( "appdata", "data/lattice/" ) + "data/lattice/";

	QDir dir( m_basePath );
	dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );
	QStringList tmplist = dir.entryList();

	for ( int i = 0; i < tmplist.size(); i++ )
	{
		QFileInfo fi( m_basePath + tmplist.at( i ) );
		if ( fi.isDir() )
			combo->addItem( tmplist.at( i ) );
	}

	slotCrystalChanged( combo->itemText( 0 ) );
}

void SimpleCrystalViewer::slotCrystalChanged( const QString& which )
{
	m_movie->setPicturePath( m_basePath + which );
}

#include "simplecrystalviewer.moc"

