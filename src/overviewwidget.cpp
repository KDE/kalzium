/***************************************************************************

    copyright            : (C) 2004 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "overviewwidget.h"
#include "element.h"
#include "detailedgraphicaloverview.h"

//KDE-Includes
#include <kdebug.h>
#include <klocale.h>

//QT-Includes
#include <qpainter.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qstring.h>

OverviewWidget::OverviewWidget( QWidget *parent, const char* name )
	: QWidget( parent, name )
{
	CentralWidget = new QWidget( this, "OverViewCentralWidget" );

	CentralWidget->setMinimumSize( QSize( 200,200 ) );
	
	m_vbox = new QHBoxLayout( CentralWidget );
	//m_vbox->setMinimumSize( sizeHint() );
}

void OverviewWidget::setElement( const int elementnumber )
{

	QLabel *l1 = new QLabel( "foo", CentralWidget );
	QLabel *l2 = new QLabel( "bar", CentralWidget );
	QString num = QString::number( elementnumber );
	QLabel *l3 = new QLabel( num, CentralWidget );

	Element *el = new Element( elementnumber );

	DetailedGraphicalOverview *view = new DetailedGraphicalOverview( el, CentralWidget );
	view->setMinimumSize( QSize(100,100) );

	m_vbox->addWidget( l1 );
	m_vbox->addWidget( view );
	m_vbox->addWidget( l3 );
	m_vbox->addWidget( l2 );

	view->show();
	l1->show();
	l2->show();
	l3->show();
}


#include "overviewwidget.moc"
