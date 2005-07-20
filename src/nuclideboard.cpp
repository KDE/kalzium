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
#include "math.h"

NuclideBoard::NuclideBoard(QValueList<Element*> list, QWidget *parent, const char* name) : QWidget(parent, name)
{
	m_list = list;
	m_start = 40;
	m_stop = 80;
}

void NuclideBoard::paintEvent( QPaintEvent* /* e */ )
{
	QPainter p;
	p.begin( this );

	QValueList<Element*>::const_iterator it = m_list.at(m_start);
	const QValueList<Element*>::const_iterator itEnd = m_list.at(m_stop);

	const int numberOfElement = m_list.count();
	const int highestNumberOfNeutrons = highestNeutronCount();
	const int lowestNumberOfNeutrons = lowestNeutronCount();
	const int rangeOfNeutrons = highestNumberOfNeutrons-lowestNumberOfNeutrons;
	const int rangeOfElements = m_stop-m_start;

	//the width and height for each square
	int w_ = width()/rangeOfNeutrons;
	int h_ = height()/rangeOfElements;

	int w;

	w_ < h_ ? w = w_ : w = h_;

	const int h = w;

	kdDebug() << h << " :: " << w << endl;

	const int offset = h;

	for ( int i = m_start ; it != itEnd; ++it )
	{
		QValueList<Isotope*> i_list = ( *it )->isotopes();
		QValueList<Isotope*>::const_iterator i_it = i_list.begin();
		QValueList<Isotope*>::const_iterator i_itEnd = i_list.end();

		for ( ; i_it != i_itEnd; ++i_it )
		{
			QColor c;

			if ( ( *i_it )->betaminusdecay() )
				c = Qt::blue;
			else if ( ( *i_it )->betaplusdecay() )
				c = Qt::red;
			else if (  ( *i_it )->alphadecay() )
				c = Qt::yellow;
			else if (  ( *i_it )->ecdecay() )
				c = Qt::green;
			else
				c= Qt::magenta;

			//on the x-axis the neutrons are places
			//on the y-axis the elements
			int y;
			
			int n_count = ( *i_it )->neutrons();
			int position = n_count - lowestNumberOfNeutrons;
			int x = position * w;

			kdDebug() << n_count << " :: " << position << " :: " << x << endl;
			y = height()-( ( i-rangeOfElements )*h );

			kdDebug() << "x: " << x << " y: " << y << endl;
			
			p.fillRect( x+20, y-20, w-1, h-1,c );
			p.drawRect( x+20, y-20, w-1, h-1 );
		}
		i++;
	}
	for ( int i = 0; i < rangeOfElements; ++i )
		p.drawText( 0, height( )-( i*h )-20,w-1,h-1, Qt::AlignCenter, QString::number( m_start+i ));
	for ( int i = 0; i < rangeOfNeutrons; ++i )
		p.drawText( i*w, 0,w-1,h-1, Qt::AlignCenter, QString::number( lowestNumberOfNeutrons+i ));
	
	p.end();
}

int NuclideBoard::highestNeutronCount()
{
	int count = 0;
	
	QValueList<Element*>::const_iterator it = m_list.at(m_start);
	const QValueList<Element*>::const_iterator itEnd = m_list.at(m_stop);
	
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

int NuclideBoard::lowestNeutronCount()
{
	QValueList<Element*>::const_iterator it = m_list.at(m_start);
	const QValueList<Element*>::const_iterator itEnd = m_list.at(m_stop);
	
	int count = 200;
	
	for (; it != itEnd; ++it )
	{
		QValueList<Isotope*> i_list = ( *it )->isotopes();
		QValueList<Isotope*>::const_iterator i_it = i_list.begin();
		QValueList<Isotope*>::const_iterator i_itEnd = i_list.end();

		for ( ; i_it != i_itEnd; ++i_it )
		{
			if ( count > ( *i_it )->neutrons() )	
				count = ( *i_it )->neutrons();
		}
	}
	return count;
}
 
 #include "nuclideboard.moc"
