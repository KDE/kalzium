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
	m_start = 1;
	m_stop = 18;
}

void NuclideBoard::paintEvent( QPaintEvent* /* e */ )
{
	QPainter p;
	p.begin( this );
	while ( m_start >= m_stop)
	{
	m_stop = m_stop+1;
	}
	const int highestNumberOfNeutrons = highestNeutronCount();
	const int lowestNumberOfNeutrons = lowestNeutronCount();
	const int rangeOfNeutrons = highestNumberOfNeutrons-lowestNumberOfNeutrons;
	const int rangeOfElements = m_stop-m_start;

	//the width and height for each square
	int w_ = width()/( rangeOfNeutrons+1 );
	int h_ = height()/( rangeOfElements+1 );

	int w;

	w_ < h_ ? w = w_ : w = h_;

	const int h = w;

	for ( int i = 0; i <= rangeOfElements; ++i )
		p.drawText( 0, height()-( i*h )-h,w-1,h-1, Qt::AlignCenter, QString::number( m_start+i ));
	for ( int i = 0; i <= rangeOfNeutrons; ++i )
		p.drawText( i*w+w, 0,w-1,h-1, Qt::AlignCenter, QString::number( lowestNumberOfNeutrons+i ));

	QValueList<IsotopeWidget*>::const_iterator it = m_isotopeWidgetList.begin();
	const QValueList<IsotopeWidget*>::const_iterator itEnd = m_isotopeWidgetList.end();

	while ( it != itEnd )
	{
		( *it )->drawSelf( &p );
		++it;
	}
	
	p.end();
}

int NuclideBoard::highestNeutronCount()
{
	int count = 0;
	
	QValueList<Element*>::const_iterator it = m_list.at(m_start-1);
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
	QValueList<Element*>::const_iterator it = m_list.at(m_start-1);
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

void NuclideBoard::updateList()
{
	kdDebug() << "IsotopeWidget::updateList()" << endl;
			
	m_isotopeWidgetList.clear();

	QValueList<Element*>::const_iterator it = m_list.at(m_start-1);
	const QValueList<Element*>::const_iterator itEnd = m_list.at(m_stop);
	
	const int highestNumberOfNeutrons = highestNeutronCount();
	const int lowestNumberOfNeutrons = lowestNeutronCount();
	const int rangeOfNeutrons = highestNumberOfNeutrons-lowestNumberOfNeutrons;
	const int rangeOfElements = m_stop-m_start;

	//the width and height for each square
	int w_ = width()/( rangeOfNeutrons+1 );
	int h_ = height()/( rangeOfElements+1 );

	int w;

	w_ < h_ ? w = w_ : w = h_;

	const int h = w;

	for ( int i = m_start ; it != itEnd; ++it )
	{
		QValueList<Isotope*> i_list = ( *it )->isotopes();
		QValueList<Isotope*>::Iterator i_it = i_list.begin();
		const QValueList<Isotope*>::Iterator i_itEnd = i_list.end();

		for ( ; i_it != i_itEnd; ++i_it )
		{
			//on the x-axis the neutrons are places
			//on the y-axis the elements
			int y;
			
			int n_count = ( *i_it )->neutrons();
			int position = n_count - lowestNumberOfNeutrons;
			int x = position * w;

			y = height()-( ( i-m_start )*h );
			
			IsotopeWidget *widget = new IsotopeWidget( *i_it );
			widget->setSize( w );
			widget->setPoint( QPoint(x,y) );

			m_isotopeWidgetList.append( widget );
		}
		i++;
	}
}

IsotopeWidget::IsotopeWidget( Isotope* isotope )
{
	m_isotope = isotope;	
	
	QColor c;

	if ( m_isotope->betaminusdecay() )
		c = Qt::blue;
	else if ( m_isotope->betaplusdecay() )
		c = Qt::red;
	else if (  m_isotope->alphadecay() )
		c = Qt::yellow;
	else if (  m_isotope->ecdecay() )
		c = Qt::green;
	else
		c= Qt::magenta;

	m_color = c;

	m_active = false;
}


void IsotopeWidget::drawSelf( QPainter*p )
{
	kdDebug() << "IsotopeWidget::drawSelf()" << endl;
	QColor color = m_color;
	if ( m_active )
		color = m_color.dark();

	p->fillRect( m_point.x()+m_size, m_point.y()-m_size, m_size-1, m_size-1, color );
	
	p->drawRect( m_point.x()+m_size, m_point.y()-m_size, m_size-1, m_size-1 );

//X 	if ( m_active )
//X 	{
//X 		p->setPen( Qt::white );
//X 
//X 		p->drawRect( m_point.x()+m_size+1, m_point.y()-m_size-1, m_size-4, m_size-4 );
//X 
//X 		p->setPen( Qt::black );
//X 	}
}
 
 #include "nuclideboard.moc"
