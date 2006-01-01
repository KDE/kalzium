/***************************************************************************
 *   Copyright (C) 2003-2005 by Carsten Niehaus, cniehaus@kde.org          *
 *   Copyright (C) 2005      by Inge Wallin,     inge@lysator.liu.se       *
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
 *                                                                         *
 ***************************************************************************/

#include "molcalcwidget.h"

#include "kalziumdataobject.h"
#include "kalziumutils.h"
#include "parser.h"

#include <element.h>

#include <kaction.h>
#include <kdebug.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <klineedit.h>

#include <QLabel>
#include <QLayout>
#include <QToolTip>

MolcalcWidget::MolcalcWidget( QWidget *parent )
    : QWidget( parent )
{
	setupUi( this );
	clear();
}


void MolcalcWidget::clear()
{
	// Clear the data.
	m_mass = 0;
	m_elementMap.clear();

	// Clear the widgets.
	resultLabel->setText( "" );
	resultMass->setText( "" );
	
	resultComposition->setText( i18n("To start, enter\na formula in the\nwidget above and\nclick on 'Calc'.") );

	resultMass->setToolTip( QString() );
	resultComposition->setToolTip( QString() );
	resultLabel->setToolTip( QString() );
}


void MolcalcWidget::updateUI()
{
	if ( m_validInput ){
		QString str;

		// The complexString stores the whole molecule like this:
		// 1 Seaborgium. Cumulative Mass: 263.119 u (39.2564 %)
		QString complexString;

		// Create the list of elements making up the molecule
		ElementCountMap::Iterator  it    = m_elementMap.begin();
		ElementCountMap::Iterator  itEnd = m_elementMap.end();
		for ( ; it != itEnd; ++it ) {
			// Update the resultLabel
			str += i18n( "For example: \"1 Carbon\" or \"3 Oxygen\"", "%1 %2\n" )
				.arg( (*it)->count() )
				.arg( (*it)->element()->dataAsString( ChemicalDataObject::name) );
		}
		resultLabel->setText( str );

		// The composition
		resultComposition->setText( compositionString(m_elementMap) );

		// The mass
		resultMass->setText( i18n( "Molecular mass: %1 u" ).arg( m_mass ) );

		resultMass->setToolTip(        complexString );
		resultComposition->setToolTip( complexString );
		resultLabel->setToolTip(       complexString );
	}
	else{//the input was invalid, so tell this the user
		resultComposition->setText( i18n( "Invalid input" ) );
		resultLabel->setText( QString() );
		resultMass->setText( QString() );

		QToolTip::add( resultMass,        i18n( "Invalid input" ) );
		QToolTip::add( resultComposition, i18n( "Invalid input" ) );
		QToolTip::add( resultLabel,       i18n( "Invalid input" ) );
	}
}

QString MolcalcWidget::compositionString( ElementCountMap &_map )
{
	QString  str;

	ElementCountMap::Iterator  it    = _map.begin();
	ElementCountMap::Iterator  itEnd = _map.end();
	for (; it != itEnd; ++it) {
		str += i18n( "%1<sub>%2</sub> " )
			.arg( (*it)->element()->dataAsString( ChemicalDataObject::symbol ) )
			.arg( (*it)->count() );
	}

	return str;
}


// ----------------------------------------------------------------
//                            slots


void MolcalcWidget::slotCalcButtonClicked()
{
	QString  molecule = formulaEdit->text();

	// Parse the molecule, and at the same time calculate the total
	// mass, and the composition of it.
	bool parseOk = m_parser.weight(molecule, &m_mass, &m_elementMap);

	m_validInput = m_parser.weight(molecule, &m_mass, &m_elementMap);

	updateUI();
}


#include "molcalcwidget.moc"
