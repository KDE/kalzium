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
#include <klocale.h>
#include <kdebug.h>
#include <kdialog.h>
#include <qtooltip.h>
#include <qstring.h>
#include <qlayout.h>

PSE::PSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : QWidget(parent, name)
{
	d = data;

	setupBlockLists();

}

PSE::~PSE()
{
}

void PSE::updatePSE()
{
	kdDebug() << "PSE::updatePSE()" << endl;

	this->update();
	//todo
}

void PSE::setupBlockLists()
{
	EList::Iterator it = d->ElementList.begin();

	while ( it != d->ElementList.end() )
	{
		/*
		 * ElementButton( int number, Element *el, QWidget *parent)
		 */
		ElementButton *b = new ElementButton( (*it)->number() , *it, this );
		QToolTip::add(b, i18n("Name: %1").arg((*it)->name().utf8()));
		connect( b, SIGNAL( num(int) ), this , SLOT( slotElementClicked(int) ) );
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

void PSE::slotElementClicked(int num)
{
	kdDebug() << "click auf " << num  << endl;
	KDialog *smallInfoDlg = new KDialog( this);
	smallInfoDlg->setCaption(QString::number(num));
	smallInfoDlg->show();
}

RegularPSE::RegularPSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : PSE(data, parent, name)
{
	QVBoxLayout *vbox = new QVBoxLayout( this , 0 , -1 , "vbox" );
	
	QGridLayout *grid = new QGridLayout( 18 , 10 );
	
	ElementButton *button;
	for ( button = sBlockList.first() ; button ; button = sBlockList.next() )
	{
		grid->addWidget( button , button->e->y-1 , button->e->x-1 );
	}
	for ( button = pBlockList.first() ; button ; button = pBlockList.next() )
	{
		grid->addWidget( button , button->e->y-1 , button->e->x-1 );
	}
	for ( button = dBlockList.first() ; button ; button = dBlockList.next() )
	{
		grid->addWidget( button , button->e->y-1 , button->e->x-1 );
	}
	for ( button = fBlockList.first() ; button ; button = fBlockList.next() )
	{
		grid->addWidget( button , button->e->y-1 , button->e->x-1 );
	}

	vbox->addLayout( grid );

	for (  int n=0; n<18; n++ ) grid->addColSpacing(  n, 40 );
	for (  int n=0; n<10; n++ ) grid->addRowSpacing(  n, 40 );

}

RegularPSE::~RegularPSE()
{}

void RegularPSE::updatePSE()
{
	this->update();
}

SimplifiedPSE::SimplifiedPSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : PSE(data, parent, name)
{
	d = data;
	QVBoxLayout *vbox = new QVBoxLayout( this , 0 , -1 , "vbox" );
	
	QGridLayout *grid = new QGridLayout( 18 , 17  );
	
	ElementButton *button;

	for ( button = sBlockList.first() ; button ; button = sBlockList.next() )
	{
		grid->addWidget( button , button->e->s_y-1 , button->e->s_x-1 );
	}
	for ( button = pBlockList.first() ; button ; button = pBlockList.next() )
	{
		grid->addWidget( button , button->e->s_y-1 , button->e->s_x-1 );
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
}

SimplifiedPSE::~SimplifiedPSE()
{
}

MendeljevPSE::MendeljevPSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : PSE(data, parent, name)
{
	d = data;
	QVBoxLayout *vbox = new QVBoxLayout( this , 0 , -1 , "vbox" );
	
	QGridLayout *grid = new QGridLayout( 18 , 17  );
	
	ElementButton *button;

	for ( button = sBlockList.first() ; button ; button = sBlockList.next() )
	{
		grid->addWidget( button , button->e->s_y-1 , button->e->s_x-1 );
	}
	for ( button = pBlockList.first() ; button ; button = pBlockList.next() )
	{
		grid->addWidget( button , button->e->s_y-1 , button->e->s_x-1 );
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
}

MendeljevPSE::~MendeljevPSE()
{
}

#include "pse.moc"
