/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
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
#include "pse.h"
#include "prefs.h"
#include "stateofmatterdialog_impl.h"

#include <klocale.h>
#include <kdebug.h>
#include <qtooltip.h>
#include <qlayout.h>
#include <qlabel.h>

PSE::PSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : QWidget(parent, name)
{
	d = data;

	setupBlockLists();
	setupPSEElementButtonsList();

//IUPAC
	    m_IUPAClist.append( "IA");
	    m_IUPAClist.append( "IIA");
	    m_IUPAClist.append( "IIIB");
	    m_IUPAClist.append( "IVB");
	    m_IUPAClist.append( "VB");
	    m_IUPAClist.append( "VIB");
	    m_IUPAClist.append( "VIIB");
	    m_IUPAClist.append( "VIII");
	    m_IUPAClist.append( "VIII");
	    m_IUPAClist.append( "VIII");
	    m_IUPAClist.append( "IB");
	    m_IUPAClist.append( "IIB");
	    m_IUPAClist.append( "IIIA");
	    m_IUPAClist.append( "IVA");
	    m_IUPAClist.append( "VA");
	    m_IUPAClist.append( "VIA");
	    m_IUPAClist.append( "VIIA");
	    m_IUPAClist.append( "VIIIA");

//oldIUPAC
	    m_IUPACOLDlist.append( "1A");
	    m_IUPACOLDlist.append( "2A");
	    m_IUPACOLDlist.append( "3A");
	    m_IUPACOLDlist.append( "4A");
	    m_IUPACOLDlist.append( "5A");
	    m_IUPACOLDlist.append( "6A");
	    m_IUPACOLDlist.append( "7A");
	    m_IUPACOLDlist.append( "8");
	    m_IUPACOLDlist.append( "8");
	    m_IUPACOLDlist.append( "8");
	    m_IUPACOLDlist.append( "1B");
	    m_IUPACOLDlist.append( "2B");
	    m_IUPACOLDlist.append( "3B");
	    m_IUPACOLDlist.append( "4B");
	    m_IUPACOLDlist.append( "5B");
	    m_IUPACOLDlist.append( "6B");
	    m_IUPACOLDlist.append( "7B");
	    m_IUPACOLDlist.append( "0");
	
}

PSE::~PSE(){}

void PSE::updateNumeration()
{
}

void PSE::slotUpdatePSE()
{
	kdDebug() << "slotUpdatePSE(), Colorscheme: " << Prefs::colorschemebox() << endl;
	activateColorScheme( Prefs::colorschemebox() );
}

void PSE::setupBlockLists()
{
	EList::Iterator it = d->ElementList.begin();

	while ( it != d->ElementList.end() )
	{
		ElementButton *b = new ElementButton( (*it)->number() , *it, this );
		QToolTip::add(b, i18n("Name: %1").arg(i18n( (*it)->elname().utf8())) );
		b->sym = (*it)->symbol();

		if ( (*it)->block() == "s" )
			sBlockList.append( b );
		if ( (*it)->block() == "p" )
			pBlockList.append( b );
		if ( (*it)->block() == "d" )
			dBlockList.append( b );
		if ( (*it)->block() == "f" )
			fBlockList.append( b );
		++it;
	}
}

void PSE::setupPSEElementButtonsList(){}

void PSE::activateColorScheme( const int nr )
{
	kdDebug() << "Setting scheme number: " << nr << endl;

	if ( nr == 0) //normal view, no colors
	{
		kdDebug() << "Number 1 was chosen... no scheme" << endl;
		ElementButton *button;
		QColor color = Prefs::noscheme();
		for ( button = m_PSEElementButtons.first() ; button; button = m_PSEElementButtons.next() )
		{
			button->setPaletteBackgroundColor( color );
		}
	}
	else if ( nr == 1) //groups view
	{
		kdDebug() << "Number 2 was chosen... groups" << endl;
		
		ElementButton *button;
		const QColor color_1 = Prefs::group_1();
		const QColor color_2 = Prefs::group_2();
		const QColor color_3 = Prefs::group_3();
		const QColor color_4 = Prefs::group_4();
		const QColor color_5 = Prefs::group_5();
		const QColor color_6 = Prefs::group_6();
		const QColor color_7 = Prefs::group_7();
		const QColor color_8 = Prefs::group_8();

		static QString group;
		
		for ( button = m_PSEElementButtons.first() ; button; button = m_PSEElementButtons.next() )
		{
			group = button->e->group();

			if (group == QString("1")) {
				button->setPaletteBackgroundColor( color_1 );
				continue;
			}
			if (group == QString("2")){
				button->setPaletteBackgroundColor( color_2 );
				continue;
			}
			if (group == QString("3")){
				button->setPaletteBackgroundColor( color_3 );
				continue;
			}
			if (group == QString("4")){
				button->setPaletteBackgroundColor( color_4 );
				continue;
			}
			if (group == QString("5")) {
				button->setPaletteBackgroundColor( color_5 );
				continue;
			}
			if (group == QString("6")){
				button->setPaletteBackgroundColor( color_6 );
				continue;
			}
			if (group == QString("7")){
				button->setPaletteBackgroundColor( color_7 );
				continue;
			}
			if (group == QString("8")){
				button->setPaletteBackgroundColor( color_8 );
				continue;
			}
		}
	}
	else if ( nr == 2) //block view
	{
		kdDebug() << "Number 3 was chosen... blocks" << endl;
		
		ElementButton *button;
		const QColor color_s = Prefs::block_s();
		const QColor color_p = Prefs::block_p();
		const QColor color_d = Prefs::block_d();
		const QColor color_f = Prefs::block_f();

		static QString block;
		
		for ( button = m_PSEElementButtons.first() ; button; button = m_PSEElementButtons.next() )
		{
			block = button->e->block();

			if (block == QString("s")) {
				button->setPaletteBackgroundColor( color_s );
				continue;
			}
			if (block == QString("p")){
				button->setPaletteBackgroundColor( color_p );
				continue;
			}
			if (block == QString("d")){
				button->setPaletteBackgroundColor( color_d );
				continue;
			}
			if (block == QString("f")){
				button->setPaletteBackgroundColor( color_f );
				continue;
			}
		}
	}
	else if ( nr == 3) //state-of-matter
	{
		//only the elements 1 to 95 (Americium) will be calculated because
		//only for these both the boiling _and_ melting point are known.
		//The other elements will have the color color_artificial

		kdDebug() << "Number 4 was chosen... state-of-matter" << endl;
	
		somDialogImpl *somDlg = new somDialogImpl(this,  this, "som" );
		somDlg->show();
		
		ElementButton *button;
		const QColor color_solid = Prefs::color_solid();
		const QColor color_liquid = Prefs::color_liquid();
		const QColor color_vapor = Prefs::color_vapor();
		const QColor color_artificial = Prefs::color_artificial();

		static int az;
		
		for ( button = m_PSEElementButtons.first() ; button; button = m_PSEElementButtons.next() )
		{
			az = button->e->az();

			if (az == 0) {
				button->setPaletteBackgroundColor( color_solid );
				continue;
			}
			if (az == 1){
				button->setPaletteBackgroundColor( color_liquid );
				continue;
			}
			if (az == 3){
				button->setPaletteBackgroundColor( color_vapor );
				continue;
			}
			if (az == 4){
				button->setPaletteBackgroundColor( color_artificial );
				continue;
			}
		}
	}
	else if ( nr == 4) //acidic beh
	{
		kdDebug() << "Number 5 was chosen... acidbeh" << endl;
		ElementButton *button;
		const QColor color_ba = Prefs::beh_basic();
		const QColor color_ac = Prefs::beh_acidic();
		const QColor color_neu = Prefs::beh_neutral();
		const QColor color_amp = Prefs::beh_amphoteric();

		static QString acidicbeh;
		
		for ( button = m_PSEElementButtons.first() ; button; button = m_PSEElementButtons.next() )
		{
			acidicbeh = button->e->acidicbeh();

			if (acidicbeh == QString("0")) {
				button->setPaletteBackgroundColor( color_ac );
				continue;
			}
			if (acidicbeh == QString("1")){
				button->setPaletteBackgroundColor( color_ba );
				continue;
			}
			if (acidicbeh == QString("2")){
				button->setPaletteBackgroundColor( color_neu );
				continue;
			}
			if (acidicbeh == QString("3")){
				button->setPaletteBackgroundColor( color_amp );
				continue;
			}
		}
	}
}

void PSE::setDate( int date )
{
	ElementButton *button;

	for ( button = m_PSEElementButtons.first() ; button; button = m_PSEElementButtons.next() )
	{//these elements are the ones which have always been known
		const int n = button->e->number();
		if (n == 6 ||
			n == 16 ||
			n == 26 ||
			n == 29 ||
			n == 33 ||
			n == 47 ||
			n == 50 ||
			n == 51 ||
			n == 79 ||
			n == 80 ||
			n == 82 ||
			n == 83
			)
		{
            button->show();
			continue;
		}
		if ( button->e->date() > date )
			button->hide();
		else
			button->show();
	}
}

void PSE::setTemperature( const double temp )
{
	//Important: The value temp is in Kelvin, not Degree Celsius!
	kdDebug() << "PSE::setTemperature()" << endl;
	static QColor c_liquid = Prefs::color_liquid();
	static QColor c_solid = Prefs::color_solid();
	static QColor c_vapor = Prefs::color_vapor();
	static QColor c_artificial = Prefs::color_artificial();
	static QColor c_radioactive = Prefs::color_radioactive();
	
	ElementButton *button;
	for ( button = m_PSEElementButtons.first() ; button; button = m_PSEElementButtons.next() )
	{//iterate through all buttons
		const int az = button->e->az();
		if ( az == 3 || az == 4 )
		{ //check if the element is radioactive or artificial
			if ( az == 3 ) button->setPaletteBackgroundColor( c_radioactive );
			if ( az == 4 ) button->setPaletteBackgroundColor( c_artificial );
			continue;
		}

		kdDebug() << "az != 3 und 4" << endl;

		double iButton_melting = button->e->melting();
		double iButton_boiling = button->e->boiling();
		
		if ( temp < iButton_melting )
		{ //the element is solid
			button->setPaletteBackgroundColor( c_solid );
			continue;
		}
		if ( temp > iButton_melting &&
			temp < iButton_boiling )
		{ //the element is liquid
			button->setPaletteBackgroundColor( c_liquid );
			continue;
		}
		if ( temp > iButton_boiling )
		{ //the element is vaporous
			button->setPaletteBackgroundColor( c_vapor );
			continue;
		}
	}
}

RegularPSE::RegularPSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : PSE(data, parent, name)
{
	QVBoxLayout *vbox = new QVBoxLayout( this , 0 , -1 , "vbox" );
	
	m_ShortName = i18n( "Regular Periodic Table" );
	
	QGridLayout *grid = new QGridLayout( 19 , 10 );
	
	ElementButton *button;
	for ( int i = 0 ; i < 18 ; i++ )
	{
		QLabel *l = new QLabel( this );
		lList.append( l );
		grid->addWidget( l , 0 , i);
	}
	for ( button = sBlockList.first() ; button ; button = sBlockList.next() )
	{
		grid->addWidget( button , button->e->y , button->e->x-1 );
	}
	for ( button = pBlockList.first() ; button ; button = pBlockList.next() )
	{
		grid->addWidget( button , button->e->y , button->e->x-1 );
	}
	for ( button = dBlockList.first() ; button ; button = dBlockList.next() )
	{
		grid->addWidget( button , button->e->y , button->e->x-1 );
	}
	for ( button = fBlockList.first() ; button ; button = fBlockList.next() )
	{
		grid->addWidget( button , button->e->y , button->e->x-1 );
	}

	vbox->addLayout( grid );

	for (  int n=0; n<18; n++ ) grid->addColSpacing(  n, 40 );
	for (  int n=0; n<10; n++ ) grid->addRowSpacing(  n, 40 );
	
	setupPSEElementButtonsList();
	activateColorScheme( Prefs::colorschemebox() );
}

RegularPSE::~RegularPSE(){} 

void RegularPSE::updateNumeration()
{
	kdDebug() << "RegularPSE::updateNumeration()" << endl;
	LabelList::iterator it = lList.begin();
	for ( int i = 0 ; it != lList.end() ; ++it )
	{
		switch ( m_num )
		{
			case NO :
			        ( *it )->setText( "" );
				break;
			case CAS :
				( *it )->setText( QString::number( i+1 ) );
				break;
			case IUPAC :
				( *it )->setText( *( m_IUPAClist.at( i )) );
				break;
			case IUPACOLD :
				( *it )->setText( *( m_IUPACOLDlist.at( i )) );
				break;
		}
		i++;
	}
}

/**
 * this method sets up the m_PSEElementButtons-list
 **/
void RegularPSE::setupPSEElementButtonsList()
{
	ElementButton *button;
	
	for ( button = sBlockList.first() ; button ; button = sBlockList.next() )
	{
		m_PSEElementButtons.append( button );
	}
	for ( button = pBlockList.first() ; button ; button = pBlockList.next() )
	{
		m_PSEElementButtons.append( button );
	}
	for ( button = dBlockList.first() ; button ; button = dBlockList.next() )
	{
		m_PSEElementButtons.append( button );
	}
	for ( button = fBlockList.first() ; button ; button = fBlockList.next() )
	{
		m_PSEElementButtons.append( button );
	}
}

SimplifiedPSE::SimplifiedPSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : PSE(data, parent, name)
{
	d = data;
	QVBoxLayout *vbox = new QVBoxLayout( this , 0 , -1 , "vbox" );

	m_ShortName = i18n( "A Simplified Periodic Table" );
	
	QGridLayout *grid = new QGridLayout( 19 , 17  );
	
	ElementButton *button;

	for ( int i = 0 ; i < 9 ; i++ )
	{
		QLabel *l = new QLabel( this );
		lList.append( l );
		grid->addWidget( l , 0 , i);
	}
	for ( button = sBlockList.first() ; button ; button = sBlockList.next() )
	{
		grid->addWidget( button , button->e->s_y , button->e->s_x-1 );
	}
	for ( button = pBlockList.first() ; button ; button = pBlockList.next() )
	{
		grid->addWidget( button , button->e->s_y , button->e->s_x-1 );
	}
	for ( button = dBlockList.first() ; button ; button = dBlockList.next() )
	{
		button->hide();
	}
	for ( button = fBlockList.first() ; button ; button = fBlockList.next() )
	{
		button->hide();
	}
	vbox->addLayout( grid );

	for (  int n=0; n<8; n++ ) grid->addColSpacing(  n, 40 );
	for (  int n=0; n<7; n++ ) grid->addRowSpacing(  n, 40 );
	
	setupPSEElementButtonsList();
	activateColorScheme( Prefs::colorschemebox() );
}

SimplifiedPSE::~SimplifiedPSE(){}

void SimplifiedPSE::updateNumeration()
{
	LabelList::iterator it = lList.begin();
	for ( int i = 0 ; it != lList.end() ; ++it )
	{
		switch ( m_num )
		{
			case NO :
				( *it )->setText( "" );
				break;
			case CAS :
				( *it )->setText( QString::number( i+1 ) );
				break;
			case IUPAC :
				( *it )->setText( *( m_IUPAClist.at( i )) );
				break;
			case IUPACOLD :
				( *it )->setText( *( m_IUPACOLDlist.at( i )) );
				break;
		}
		i++;
		if ( i == 2 )
			i+=8;
	}
}

/**
 * this method sets up the m_PSEElementButtons-list
 **/
void SimplifiedPSE::setupPSEElementButtonsList()
{
	kdDebug() << "SimplifiedPSE::setupPSEElementButtonsList()" << endl;
	ElementButton *button;
	
	for ( button = sBlockList.first() ; button ; button = sBlockList.next() )
	{
		m_PSEElementButtons.append( button );
	}
	for ( button = pBlockList.first() ; button ; button = pBlockList.next() )
	{
		m_PSEElementButtons.append( button );
	}
}
/*

MendeljevPSE::MendeljevPSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : PSE(data, parent, name)
{
	d = data;

	m_ShortName = i18n( "Mendeleev - localized name of Russian chemist Dmitri Mendeleev","Mendeleev's Periodic Table" );
	
	QVBoxLayout *vbox = new QVBoxLayout( this , 0 , -1 , "vbox" );
	
	QGridLayout *grid = new QGridLayout( 18 , 17  );
	
	ElementButton *button;

	for ( int i = 0 ; i < 8 ; i++ )
	{
		QLabel *l = new QLabel( this );
		lList.append( l );
		grid->addWidget( l , 0 , i);
	}
	for ( button = sBlockList.first() ; button ; button = sBlockList.next() )
	{
		grid->addWidget( button , button->e->s_y , button->e->s_x-1 );
	}
	for ( button = pBlockList.first() ; button ; button = pBlockList.next() )
	{
		grid->addWidget( button , button->e->s_y , button->e->s_x-1 );
	}
	for ( button = dBlockList.first() ; button ; button = dBlockList.next() )
	{
		button->hide();
	}
	for ( button = fBlockList.first() ; button ; button = fBlockList.next() )
	{
		button->hide();
	}
	vbox->addLayout( grid );

	for (  int n=0; n<8; n++ ) grid->addColSpacing(  n, 40 );
	for (  int n=0; n<7; n++ ) grid->addRowSpacing(  n, 40 );

	setupPSEElementButtonsList();
	activateColorScheme( Prefs::colorschemebox() );
}

MendeljevPSE::~MendeljevPSE(){}

void MendeljevPSE::setupPSEElementButtonsList()
{
	kdDebug() << "MendeljevPSE::setupPSEElementButtonsList()" << endl;
}

void MendeljevPSE::updateNumeration()
{
	LabelList::iterator it = lList.begin();
	for ( ; it != lList.end() ; ++it )
	{
		( *it )->hide();
	}
}
*/

#include "pse.moc"
