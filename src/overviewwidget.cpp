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
	: OverViewBase( parent, name )
{
}

void OverviewWidget::setElement( const int elementnumber )
{
	Element *el = new Element( elementnumber );
	detailedGraphicalOverview->setElement( el );
	detailedGraphicalOverview->repaint();
	textLabel1->setText( i18n( "<b>Name</b>: %1" ).arg( el->elname().utf8() ) );
	textLabel2->setText( i18n( "<b>Weight</b>: %1" ).arg( Element::adjustUnits( el->weight(),3 ) ) );
	textLabel3->setText( i18n( "<b>Boilingpoint</b>: %1" ).arg( Element::adjustUnits( el->boiling(),0 ) ) );
	textLabel4->setText( i18n( "<b>Meltingpoint</b>: %1" ).arg( Element::adjustUnits( el->melting(),0 ) ) );
}


#include "overviewwidget.moc"
