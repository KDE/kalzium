/***************************************************************************
                         csvdialogimpl.cpp  -  description
                             -------------------
    begin                : June 2003
	copyright            : (C) 2003 by Carsten Niehaus                     
    email                : cniehaus@kde.org
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//KDE-Includes
#include <kdebug.h>
#include <klocale.h>
#include <kcombobox.h>
#include <klistview.h>
#include <kdialogbase.h>

//QT-Includes
#include <qdialog.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qsplitter.h>
#include <qmessagebox.h>

#include "kalziumplotdialogimpl.h"
#include "kalziumplotwidget.h"

KalziumPlotDialogImpl::KalziumPlotDialogImpl ( QWidget *parent, const char *name )
 : KDialogBase( KDialogBase::Plain, i18n("Kalzium Plotdialog"), Close|User1, Close, parent , name )
{
	QFrame *page = plainPage();
	setButtonText( User1 , i18n( "&Plot" ) );
	QVBoxLayout *vlay = new QVBoxLayout( page, 0, spacingHint() );

////
	fromSpin = new QSpinBox( 1, 108 , 1 , page );
	toSpin = new QSpinBox( 2, 109 , 1 , page );
	whatKCB = new KComboBox( false , page , "whatKCB" );
	whatKCB->insertItem( i18n( "Atomic Weight" ) );
	whatKCB->insertItem( i18n( "Density" ) );
	whatKCB->insertItem( i18n( "Ionisation Energy" ) );
////
	
	QSplitter *plotW = new QSplitter( page );
	pw = new KalziumPlotWidget( -46.0, 46.0, -46.0, 46.0, plotW );
	
	elementsKLV = new KListView( plotW, "elementsKLV" );
	elementsKLV->addColumn( i18n( "Number" ) );
	elementsKLV->addColumn( i18n( "Symbol" ) );
	elementsKLV->addColumn( i18n( "Name" ) );
	
	vlay->addWidget( fromSpin );
	vlay->addWidget( toSpin );
	vlay->addWidget( whatKCB );
	vlay->addWidget( plotW );

	resize( 500, 500 );

	initPlotObjects();
}

void KalziumPlotDialogImpl::slotUser1()
{
	startPlotting();
}

void KalziumPlotDialogImpl::startPlotting()
{
	int from, to;
	from = fromSpin->value();
	to = toSpin->value();
	if ( to < from )
	{
		QMessageBox::warning(  this, i18n( "Kalzium: Plotting Dialog" ),
				i18n( "The \"from\"-number has to be bigger than the \"to\"-number." ) );
		return;
	}
	
	kdDebug() << "Start plotting from " << from << " to " << to << endl;

	KPlotObject *elements = new KPlotObject( "Elements" , "cyan2" , KPlotObject::POINTS, 6, KPlotObject::CIRCLE);
	double posX = 1.0, posY = 9.3;
	
	for ( int i = from ; i < to+1 ; ++i )
	{
		getPositions( posX , posY );
		elements->addPoint( new DPoint( posX , posY ) );
		kdDebug() << ( int )posX << " " << ( int ) posY << endl;
	}

	pw->addObject( elements );
}

void KalziumPlotDialogImpl::getPositions( double& x , double& y )
{
	x = 23.0;
	y = 20.3;
}

void KalziumPlotDialogImpl::initPlotObjects(){}

#include "kalziumplotdialogimpl.moc"
