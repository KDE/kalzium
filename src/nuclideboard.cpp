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

IsotopeTableDialog::IsotopeTableDialog( QWidget* parent, const char* name )
	: KDialogBase(parent, "IsotopeTableDialog", true, i18n( "Isotope Table" ),
			KDialogBase::Apply|KDialogBase::Close|KDialogBase::Help, KDialogBase::Apply, true )
{
	QWidget *page = new QWidget( this );

	IsotopeTable *b = new IsotopeTable( page, "nb" );
	NuclideLegend *legend = new NuclideLegend( page );

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

/*	QHBoxLayout *hbox1 = new QHBoxLayout( page, 0, KDialog::spacingHint() );
	hbox1->addWidget( new QLabel( i18n( "First Element:" ), page ) );
	hbox1->addWidget( spin1 );
	QHBoxLayout *hbox2 = new QHBoxLayout( page, 0, KDialog::spacingHint() );
	hbox2->addWidget( new QLabel( i18n( "Last Element:" ), page ) );
	hbox2->addWidget( spin2 );
*/
	QGridLayout *spinLayout = new QGridLayout( page, 2, 2, 0, KDialogBase::spacingHint() );
	spinLayout->addWidget( new QLabel( i18n( "First Element:" ), page ), 0, 0 );
	spinLayout->addWidget( new QLabel( i18n( "Last Element:" ), page ), 1, 0 );
	spinLayout->addWidget( spin1, 0, 1 );
	spinLayout->addWidget( spin2, 1, 1 );

	QHBoxLayout *legendBox = new QHBoxLayout( page, 0, KDialog::spacingHint() );
	legendBox->addWidget( legend );
	legendBox->addLayout( spinLayout );	

	vbox->addWidget( b );
//	vbox->addLayout( hbox1 );
	vbox->addLayout( legendBox );
//	vbox->addLayout( hbox2 );

	setMinimumSize( 500, 450 );
	resize( minimumSize() );
	update();
}

void IsotopeTableDialog::slotHelp()
{
	emit helpClicked();
	if ( kapp )
		kapp->invokeHelp ( "isotope_table", "kalzium" );
}


IsotopeTable::IsotopeTable( QWidget *parent, const char* name ) 
	: QScrollView( parent, name )
{
	kdDebug() << "IsotopeTable()" << endl;

	m_list = KalziumDataObject::instance()->ElementList;
	m_decay = 0;
	m_start = 80;
	m_stop = 100;
	m_isoWidth = 30;

	m_highestNumberOfNeutrons = highestNeutronCount();
	m_lowestNumberOfNeutrons = lowestNeutronCount();
}

void IsotopeTable::slotDrawDecayRow( Isotope* isotope )
{
	kdDebug() << "IsotopeTable::slotDrawDecayRow()" << endl;
	if ( !isotope->alphadecay() 
			&& !isotope->betaplusdecay()
			&& !isotope->betaminusdecay() )
		return;

	m_decay = new Decay( this, isotope, m_list[isotope->protones() -1] );
	m_decay->showDecay();

	kdDebug() << "Isotope: " << isotope->neutrons() << endl;
}

void IsotopeTable::drawContents( QPainter * p, int clipx, int clipy, int clipw, int cliph ) 
{
	kdDebug() << "IsotopeTable::drawContents()" << endl;
	const int border = 10;

	resizeContents( ( m_highestNumberOfNeutrons - m_lowestNumberOfNeutrons + 1 ) * m_isoWidth + 10 
		+ m_isoWidth + border, ( m_stop - m_start + 1 ) * m_isoWidth + m_isoWidth + border );

	for ( int i = m_start; i <= m_stop; ++i )
		p->drawText( 0, ( m_stop - i ) * m_isoWidth + m_isoWidth, m_isoWidth, m_isoWidth, Qt::AlignCenter,				 m_list[i - 1]->symbol() );

	for ( int i = m_lowestNumberOfNeutrons; i <= m_highestNumberOfNeutrons; i += 2 )
	 	p->drawText( ( i - m_lowestNumberOfNeutrons ) * m_isoWidth + 10 + m_isoWidth, 0, m_isoWidth,
                                m_isoWidth, Qt::AlignCenter, QString::number( i ) );
}

int IsotopeTable::highestNeutronCount()
{
	kdDebug() << "IsotopeTable::highestNeutronCount()" << endl;

	QValueList<Element*>::const_iterator it = m_list.at( m_start - 1 );
	const QValueList<Element*>::const_iterator itEnd = m_list.at( m_stop );

	QValueList<Isotope*> isotopeList;
	QValueList<Isotope*>::const_iterator isotope;
	QValueList<Isotope*>::const_iterator isotopeEnd;

	int count = 0;

	for ( ; it != itEnd; ++it )
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

int IsotopeTable::lowestNeutronCount()
{
	kdDebug() << "IsotopeTable::lowestNeutronCount()" << endl;

	QValueList<Element*>::const_iterator it = m_list.at( m_start - 1 );
	const QValueList<Element*>::const_iterator itEnd = m_list.at( m_stop );

	QValueList<Isotope*> isotopeList;
	QValueList<Isotope*>::const_iterator isotope;
	QValueList<Isotope*>::const_iterator isotopeEnd;

	int count = 1000;
	
	for ( ; it != itEnd; ++it )
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

void IsotopeTable::setStop( int value )
{
	kdDebug() << "IsotopeTable::setStop()" << endl;

	if ( value < m_start )
	{
		m_start = value - 1;
		emitStartValue( m_start );
	}

	m_stop = value;
	updateList();
}

void IsotopeTable::setStart( int value )
{
	kdDebug() << "IsotopeTable::setStart()" << endl;	

	if ( value > m_stop )
	{
		m_stop = value + 1;
		emitStopValue( m_stop );
	}

	m_start = value;
	updateList();
}

void IsotopeTable::updateList()
{
	kdDebug() << "IsotopeTable::updateList()" << endl;

	m_highestNumberOfNeutrons = highestNeutronCount();
	m_lowestNumberOfNeutrons = lowestNeutronCount();

	if( !m_isotopeWidgetList.empty() )
	{
		QValueList<IsotopeWidget*>::const_iterator wid = m_isotopeWidgetList.begin();
		QValueList<IsotopeWidget*>::const_iterator widEnd = m_isotopeWidgetList.end();

		for ( ; wid != widEnd; ++wid )
		{
			( *wid )->hide();
			delete ( *wid );
		}

		m_isotopeWidgetList.clear();
	}	
	
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
			IsotopeWidget *widget = new IsotopeWidget( *isotope, viewport() );
	
//			connect( widget, SIGNAL( clicked( Isotope* ) ), this,
//					 SLOT( slotDrawDecayRow( Isotope* ) ) );

			addChild( widget );
			widget->resize( m_isoWidth, m_isoWidth );		
			widget->move( m_isoWidth + 10 + ( ( *isotope )->neutrons() - m_lowestNumberOfNeutrons )
				 * m_isoWidth, m_isoWidth + ( m_stop - ( *it )->number() ) * m_isoWidth );

			widget->show();
			m_isotopeWidgetList.append( widget );
		}
	}

	updateContents();
}


IsotopeWidget* IsotopeTable::getIsotopeWidget( Isotope* isotope )
{
       QValueList<IsotopeWidget*>::const_iterator it = m_isotopeWidgetList.begin();
       const QValueList<IsotopeWidget*>::const_iterator itEnd = m_isotopeWidgetList.end();

       for (; it != itEnd; ++it )
       {
               if ( ( *it )->isotope() == isotope )
                       return ( *it );
       }

       return 0;
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
}

IsotopeWidget::~IsotopeWidget()
{
}

void IsotopeWidget::paintEvent( QPaintEvent* /*e*/ )
{
//	kdDebug() << "IsotopeWidget::paintEvent()" << endl;

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

Decay::Decay( IsotopeTable* parent, Isotope* isotope, Element* element )
{
       kdDebug() << "Decay::Decay()" << endl;
       m_parent = parent;
       m_startIsotope = isotope;
       m_startElement = element;
       m_elements = KalziumDataObject::instance()->ElementList;
       buildDecayRow();
}

void Decay::buildDecayRow()
{
       	kdDebug() << "Decay::buildDecayRow()" << endl;

       	QValueList<Isotope*> tmpIsotopes;
       	QValueList<Isotope*> tmp;
       	QValueList<Isotope*>::const_iterator iso;
       	QValueList<Isotope*>::const_iterator isoEnd;

       	tmpIsotopes.append( m_startIsotope );

       	while( !tmpIsotopes.isEmpty() )
	{
               iso = tmpIsotopes.begin();
               isoEnd = tmpIsotopes.end();

               for ( ; iso != isoEnd; ++iso )
               {
                       if( ( *iso )->betaminusdecay() )
                       {
                               if ( getIsotope( (*iso)->neutrons()-1 ,(*iso)->protones()+1 ) != 0 )
                                       tmp.append( getIsotope( (*iso)->neutrons()-1,
                                                               (*iso)->protones()+1 ) );
                       }
                       if( ( *iso )->betaplusdecay() )
                       {
                               if ( getIsotope( (*iso)->neutrons()+1 ,(*iso)->protones()-1 ) != 0 )
                                       tmp.append( getIsotope( (*iso)->neutrons()+1,
                                                               (*iso)->protones()-1 ) );
                       }
                       if( ( *iso )->alphadecay() )
                       {
                               if ( getIsotope( (*iso)->neutrons()-2 ,(*iso)->protones()-2 ) != 0 )
                                       tmp.append( getIsotope( (*iso)->neutrons()-2,
                                                               (*iso)->protones()-2 ) );
                       }

                       if( m_parent->getIsotopeWidget( *iso ) != 0 )
                               m_list.append( m_parent->getIsotopeWidget( *iso ) );

                       tmpIsotopes.remove( *iso );
               }
               tmpIsotopes = tmp;
               tmp.clear();
       }
}

Isotope* Decay::getIsotope( int protones, int neutrons )
{
       kdDebug() << "Decay::getIsotope()" << endl;
       QValueList<Isotope*> tmpList = m_elements[ protones ]->isotopes();
       QValueList<Isotope*>::const_iterator it;

       if ( tmpList.empty() )
               return 0;

       for ( it = tmpList.begin(); it != tmpList.end(); ++it )
       {
               if ( ( *it )->neutrons() == neutrons )
               {
                       kdDebug() << "leaving value"<< endl;
                       return ( *it );
               }
       }
       kdDebug() << "leaving null" << endl;
       return 0;
}

NuclideLegend::NuclideLegend( QWidget* parent, const char* name ) : QWidget( parent, name )
{
//	KDialog::sizeHint();
}

void NuclideLegend::paintEvent( QPaintEvent* /*e*/ )
{
/*	QPainter p( this );
	QString text;

	p.fillRect( 10, 10, 10, 10, Qt::blue );
	p.drawRect( 10, 10, 30, 30 );
	text = i18n( "%1- %2" ).arg( QChar( 946 ) ).arg( i18n( "Decay" ) );
	p.drawText( 10 + 30, 10, text );

	p.fillRect( 10, 30, 10, 10, Qt::red );
	p.drawRect( 30,30 );
	text =  i18n( "%1+ %2" ).arg( QChar( 946) ).arg( i18n( "Decay" ) );
	p.drawText( 30, 10, text );

	p.fillRect( );
	p.drawRect( 30, 30 );
	text =  i18n( "Stable" ) 
	p.drawText( text )

	p.fillRect( 50, 10, 10, 10, Qt::yellow );
	p.drawRect( 30,30 );
	text =  i18n( "%1%2" ).arg( QChar( 945 ) ).arg( i18n( "Decay" ) );
	p.drawText( 30, 10, text );

	p.fillRect( 50, 30, 10, 10, Qt::green );
	p.drawRect( 30,30 );
	text = i18n( "Acronym of Electron Capture Decay", "EC Decay" ) 
	p.drawText( 30, 10, text );*/
}
 
#include "nuclideboard.moc"

