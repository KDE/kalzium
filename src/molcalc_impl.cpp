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

	resultLabel->setText( i18n( "Weight of the molecule: %1" ).arg( m_weight ) );
}

void MolcalcImpl::updateData( int number, KIND kind )
{
	Element el( number );
	if ( kind == ADD )
		m_weight += el.weight();
	else //TODO check if the element was in the list
		m_weight -= el.weight();
}

void MolcalcImpl::slotPlusToggled(bool on)
{
	on ? minusButton->setOn( false ) : minusButton->setOn( true );
}	

void MolcalcImpl::slotMinusToggled(bool on)
{
	on ? plusButton->setOn( false ) : plusButton->setOn( true );
}	

#include "molcalc_impl.moc"
