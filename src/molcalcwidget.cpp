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
	minusButton->setWhatsThis( i18n( "Toggle this button to allow selected elements to be added to the current molecule" ) );
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
		m_mass += el->mass();
		m_elements.append( el );
	}
	else
	{//removing the element in case there was at least on such element
	 //added before
		QValueList<Element*>::const_iterator it = m_elements.begin( );
		QValueList<Element*>::const_iterator itEnd = m_elements.end( );

		//I am not sure if this is the best way, but at least it works. 
		//QValueList<> can't simple remove a *it, it would remove all (!!)
		//element of the same kind, for example all Carbon-elements.
		//Therefore I am using two list, a positive-match and a negative-match
		//list. Afterwards I append all but one elements of the positive-list
		//to the negative list. In the end I "removed" one element.
		QValueList<Element*> tmpList;
		QValueList<Element*> tmpList2;

		for ( ; it != itEnd ; ++it )
		{
			if ( ( *it )->elname() != el->elname() )
			{//negative-list
				tmpList.append( *it );
			}
			if ( ( *it )->elname() == el->elname() )
			{//positive-list
				tmpList2.append( *it );
			}
		}
		
		//I need to iterate it2 in order to skip on element
		QValueList<Element*>::const_iterator it2 = tmpList2.begin(); it2++;
		const QValueList<Element*>::const_iterator itEnd2 = tmpList2.end(); 
		for ( ; it2 != itEnd2 ; ++it2 )
		{ 
			tmpList.append( *it2 ); 
		}

		m_elements = tmpList;
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
	QValueList<Element*>::const_iterator it = m_elements.begin( );
	const QValueList<Element*>::const_iterator itEnd = m_elements.end( );

	m_mass = 0.0;
	
	for ( ; it != itEnd ; ++it )
	{
		m_mass += ( *it )->mass();
	}
}

void MolcalcWidget::updateUI()
{
	QString str;

	//The complexString stores the whole molecule like this:
	//1 Seaborgium. Cumulative Mass: 263.119 u (39.2564 %)
	QString complexString;
	
	//the elements
	QMap<Element*, int> map;
	
	QValueList<Element*>::const_iterator it = m_elements.begin( );
	const QValueList<Element*>::const_iterator itEnd = m_elements.end( );

	QValueList<Element*> differentElements;
	QValueList<Element*>::const_iterator itNames;

	for ( ; it != itEnd ; ++it )
	{//get the different elements in the molecule
		bool contains = false;
		for ( itNames = differentElements.begin(); itNames != differentElements.end() ; ++itNames )
		{
			if (  ( *it )->elname() == ( *itNames )->elname()     )
				contains = true;
		}
		if ( !contains )
			differentElements.append( *it );
	}

	itNames = differentElements.begin( );
	
	for ( ; itNames != differentElements.end() ; ++itNames )
	{//count the different elements (write the result in a QMap)
		it = m_elements.begin( );
		
		int count = 0;
		for ( ; it != itEnd ; ++it )
		{
			if ( ( *it )->elname() == ( *itNames )->elname() )
				count++;
		}
		map[ *itNames ] = count;
	}

	QMap<Element*, int>::Iterator itMap;
	for ( itMap = map.begin(); itMap != map.end(); ++itMap ) 
	{//update the resultLabel
		str += i18n( "For example: \"1 Carbon\" or \"3 Oxygen\"", "%1 %2." ).arg( itMap.data() ).arg( itMap.key()->elname() );
		str += "\n";
		complexString += i18n( "For example: 1 Seaborgium. Cumulative Mass: 263.119 u (39.25%)", "%1 %2. Cumulative Mass: %3 u (%4%)\n" ).arg( itMap.data() ).arg( itMap.key()->elname() ).arg( itMap.data() * itMap.key()->mass() ).arg(KalziumUtils::strippedValue( ((  itMap.data() * itMap.key()->mass() )/m_mass )*100) );
	}
	
	resultLabel->setText( str );
	
	//the composition
	resultComposition->setText( composition( map ) );
	
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
	    kdDebug() << "Weight of " << substance << " = " << weight << ".\n";
		m_elements = m_parser.elementList();
		updateUI();
	}
	else
	    kdDebug() << "Parse error\n";
}


#include "molcalcwidget.moc"
