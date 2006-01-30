/***************************************************************************
 *   Copyright (C) 2005      by Carsten Niehaus,    cniehaus@kde.org       *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "gamesimplementation.h"
#include "gamecontrols_impl.h"
#include "gamesdialog.h"
#include "gamefieldwidget.h"
#include "statisticwidget.h"

#include <QLayout>

#include <kdebug.h>

GamesDialog::GamesDialog()
	: KDialog( 0, "KalziumGames" )
{
	QVBoxLayout * vbox = new QVBoxLayout( this );
	QHBoxLayout * hbox = new QHBoxLayout( vbox );

	m_controls = new GameControls_Impl( this );
	m_gamefield = new GamefieldWidget( this );
	m_stats = new StatisticWidget( this );

	vbox->addWidget( m_gamefield );
	vbox->addWidget( m_controls );
	
	m_game = new RAGame();
	m_gamefield->setField( m_game->field() );

	hbox->addWidget( m_stats );

	createConnetions();
}

void GamesDialog::slotStartWithTimer()
{
	m_game->startWithTimer( 50 );
}

void GamesDialog::createConnetions()
{
	connect(m_controls->ui.next, SIGNAL( clicked() ), 
			m_game, SLOT(slotNextMove()) );
	connect(m_controls->ui.start, SIGNAL( clicked() ), 
			this, SLOT(slotStartWithTimer()) );
	connect(m_controls->ui.stop, SIGNAL( clicked() ), 
			m_game, SLOT(stopGame()) );
	connect(m_game, SIGNAL( turnOver(Move*) ), 
			m_gamefield, SLOT(slotUpdate(Move*)) );
	connect(m_game, SIGNAL( turnOver() ), 
			m_stats, SLOT(updateData( )) );

	m_stats->setGame( m_game );
}

#include "gamesdialog.moc"
