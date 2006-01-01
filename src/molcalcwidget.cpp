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

//libscience
#include "element.h"

#include "kalziumdataobject.h"
#include "kalziumutils.h"

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
	kdDebug() << "MolcalcWidget()::MolcalcWidget" << endl;
	m_parser = new MoleculeParser( KalziumDataObject::instance()->ElementList );
	
	ui.setupUi( this );
	
	connect( ui.calcButton, SIGNAL( clicked() ), this, SLOT( slotCalcButtonClicked() ) );
	clear();

}


void MolcalcWidget::clear()
{
	kdDebug() << "MolcalcWidget::clear()" << endl;
	// Clear the data.
	m_mass = 0;
	m_elementMap.clear();

	// Clear the widgets.
	ui.resultLabel->setText( "" );
	ui.resultMass->setText( "" );
	
	ui.resultComposition->setText( i18n("To start, enter\na formula in the\nwidget above and\nclick on 'Calc'.") );

	ui.resultMass->setToolTip( QString() );
	ui.resultComposition->setToolTip( QString() );
	ui.resultLabel->setToolTip( QString() );
}


void MolcalcWidget::updateUI()
{
	kdDebug() << "MolcalcWidget::updateUI()" << endl;
	
	if ( m_validInput ){
		kdDebug() << "m_validInput == TRUE" << endl;
		QString str;

		// The complexString stores the whole molecule like this:
		// 1 Seaborgium. Cumulative Mass: 263.119 u (39.2564 %)
		QString complexString;

//X 		// Create the list of elements making up the molecule
//X 		ElementCountMap::Iterator  it    = m_elementMap.begin();
//X 		const ElementCountMap::Iterator  itEnd = m_elementMap.end();
//X 		for ( ; it != itEnd; ++it ) {
//X 			// Update the resultLabel
//X 			str += i18n( "For example: \"1 Carbon\" or \"3 Oxygen\"", "%1 %2\n" )
//X 				.arg( (*it)->count() )
//X 				.arg( (*it)->element()->dataAsString( ChemicalDataObject::name) );
//X 		}
		ui.resultLabel->setText( str );

		// The composition
//X 		resultComposition->setText( compositionString(m_elementMap) );

		// The mass
		ui.resultMass->setText( i18n( "Molecular mass: %1 u" ).arg( m_mass ) );

		ui.resultMass->setToolTip(        complexString );
		ui.resultComposition->setToolTip( complexString );
		ui.resultLabel->setToolTip(       complexString );
	}
	else{//the input was invalid, so tell this the user
		kdDebug() << "m_validInput == FALSE" << endl;
		ui.resultComposition->setText( i18n( "Invalid input" ) );
		ui.resultLabel->setText( QString() );
		ui.resultMass->setText( QString() );

		QToolTip::add( ui.resultMass,        i18n( "Invalid input" ) );
		QToolTip::add( ui.resultComposition, i18n( "Invalid input" ) );
		QToolTip::add( ui.resultLabel,       i18n( "Invalid input" ) );
	}
}

QString MolcalcWidget::compositionString( ElementCountMap &_map )
{
	kdDebug() << "MolcalcWidget::compositionString()" << endl;
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
	kdDebug() << "MolcalcWidget::slotCalcButtonClicked()" << endl;
	QString  molecule = ui.formulaEdit->text();

	// Parse the molecule, and at the same time calculate the total
	// mass, and the composition of it.
//	m_validInput = m_parser->weight("CO2", &m_mass, &m_elementMap);

	updateUI();
}


#include "molcalcwidget.moc"
