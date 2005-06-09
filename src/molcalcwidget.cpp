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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "molcalcwidget.h"

#include <kdebug.h>
#include <klocale.h>
#include <kpushbutton.h>

#include <qlabel.h>
#include <qtooltip.h>

#include "molcalcwidgetbase.h"
#include "element.h"

MolcalcWidget::MolcalcWidget( KalziumDataObject *data, QWidget *parent, const char *name )
    : MolcalcWidgetBase( parent, "molcalcwidgetbase" )
{
	setName( name );

	m_mass = 0;

	m_data = data;

	QToolTip::add( plusButton, i18n( "If this button is selected, a selected element will be added to the molecule" ) );
	QToolTip::add( minusButton, i18n( "If this button is selected, a selected element will be removed from the molecule" ) );

	plusButton->setGuiItem( KGuiItem( i18n( "&Add" ), "add" ) );
	minusButton->setGuiItem( KGuiItem( i18n( "&Remove" ), "remove" ) );
	connect( plusButton, SIGNAL( toggled(bool) ), this, SLOT( slotPlusToggled(bool) ) );
	connect( minusButton, SIGNAL( toggled(bool) ), this, SLOT( slotMinusToggled(bool) ) );

	resultComposition->setText( i18n("To start, click\non the elements") );

	resize( 500, 400 );
}

void MolcalcWidget::slotButtonClicked( int buttonnumber )
{
	if ( plusButton->isOn() )
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
	minusButton->setOn( !on );
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
		str += i18n( "%1 %2.\n" ).arg( itMap.data() ).arg( i18n( itMap.key()->elname().utf8() ) );
		complexString += i18n( "For example: 1 Seaborgium. Cumulative Mass: 263.119 u (39.25%)", "%1 %2. Cumulative Mass: %3 u (%4%)\n" ).arg( itMap.data() ).arg( i18n( itMap.key()->elname().utf8() ) ).arg( itMap.data() * itMap.key()->mass() ).arg(Element::strippedValue( ((  itMap.data() * itMap.key()->mass() )/m_mass )*100) );
	}
	
	resultLabel->setText( str );
	
	//the composition
	resultComposition->setText( composition( map ) );
	
	//the mass
	recalculate();
	resultMass->setText( i18n( "Molecular Mass: %1u" ).arg( m_mass ) );
	
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
	plusButton->setOn( !on );
}

void MolcalcWidget::clear()
{
	m_mass = 0;
	m_elements.clear();

	resultLabel->setText( "" );
	resultComposition->setText( "" );
	resultMass->setText( "" );
}


#include "molcalcwidget.moc"
