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
#include <ksimpleconfig.h>
#include <kstandarddirs.h>

//QT-Includes
#include <qdialog.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qsplitter.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qstringlist.h>

#include "kalziumplotdialogimpl.h"
#include "kalziumplotwidget.h"

KalziumPlotDialogImpl::KalziumPlotDialogImpl ( QWidget *parent, const char *name )
 : KDialogBase( KDialogBase::Plain, i18n("Kalzium Plotdialog"), Close|User1, Close, parent , name )
{
	loadData(); 
	
	QFrame *page = plainPage();
	setButtonText( User1 , i18n( "&Plot" ) );
	QVBoxLayout *vlay = new QVBoxLayout( page, 0, spacingHint() );

////
	fromSpin = new QSpinBox( 1, 108 , 1 , page );
	toSpin = new QSpinBox( 2, 109 , 1 , page );
	whatKCB = new KComboBox( false , page , "whatKCB" );
	whatKCB->insertItem( i18n( "Atomic Weight" ) );
	whatKCB->insertItem( i18n( "Electronegativity" ) );
	whatKCB->insertItem( i18n( "Meltingpoint" ) );
	whatKCB->insertItem( i18n( "Ionisation Energy" ) );
	whatKCB->insertItem( i18n( "Atomic Radius" ) );
	whatKCB->insertItem( i18n( "Boilingpoint" ) );
whatKCB->insertItem( i18n( "Density" ) );
////
	
	QSplitter *plotW = new QSplitter( page );
	pw = new KalziumPlotWidget( 0.0,0.0,0.0,0.0,plotW );
	
	elementsKLV = new KListView( plotW, "elementsKLV" );
	elementsKLV->addColumn( i18n( "Number" ) );
	elementsKLV->addColumn( i18n( "Symbol" ) );
	elementsKLV->addColumn( i18n( "Name" ) );
	
	vlay->addWidget( fromSpin );
	vlay->addWidget( toSpin );
	vlay->addWidget( whatKCB );
	vlay->addWidget( plotW );

	resize( 500, 500 );
}

void KalziumPlotDialogImpl::slotUser1()
{
	startPlotting();
}

void KalziumPlotDialogImpl::paintEvent( QPaintEvent * /*e*/ ) 
{
	pw->update();
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
	double posX = 0.0, posY = 0.0;
	
	doubleList *ptrDoubleList;
	int kind = whatKCB->currentItem();
	ptrDoubleList= dl.at( kind );

	for ( int i = from ; i < to+1 ; ++i )
	{
		getPositions( i , posY , ptrDoubleList );
		posX = i;
		elements->addPoint( new DPoint( posX , posY ) );
		kdDebug() << ( int )posX << " " << ( int ) posY << endl;
	}

	pw->setLimits(0.0 , to , 0.0 , 200.0 );
	pw->addObject( elements );
}

void KalziumPlotDialogImpl::loadData()
{
	KSimpleConfig config (locate("data", "kalzium/kalziumrc"));
	QStringList dataKinds;
	dataKinds.append( "Weight" );
	dataKinds.append( "EN" );
	dataKinds.append( "MP" );
	dataKinds.append( "IE" );
	dataKinds.append( "AR" );
	dataKinds.append( "BP" );
	dataKinds.append( "Density" );
	
    for ( int i = 0 ; i < 7 ; ++i )
	{
		dl.append( new doubleList );
	}
	
	doubleList *ptrDoubleList = dl.first();
    for ( QStringList::Iterator strIt = dataKinds.begin() ; strIt != dataKinds.end() ; ++strIt )
	{
		for ( int e = 1 ; e < 110 ; ++e )
		{
			 config.setGroup(QString::number( e ));
			 ptrDoubleList->append( config.readDoubleNumEntry( *strIt, -1 ) );
		}
		ptrDoubleList = dl.next();
	}
}

void KalziumPlotDialogImpl::getPositions( int num , double& y , doubleList *liste)
{
	y = *( liste->at( num ) );
}

const int KalziumPlotDialogImpl::getMax()
{
}

void KalziumPlotDialogImpl::initPlotObjects(){}

void KalziumPlotDialogImpl::keyPressEvent( QKeyEvent *e ) 
{
	kdDebug() << "KalziumPlotDialogImpl::keyPressEvent()" << endl;
	switch ( e->key() ) {
		case Key_Plus:
		case Key_Equal:
			slotZoomIn();
			break;
		case Key_Minus:
		case Key_Underscore:
			slotZoomOut();
			break;
	}
}

void KalziumPlotDialogImpl::slotZoomIn() 
{
	kdDebug() << "KalziumPlotDialogImpl::slotZoomIn()" << endl;
	if ( pw->x2() > 0.4 ) {
		pw->setLimits( 0.95*pw->x(), 0.95*pw->x2(), 0.95*pw->y(), 0.95*pw->y2() );
		pw->update();
	}
}

void KalziumPlotDialogImpl::slotZoomOut() 
{
	kdDebug() << "KalziumPlotDialogImpl::slotZoomOut()" << endl;
	if ( pw->x2() < 50.0 ) {
		pw->setLimits( 1.05*pw->x(), 1.05*pw->x2(), 1.05*pw->y(), 1.05*pw->y2() );
		pw->update();
	}
}


#include "kalziumplotdialogimpl.moc"
