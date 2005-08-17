/***************************************************************************
 *   Copyright (C) 2003, 2004 by Carsten Niehaus                                 *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/
#include "molcalcwidget.h"

#include "kalziumdataobject.h"
#include "molcalcwidgetbase.h"
#include "element.h"
#include "kalziumutils.h"
#include "parser.h"
#include "moleculeparser.h"

#include <kaction.h>
#include <kdebug.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <klineedit.h>
#include <ktoolbar.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>

MolcalcWidget::MolcalcWidget( QWidget *parent, const char *name )
    : MolcalcWidgetBase( parent, "molcalcwidgetbase" )
{
	setName( name );

	m_mass = 0;

	m_data = KalziumDataObject::instance();

	// setting up the toolbar
	KToolBar *toolbar = new KToolBar( this, "toolbar", this );
	toolbar->setIconSize( 22 );
	MolcalcWidgetBaseLayout->addWidget( toolbar, 0, 0 );

	plusButton = new KToggleAction( i18n( "&Add" ), "add", 0, 0, 0, this, "add" );
	plusButton->setChecked( true );
	plusButton->setWhatsThis( i18n( "Toggle this button to allow selected elements to be added to the current molecule" ) );
	connect( plusButton, SIGNAL( toggled(bool) ), this, SLOT( slotPlusToggled(bool) ) );
	plusButton->plug( toolbar );

	minusButton = new KToggleAction( i18n( "&Remove" ), "remove", 0, 0, 0, this, "remove" );
	minusButton->setWhatsThis( i18n( "Toggle this button to allow selected elements to be removed from the current molecule" ) );
	connect( minusButton, SIGNAL( toggled(bool) ), this, SLOT( slotMinusToggled(bool) ) );
	minusButton->plug( toolbar );

	toolbar->insertLineSeparator();

	KAction *clearButton = new KAction( i18n( "&Clear" ), "trashcan_empty", 0, this, SLOT( clear() ), this, "clear" );
	clearButton->setWhatsThis( i18n( "Click this button to drop the created molecule" ) );
	clearButton->plug( toolbar );

	clear();
}

void MolcalcWidget::slotButtonClicked( int buttonnumber )
{
	if ( plusButton->isChecked() )
		updateData( buttonnumber, ADD );
	else
		updateData( buttonnumber, REMOVE );
}

void MolcalcWidget::updateData( int number, KIND kind )
{
	Element *el = m_data->element( number );
	
	if ( kind == ADD )
	{//adding the element
		bool found = false;
		QMap<Element*, int> newelements;
		QMap<Element*, int>::Iterator it = m_elements.begin();
		QMap<Element*, int>::Iterator itEnd = m_elements.end();
		// searching for the element
	        for ( ; it != itEnd; ++it ) {
			if ( it.key()->elname() == el->elname() )
			{
				newelements.insert( it.key(), it.data() + 1 );
				found = true;
			}
			else
				newelements.insert( it.key(), it.data() );
		}
		if ( !found )
		{
			// element not found, adding it
			newelements.insert( el, 1 );
		}
		m_elements = newelements;
	}
	else
	{//removing the element in case there was at least on such element
	 //added before
		QMap<Element*, int> newelements;
		QMap<Element*, int>::Iterator it = m_elements.begin();
		QMap<Element*, int>::Iterator itEnd = m_elements.end();
		bool found = false;
		// searching for the element
	        for ( ; it != itEnd; ++it ) {
			if ( it.key()->elname() == el->elname() )
			{
				if ( it.data() > 1 )
					newelements.insert( it.key(), it.data() - 1 );
			}
			else
				newelements.insert( it.key(), it.data() );
	        }
		m_elements = newelements;
	}
	//the list is updated, now update the User Interface
	updateUI();
}

void MolcalcWidget::slotPlusToggled(bool on)
{
	minusButton->setChecked( !on );
}	

void MolcalcWidget::recalculate()
{
	QMap<Element*, int>::Iterator it = m_elements.begin();
	QMap<Element*, int>::Iterator itEnd = m_elements.end();

	m_mass = 0.0;

	for ( ; it != itEnd; ++it )
	{
		m_mass += it.key()->mass() * it.data();
        }
}

void MolcalcWidget::updateUI()
{
	QString str;

	//The complexString stores the whole molecule like this:
	//1 Seaborgium. Cumulative Mass: 263.119 u (39.2564 %)
	QString complexString;
	
	QMap<Element*, int>::Iterator itMap = m_elements.begin();
	QMap<Element*, int>::Iterator itMapEnd = m_elements.end();
	for ( ; itMap != itMapEnd; ++itMap )
	{//update the resultLabel
		str += i18n( "For example: \"1 Carbon\" or \"3 Oxygen\"", "%1 %2." ).arg( itMap.data() ).arg( itMap.key()->elname() );
		str += "\n";
		complexString += i18n( "For example: 1 Seaborgium. Cumulative Mass: 263.119 u (39.25%)", "%1 %2. Cumulative Mass: %3 u (%4%)\n" ).arg( itMap.data() ).arg( itMap.key()->elname() ).arg( itMap.data() * itMap.key()->mass() ).arg(KalziumUtils::strippedValue( ((  itMap.data() * itMap.key()->mass() )/m_mass )*100) );
	}
	
	resultLabel->setText( str );
	
	//the composition
	resultComposition->setText( composition( m_elements ) );
	
	//the mass
	recalculate();
	resultMass->setText( i18n( "Molecular mass: %1 u" ).arg( m_mass ) );
	
	QToolTip::add( resultMass, complexString );
	QToolTip::add( resultComposition, complexString );
	QToolTip::add( resultLabel, complexString );
}

QString MolcalcWidget::composition( QMap<Element*,int> map )
{
	QString str;
	
	QMap<Element*, int>::Iterator itMap;
	for ( itMap = map.begin(); itMap != map.end(); ++itMap )
	{
		str += i18n( "%1<sub>%2</sub>" ).arg( itMap.key()->symbol() ).arg( itMap.data() );
	}

	return str;
}

void MolcalcWidget::slotMinusToggled(bool on)
{
	plusButton->setChecked( !on );
}

void MolcalcWidget::clear()
{
	m_mass = 0;
	m_elements.clear();

	resultLabel->setText( "" );
	resultMass->setText( "" );
	
	resultComposition->setText( i18n("To start, click\non the elements") );
}

void MolcalcWidget::slotCalcButtonClicked()
{
	m_elements.clear();
	m_mass = 0;
	QString substance = formulaEdit->text();
	double weight;
	
	if (m_parser.weight(substance, &weight))
	{
	    kdDebug() << "Weight of " << substance << " = " << weight << endl;;
		m_elements = m_parser.elementMap();
		updateUI();
	}
	else
	    kdDebug() << "Parse error" << endl;
}


#include "molcalcwidget.moc"
