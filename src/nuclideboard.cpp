/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *   
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
#include "nuclideboard.h"
#include <kdebug.h>

NuclideBoard::NuclideBoard(QValueList<Element*> list, QWidget *parent, const char* name) : QWidget(parent, name)
{
	m_list = list;
}

void NuclideBoard::paintEvent( QPaintEvent* /* e */ )
{
	QPainter p;
	p.begin( this );

	p.drawLine( 0,0,width(), height() );

	QValueList<Element*>::const_iterator it = m_list.begin();
	const QValueList<Element*>::const_iterator itEnd = m_list.end();

	const int numberOfElement = m_list.count();
	const int numberOfNeutrons = highestNeutronCount();

	//the width and height for each square
	const int w = width()/numberOfElement;
	const int h = w;

	for ( int i = 1 ; it != itEnd; ++it )
	{
		QValueList<Isotope*> i_list = ( *it )->isotopes();
		QValueList<Isotope*>::const_iterator i_it = i_list.begin();
		QValueList<Isotope*>::const_iterator i_itEnd = i_list.end();

		for ( ; i_it != i_itEnd; ++i_it )
		{
			p.drawRect( i*w, ( *i_it )->neutrons()*h, w, h);
		}
		i++;
	}
	
	p.end();
}

int NuclideBoard::highestNeutronCount()
{
	int count = 0;
	
	QValueList<Element*>::const_iterator it = m_list.begin();
	const QValueList<Element*>::const_iterator itEnd = m_list.end();
	
	for (; it != itEnd; ++it )
	{
		QValueList<Isotope*> i_list = ( *it )->isotopes();
		QValueList<Isotope*>::const_iterator i_it = i_list.begin();
		QValueList<Isotope*>::const_iterator i_itEnd = i_list.end();

		for ( ; i_it != i_itEnd; ++i_it )
		{
			if ( count < ( *i_it )->neutrons() )	
				count = ( *i_it )->neutrons();
		}
	}
	return count;
}
 
 #include "nuclideboard.moc"
