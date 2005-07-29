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

#include <qlabel.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qcursor.h>

#include <kdebug.h>
#include <klocale.h>

#include "kalziumdataobject.h"

#include "math.h"

NuclideBoardDialog::NuclideBoardDialog( QWidget* parent, const char* name )
	: KDialogBase( Plain, i18n( "Nuclide Board" ), Close, Close, parent, name, false )
{
	QVBoxLayout *vbox = new QVBoxLayout( plainPage(), 0, spacingHint() );
	vbox->activate();
	NuclideBoard *b = new NuclideBoard( plainPage(), "nb" );
	spin1 = new QSpinBox( 1, 110, 1, plainPage() );
	spin2 = new QSpinBox( 2, 111, 1, plainPage() );
	connect( spin1, SIGNAL( valueChanged( int ) ), b, SLOT( setStart( int ) ) );
	connect( spin2, SIGNAL( valueChanged( int ) ), b, SLOT( setStop( int ) ) );

	connect( b, SIGNAL( emitStartValue( int ) ), spin1, SLOT( setValue( int ) ) );
	connect( b, SIGNAL( emitStopValue( int ) ), spin2, SLOT( setValue( int ) ) );
	spin1->setValue( 1 );
	spin2->setValue( 18 );

	QHBoxLayout *hbox1 = new QHBoxLayout( 0L, 0, KDialog::spacingHint() );
	hbox1->addWidget( new QLabel( i18n( "First Element:" ), plainPage() ) );
	hbox1->addWidget( spin1 );
	QHBoxLayout *hbox2 = new QHBoxLayout( 0L, 0, KDialog::spacingHint() );
	hbox2->addWidget( new QLabel( i18n( "Last Element:" ), plainPage() ) );
	hbox2->addWidget( spin2 );

	vbox->addWidget( b );
	vbox->addLayout( hbox1 );
	vbox->addLayout( hbox2 );

	setMinimumSize( 500, 450 );
	resize( minimumSize() );
}

NuclideBoard::NuclideBoard(QWidget *parent, const char* name) 
	: QWidget(parent, name)
{
	m_list = KalziumDataObject::instance()->ElementList;
	m_start = 80;
	m_stop = 100;
}

void NuclideBoard::mousePressEvent( QMouseEvent *e )
{
	kdDebug() << "NuclideBoard::mousePressEvent()" << endl;
	
	QValueList<IsotopeWidget*>::const_iterator it = m_isotopeWidgetList.begin();
	const QValueList<IsotopeWidget*>::const_iterator itEnd = m_isotopeWidgetList.end();
	
	QPoint pt = mapFromGlobal( QCursor::pos() );
	
	int size = ( *it )->size();

	QPoint point( pt.x()/size+m_lowestNumberOfNeutrons, ( height()-pt.y() )/size+m_start );

	kdDebug() << "point : " << point << endl;
	
	if (  e->button() == QMouseEvent::LeftButton )
	{
//X 		emit clicked( m_isotope );
	}
}


void NuclideBoard::slotDrawDecayRow( Isotope* isotope )
{
	kdDebug() << "NuclideBoard::slotDrawDecayRow()" << endl;
	if ( !isotope->alphadecay() 
			&& !isotope->betaplusdecay()
			&& !isotope->betaminusdecay() )
		return;

	kdDebug() << "Isotope: " << isotope->neutrons() << endl;
}

void NuclideBoard::paintEvent( QPaintEvent* /* e */ )
{
	QPainter p;
	p.begin( this );
	
	const int rangeOfNeutrons = m_highestNumberOfNeutrons-m_lowestNumberOfNeutrons;
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
		p.drawText( i*w+w, 0,w-1,h-1, Qt::AlignCenter, QString::number( m_lowestNumberOfNeutrons+i ));

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
	m_isotopeWidgetList.clear();

	QValueList<Element*>::const_iterator it = m_list.at(m_start-1);
	const QValueList<Element*>::const_iterator itEnd = m_list.at(m_stop);
	
	const int rangeOfNeutrons = m_highestNumberOfNeutrons-m_lowestNumberOfNeutrons;
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
			int position = n_count - m_lowestNumberOfNeutrons;
			int x = position * w;

			y = height()-( ( i-m_start )*h );
			
			IsotopeWidget *widget = new IsotopeWidget( *i_it, this );
	
			connect( widget, SIGNAL( clicked( Isotope* ) ), this, SLOT(slotDrawDecayRow( Isotope* ) ) );

			widget->setSize( w );
			widget->setPoint( QPoint(x,y) );

			m_isotopeWidgetList.append( widget );
		}
		i++;
	}
	update();
}

IsotopeWidget::IsotopeWidget( Isotope* isotope, QWidget *parent ) : QWidget(parent)
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
