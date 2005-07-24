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
#include <qstring.h>

#include <kdebug.h>
#include <klocale.h>
#include <kapplication.h>

#include "kalziumdataobject.h"

#include "math.h"

NuclideBoardDialog::NuclideBoardDialog( QWidget* parent, const char* name )
	: KDialogBase(parent, "NuclideBoardDialog", true, i18n( "Nuclide Board" ),
			KDialogBase::Apply|KDialogBase::Close|KDialogBase::Help, KDialogBase::Apply, true )
{
	QWidget *page = new QWidget( this );

	NuclideBoard *b = new NuclideBoard( page, "nb" );

	setMainWidget( page );

	QVBoxLayout *vbox = new QVBoxLayout(  page , 0, KDialogBase:: spacingHint() );
	
	spin1 = new QSpinBox( 1, 110, 1, page );
	spin2 = new QSpinBox( 2, 111, 1, page );
	connect( spin1, SIGNAL( valueChanged( int ) ), b, SLOT( setStart( int ) ) );
	connect( spin2, SIGNAL( valueChanged( int ) ), b, SLOT( setStop( int ) ) );

	connect( b, SIGNAL( emitStartValue( int ) ), spin1, SLOT( setValue( int ) ) );
	connect( b, SIGNAL( emitStopValue( int ) ), spin2, SLOT( setValue( int ) ) );
	spin1->setValue( 80 );
	spin2->setValue( 100 );

	QHBoxLayout *hbox1 = new QHBoxLayout( page, 0, KDialog::spacingHint() );
	hbox1->addWidget( new QLabel( i18n( "First Element:" ), page ) );
	hbox1->addWidget( spin1 );
	QHBoxLayout *hbox2 = new QHBoxLayout( page, 0, KDialog::spacingHint() );
	hbox2->addWidget( new QLabel( i18n( "Last Element:" ), page ) );
	hbox2->addWidget( spin2 );

	vbox->addWidget( b );
	vbox->addLayout( hbox1 );
	vbox->addLayout( hbox2 );

	setMinimumSize( 500, 450 );
	resize( minimumSize() );
	update();
}

void NuclideBoardDialog::slotHelp()
{
	emit helpClicked();
	if ( kapp )
		kapp->invokeHelp ( "nuclid_board", "kalzium" );
}


NuclideBoard::NuclideBoard( QWidget *parent, const char* name ) 
	: QScrollView( parent, name )
{
	kdDebug() << "NuclideBoard()" << endl;

	m_list = KalziumDataObject::instance()->ElementList;
	m_start = 80;
	m_stop = 100;

	m_highestNumberOfNeutrons = highestNeutronCount();
	m_lowestNumberOfNeutrons = lowestNeutronCount();
}

void NuclideBoard::slotDrawDecayRow( Isotope* isotope )
{
	kdDebug() << "NuclideBoard::slotDrawDecayRow()" << endl;
	if ( !isotope->alphadecay() 
			|| !isotope->betaplusdecay()
			|| !isotope->betaminusdecay() )
		return;

	kdDebug() << "Isotope: " << isotope->neutrons() << endl;
}

void NuclideBoard::drawContents( QPainter * p, int clipx, int clipy, int clipw, int cliph ) 
{
	kdDebug() << "NuclideBoard::drawContents()" << endl;

	m_highestNumberOfNeutrons = highestNeutronCount();
	m_lowestNumberOfNeutrons = lowestNeutronCount();

	resizeContents( ( m_highestNumberOfNeutrons - m_lowestNumberOfNeutrons ) * 50 + 60,
			( m_stop - m_start ) * 50 + 50 );

	for ( int i = m_start; i <= m_stop; ++i )
		p->drawText( 0, ( m_stop - i ) * 50 + 50, 50, 50, Qt::AlignCenter,
				 m_list[i - 1]->symbol() );

	for ( int i = m_lowestNumberOfNeutrons; i <= m_highestNumberOfNeutrons; ++i )
		p->drawText( ( i - m_lowestNumberOfNeutrons ) * 50 + 60, 0, 50, 50,
				 Qt::AlignCenter, QString::number( i ) );

	QValueList<IsotopeWidget*>::const_iterator it = m_isotopeWidgetList.begin();
	const QValueList<IsotopeWidget*>::const_iterator itEnd = m_isotopeWidgetList.end();

	while ( it != itEnd )
	{
		( *it )->update();
		++it;
	}
}

int NuclideBoard::highestNeutronCount()
{
	kdDebug() << "NuclideBoard::highestNeutronCount()" << endl;

	QValueList<Element*>::const_iterator it;
	const QValueList<Element*>::const_iterator itEnd = m_list.at( m_stop );

	QValueList<Isotope*> isotopeList;
	QValueList<Isotope*>::const_iterator isotope;
	QValueList<Isotope*>::const_iterator isotopeEnd;

	int count = 0;

	for ( it = m_list.at( m_start - 1 ); it != itEnd; ++it )
	{
		isotopeList = ( *it )->isotopes();

		if ( isotopeList.empty() )
			continue;

		isotopeEnd = isotopeList.end();
	
		for ( isotope = isotopeList.begin(); isotope != isotopeEnd; ++isotope )
		{
			if ( count < ( *isotope )->neutrons() )	
				count = ( *isotope )->neutrons();
		}
	}

	return count;
}

int NuclideBoard::lowestNeutronCount()
{
	kdDebug() << "NuclideBoard::lowestNeutronCount()" << endl;

	QValueList<Element*>::const_iterator it = m_list.at( m_start - 1 );
	const QValueList<Element*>::const_iterator itEnd = m_list.at( m_stop );

	QValueList<Isotope*> isotopeList;
	QValueList<Isotope*>::const_iterator isotope;
	QValueList<Isotope*>::const_iterator isotopeEnd;

	int count = 0;
	
	for (; it != itEnd; ++it )
	{
		isotopeList = ( *it )->isotopes();
		if ( isotopeList.empty() )
			continue;

		isotopeEnd = isotopeList.end();

		for ( isotope = isotopeList.begin(); isotope != isotopeEnd; ++isotope )
		{
			if ( count > ( *isotope )->neutrons() )	
				count = ( *isotope )->neutrons();
		}
	}

	return count;
}

void NuclideBoard::setStop( int value )
{
	if ( value < m_start )
	{
		m_start = value - 1;
		emitStartValue( m_start );
	}

	m_stop = value;
	updateList();
}

void NuclideBoard::setStart( int value )
{
	if ( value > m_stop )
	{
		m_stop = value + 1;
		emitStopValue( m_stop );
	}

	m_start = value;
	updateList();
}

void NuclideBoard::updateList()
{
	kdDebug() << "NuclideBoard::updateList()" << endl;

	m_isotopeWidgetList.clear();

	QValueList<Element*>::const_iterator it = m_list.at( m_start - 1 );
	const QValueList<Element*>::const_iterator itEnd = m_list.at( m_stop );

	QValueList<Isotope*> isotopeList;
	QValueList<Isotope*>::const_iterator isotope;
	QValueList<Isotope*>::const_iterator isotopeEnd;

	for ( ; it != itEnd; ++it )
	{
		isotopeList = ( *it )->isotopes();
		isotope = isotopeList.begin();
		isotopeEnd = isotopeList.end();

		for ( ; isotope != isotopeEnd; ++isotope )
		{
			IsotopeWidget *widget = new IsotopeWidget( *isotope, this );
	
			connect( widget, SIGNAL( clicked( Isotope* ) ), this,
					 SLOT( slotDrawDecayRow( Isotope* ) ) );

			addChild( widget );
		
			widget->move( 60 + ( ( *isotope )->neutrons() - m_lowestNumberOfNeutrons ) * 50,
					  50 + ( m_stop - ( *it )->number() ) * 50 );
			m_isotopeWidgetList.append( widget );
		}
	}

	updateContents();
}

IsotopeWidget::IsotopeWidget( Isotope* isotope, QWidget *parent ) : QWidget(parent)
{
	m_isotope = isotope;	

	if ( m_isotope->betaminusdecay() )
		m_color = Qt::blue;
	else if ( m_isotope->betaplusdecay() )
		m_color = Qt::red;
	else if ( m_isotope->alphadecay() )
		m_color = Qt::yellow;
	else if (  m_isotope->ecdecay() )
		m_color = Qt::green;
	else
		m_color = Qt::magenta;

	m_active = false;

	resize( 50, 50 );
}

void IsotopeWidget::mousePressEvent( QMouseEvent *e )
{
	emit clicked( m_isotope );
}

void IsotopeWidget::paintEvent( QPaintEvent* e )
{
	QPainter p( this );

	if ( m_active )
		p.fillRect( 0, 0, width(), height(), m_color.dark() );
	else
		p.fillRect( 0, 0, width(), height(), m_color );

	p.drawRect( 0, 0, width(), height() );
}

void Decay::showDecay()
{
	// iterate through all isotopeWidgets and set them active = true

	QValueList<IsotopeWidget*>::const_iterator it = m_list.begin();
	const QValueList<IsotopeWidget*>::const_iterator itEnd = m_list.end();

	while ( it != itEnd )
	{
		( *it )->activate( true );
		++it;
	}
}

void Decay::hideDecay()
{
	// iterate through all isotopeWidgets and set them active = false

 	QValueList<IsotopeWidget*>::const_iterator it = m_list.begin();
	const QValueList<IsotopeWidget*>::const_iterator itEnd = m_list.end();

	while ( it != itEnd )
	{
		( *it )->activate( false );
		++it;
	}
}
 
#include "nuclideboard.moc"

