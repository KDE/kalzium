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

#include "element.h"

#include <qlabel.h>
#include <qmap.h>

MolcalcImpl::MolcalcImpl(QWidget *parent, const char *name, bool modal )
 : MolcalcDialog(parent, name, modal)
{
	m_weight = 0;

	connect( plusButton, SIGNAL( toggled(bool) ), this, SLOT( slotPlusToggled(bool) ) );
	connect( minusButton, SIGNAL( toggled(bool) ), this, SLOT( slotMinusToggled(bool) ) );
}

void MolcalcImpl::slotButtonClicked( int buttonnumber )
{
	//kdDebug() << "slotButtonClicked( int buttonnumber ) ... " << buttonnumber << endl;

	if ( plusButton->isOn() )
		updateData( buttonnumber, ADD );
	else
		updateData( buttonnumber, REMOVE );
}

void MolcalcImpl::updateData( int number, KIND kind )
{
	Element *el = new Element( number );
	if ( kind == ADD )
	{
		//kdDebug() << "ADD" << endl;
		m_weight += el->weight();
		m_elements.append( el );
	}
	else //TODO check if the element was in the list
	{
		//kdDebug() << "REMOVE" << endl;

		QValueList<Element*>::const_iterator it = m_elements.begin( );
		QValueList<Element*>::const_iterator itEnd = m_elements.end( );

		//kdDebug() << "Try to remove Element " << el->elname() << endl;

		bool removed = false;
		while ( !removed && ( it != itEnd ))
		{
				if ( ( *it )->elname() == el->elname() )
				{
					m_elements.remove( *it );
					removed = true;
				}
				it++;
		}
	}
	updateUI();
}

void MolcalcImpl::slotPlusToggled(bool on)
{
	on ? minusButton->setOn( false ) : minusButton->setOn( true );
}	

void MolcalcImpl::recalculate()
{
	//kdDebug() << "# of elements: " << m_elements.count() << endl;
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
		else
			kdDebug() << ( *it )->elname() << " not added, was already included" << endl;
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
	for ( itMap = map.begin(); itMap != map.end(); ++itMap ) {
		kdDebug() << "Key: "<< itMap.key()->elname() << "   ...    Data: " << itMap.data() << endl;
		str += i18n( "%1 %2\n" ).arg( itMap.data() ).arg( itMap.key()->elname() );
	}
	
	resultLabel->setText( str );

	//the composition
	
	//the weight
	recalculate();
	resultWeight->setText( i18n( "Molecular Weight: %1u" ).arg( m_weight ) );
}

void MolcalcImpl::slotMinusToggled(bool on)
{
	on ? plusButton->setOn( false ) : plusButton->setOn( true );
}	

#include "molcalc_impl.moc"
