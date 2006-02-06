/***************************************************************************
 *   Copyright (C) 2005 by Martin Pfeiffer                                 *
 *   hubipete@gmx.net                                                      *
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

#include "printwidget.h"

#include <QDropEvent>
#include <QMimeData>
#include <QVBoxLayout>

#include <kdebug.h>

PrintWidget::PrintWidget( QWidget *parent ) : QWidget( parent )
{
	setupUi( this );

	m_printList = new PrintListWidget( this );

	QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget( btnPrint );
        layout->addWidget( m_printList );
	layout->addWidget( lblDescription );
        setLayout( layout );

	connect( btnPrint, SIGNAL( clicked() ), this, SLOT( slotPrint() ) );
}

void PrintWidget::slotPrint()
{
	if ( m_printList->count() == 0 )
		return;

//	Exporter exporter();
//	m_exporter.printElements();
}


PrintListWidget::PrintListWidget( QWidget* parent ) : QListWidget( parent )
{
	setAcceptDrops( true );
	setDropIndicatorShown( true );
}

void PrintListWidget::dragEnterEvent( QDragEnterEvent* event )
{
	if ( event->mimeData()->hasFormat("text/plain") )
		event->acceptProposedAction();
}

void PrintListWidget::dragMoveEvent( QDragMoveEvent* event )
{
	if ( event->mimeData()->hasFormat("text/plain") )
		event->acceptProposedAction();
}

void PrintListWidget::dropEvent( QDropEvent* event )
{
	kDebug() << "hall" << endl;
	addItem( event->mimeData()->text() );
}

#include "printwidget.moc"
