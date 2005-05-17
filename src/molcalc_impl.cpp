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
#include "molcalc_impl.h"

#include <kdebug.h>
#include <klocale.h>
#include <kpushbutton.h>

#include <qlabel.h>
#include <qmap.h>

MolcalcImpl::MolcalcImpl(KalziumDataObject *data, QWidget *parent, const char *name, bool modal )
 : MolcalcDialog(parent, name, modal)
{
	m_weight = 0;

	m_data = data;

	connect( plusButton, SIGNAL( toggled(bool) ), this, SLOT( slotPlusToggled(bool) ) );
	connect( minusButton, SIGNAL( toggled(bool) ), this, SLOT( slotMinusToggled(bool) ) );
}

void MolcalcImpl::slotButtonClicked( int buttonnumber )
{
	if ( plusButton->isOn() )
		updateData( buttonnumber, ADD );
	else
		updateData( buttonnumber, REMOVE );
}

void MolcalcImpl::updateData( int number, KIND kind )
{
	Element *el = m_data->element( number );
	
	if ( kind == ADD )
	{//adding the element
		m_weight += el->weight();
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
		QValueList<Element*>::const_iterator itEnd2 = tmpList2.end(); 
		for ( ; it2 != itEnd2 ; ++it2 )
		{ 
			tmpList.append( *it2 ); 
		}

		m_elements = tmpList;
	}
	//the list is updated, now update the User Interface
	updateUI();
}

void MolcalcImpl::slotPlusToggled(bool on)
{
	on ? minusButton->setOn( false ) : minusButton->setOn( true );
}	

void MolcalcImpl::recalculate()
{
	QValueList<Element*>::const_iterator it = m_elements.begin( );
	const QValueList<Element*>::const_iterator itEnd = m_elements.end( );

	m_weight = 0.0;
	
	for ( ; it != itEnd ; ++it )
	{
		m_weight += ( *it )->weight();
	}
}

void MolcalcImpl::updateUI()
{
	QString str;
	
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
		str += i18n( "%1 %2. Cumulative Weight: %3 u (%4 %)\n" ).arg( itMap.data() ).arg( i18n( itMap.key()->elname().utf8() ) ).arg( itMap.data() * itMap.key()->weight() ).arg(((  itMap.data() * itMap.key()->weight() )/m_weight )*100);
	}
	
	resultLabel->setText( str );
	
	//the composition
	resultComposition->setText( composition( map ) );
	
	//the weight
	recalculate();
	resultWeight->setText( i18n( "Molecular Weight: %1u" ).arg( m_weight ) );
}

QString MolcalcImpl::composition( QMap<Element*,int> map )
{
	QString str;
	
	QMap<Element*, int>::Iterator itMap;
	for ( itMap = map.begin(); itMap != map.end(); ++itMap ) 
	{
		str += i18n( "%1<sub>%2</sub>" ).arg( itMap.key()->symbol() ).arg( itMap.data() );
	}
	
	return str;
}

void MolcalcImpl::slotMinusToggled(bool on)
{
	on ? plusButton->setOn( false ) : plusButton->setOn( true );
}	

void MolcalcImpl::closeEvent(QCloseEvent* e)
{
	QWidget::closeEvent(e);
	emit closed();
}

#include "molcalc_impl.moc"
