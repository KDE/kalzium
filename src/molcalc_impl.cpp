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

MolcalcImpl::MolcalcImpl(QWidget *parent, const char *name, bool modal )
 : MolcalcDialog(parent, name, modal)
{
	m_weight = 0;

	connect( plusButton, SIGNAL( toggled(bool) ), this, SLOT( slotPlusToggled(bool) ) );
	connect( minusButton, SIGNAL( toggled(bool) ), this, SLOT( slotMinusToggled(bool) ) );
}

void MolcalcImpl::slotButtonClicked( int buttonnumber )
{
	kdDebug() << "slotButtonClicked( int buttonnumber ) ... " << buttonnumber << endl;

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
		kdDebug() << "ADD" << endl;
		m_weight += el->weight();
		m_elements.append( el );
	}
	else //TODO check if the element was in the list
	{
		kdDebug() << "REMOVE" << endl;

		QValueList<Element*>::const_iterator it = m_elements.begin( );
		QValueList<Element*>::const_iterator itEnd = m_elements.end( );

		kdDebug() << "Try to remove Element " << el->elname() << endl;

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
	recalculate();
	updateUI();
}

void MolcalcImpl::slotPlusToggled(bool on)
{
	on ? minusButton->setOn( false ) : minusButton->setOn( true );
}	

void MolcalcImpl::recalculate()
{
	kdDebug() << "# of elements: " << m_elements.count() << endl;
	QValueList<Element*>::const_iterator it = m_elements.begin( );
	QValueList<Element*>::const_iterator itEnd = m_elements.end( );

	m_weight = 0.0;
	
	for ( ; it != itEnd ; ++it )
	{
		m_weight += ( *it )->weight();
	}
}

void MolcalcImpl::updateUI()
{
	QValueList<Element*>::const_iterator it = m_elements.begin( );
	QValueList<Element*>::const_iterator itEnd = m_elements.end( );

	QString str;
	
	for ( ; it != itEnd ; ++it )
	{
		str += i18n( "%1 %2\n" ).arg(( *it )->elname()).arg(( *it )->weight());
	}
	resultLabel->setText( str );
}

void MolcalcImpl::slotMinusToggled(bool on)
{
	on ? plusButton->setOn( false ) : plusButton->setOn( true );
}	

#include "molcalc_impl.moc"
