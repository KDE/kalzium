/***************************************************************************
                         kalziumplotdialogimpl.cpp  -  description
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

ChemicalElement::ChemicalElement(){}

KalziumPlotDialogImpl::KalziumPlotDialogImpl ( QWidget *parent, const char *name )
 : KDialogBase( KDialogBase::Plain, i18n("Plot Dialog"), Close|User1, Close, parent , name )
{
	loadData(); 
	
	QFrame *page = plainPage();
	setButtonText( User1 , i18n( "&Plot" ) );
	QVBoxLayout *vlay = new QVBoxLayout( page, 0, spacingHint() );

////
	fromSpin = new QSpinBox( 1, 109 , 1 , page );
	toSpin = new QSpinBox( 2, 110 , 1 , page );
	toSpin->setValue( 110 );
	whatKCB = new KComboBox( false , page , "whatKCB" );
	whatKCB->insertItem( i18n( "Atomic Weight" ) );
	whatKCB->insertItem( i18n( "Electronegativity" ) );
	whatKCB->insertItem( i18n( "Melting Point" ) );
	whatKCB->insertItem( i18n( "Ionization Energy" ) );
	whatKCB->insertItem( i18n( "Atomic Radius" ) );
	whatKCB->insertItem( i18n( "Boiling Point" ) );
	whatKCB->insertItem( i18n( "Density" ) );
////
	
	QSplitter *plotW = new QSplitter( page );
	pw = new KalziumPlotWidget( 0.0,0.0,0.0,0.0,plotW );
	pw->setXAxisLabel(  i18n(  "Atomic Number" ) );
	
	elementsKLV = new KListView( plotW, "elementsKLV" );
	elementsKLV->setAlternateBackground( elementsKLV->paletteBackgroundColor().dark() );
	elementsKLV->setSorting( -1 );
	elementsKLV->setSelectionMode( QListView::NoSelection );
	
	elementsKLV->setRootIsDecorated( true );
	elementsKLV->addColumn( i18n( "Symbol" ) );
	elementsKLV->addColumn( i18n( "Name" ) );
	elementsKLV->addColumn( i18n( "Number" ) );
	
	vlay->addWidget( fromSpin );
	vlay->addWidget( toSpin );
	vlay->addWidget( whatKCB );
	vlay->addWidget( plotW );

	resize( 500, 500 );

	slotUser1(); //at the beginning the user already sees one plot
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
	
	KPlotObject *elements = new KPlotObject( "Elements" , "cyan2" , KPlotObject::POINTS, 6, KPlotObject::CIRCLE);
	double posX = 0.0, posY = 0.0;

	ChemicalElement *elem = elementsPtrList.at(from-1);
	
	for ( int i = from ; i < to+1 ; ++i )
	{
		getPositions( i , posY , elem );
		posX = i;
		elements->addPoint( new DPoint( posX , posY ) );
		elem = elementsPtrList.next();
	}

 	pw->setLimits( from-1, to+1 , getMin( from, to ) , getMax( from , to ) );
	pw->clearObjectList();
	pw->addObject( elements );

	setPlotAxis( whatKCB->currentItem() );
	updateListview();
}

void KalziumPlotDialogImpl::setPlotAxis( const int id )
{
	QString yLabel;

	switch ( id )
	{
		case 0:
			yLabel =  i18n( "Atomic Weight [u]" );
			break;
		case 1:
			yLabel =  i18n( "Electronegativity" );
			break;
		case 2:
			yLabel =  i18n( "Melting point [K]" );
			break;
		case 3:
			yLabel =  i18n( "Ionization Energy [kJ]" );
			break;
		case 4:
			yLabel =  i18n( "Atomic Radius [pm]" );
			break;
		case 5:
			yLabel =  i18n( "Boiling point [K]" );
			break;
		case 6:
			yLabel =  i18n( "Density g/cm<sup>3</sup>" );
			break;
	}
	
	pw->setYAxisLabel( yLabel );
}

void KalziumPlotDialogImpl::updateListview()
{
	elementsKLV->clear();

	int from = fromSpin->value()-1;
	int to = toSpin->value();
	QString name, sym;
	
	ChemicalElement *elem;
	KListViewItem *childItem;
	KListViewItem *item;

	
	for ( elem = elementsPtrList.at(to-1 ) ; to!=from ; elem = elementsPtrList.prev() )
	{
		name = elem->name;
		sym = elem->symbol;
		item = new KListViewItem( elementsKLV, sym , name , QString::number( elem->number ) );
		for ( int i = 0 ; i < 7 ; i++ )
		{
			childItem = new KListViewItem( item );
			switch ( i )
			{
				case 0:
					childItem->setText( 0 , i18n( "Atomic Weight" ));
					childItem->setText( 1 , i18n( "%1 u" ).arg( QString::number( elem->weight ) ) );
					break;
				case 1:
					childItem->setText( 0 , i18n( "EN" ) );
					childItem->setText( 1 , QString::number( elem->en ) );
					break;
				case 2:
					childItem->setText( 0 , i18n( "Melting Point" ) );
					childItem->setText( 1 , QString::number( elem->mp ) );
					break;
				case 3:
					childItem->setText( 0 , i18n( "Ion. Energy" ) );
					childItem->setText( 1 , QString::number( elem->ie ) );
					break;
				case 4:
					childItem->setText( 0 , i18n( "Atomic Radius" ) );
					childItem->setText( 1 , QString::number( elem->ar ) );
					break;
				case 5:
					childItem->setText( 0 , i18n( "Boiling Point" ) );
					childItem->setText( 1 , QString::number( elem->bp) );
					break;
				case 6:
					childItem->setText( 0 , i18n( "Density" ) );
					childItem->setText( 1 , QString::number( elem->density ) );
					break;
			}
		}
		to--;
	}
}

void KalziumPlotDialogImpl::loadData()
{
	KSimpleConfig config (locate("data", "kalzium/kalziumrc"));
	
	for ( int n = 1 ; n < 111 ; ++n )
	{
		elementsPtrList.append( new ChemicalElement );
	}

	ChemicalElement *elem;
	int i = 1;
	for ( elem = elementsPtrList.first(); elem ; elem = elementsPtrList.next() )
	{
		config.setGroup(QString::number( i ));

		elem->name = config.readEntry( "Name" , "Unknown" );
		elem->symbol = config.readEntry( "Symbol" , "Unknown" );
		elem->block = config.readEntry( "Block" , "Unknown" );
		elem->orbit = config.readEntry( "Orbits" , "Unknown" );
		elem->ox = config.readEntry( "Ox" , "Unknown" );

		elem->weight = config.readDoubleNumEntry( "Weight", -1 ) ;
		elem->en =config.readDoubleNumEntry( "EN", -1 ) ;
		elem->mp =config.readDoubleNumEntry( "MP", -1 ) ;
		elem->bp =config.readDoubleNumEntry( "BP", -1 ) ;
		elem->density =config.readDoubleNumEntry( "Density", -1 ) ;
		elem->ie =config.readDoubleNumEntry( "IE", -1 ) ;
		elem->ar =config.readDoubleNumEntry( "AR", -1 ) ;
		
		elem->acidbeh = config.readNumEntry( "acidbeh" , -1 );
		elem->az = config.readNumEntry( "az" , -1 );
		elem->date = config.readNumEntry( "date" , -1 );
		elem->group = config.readNumEntry( "Group" , -1 );
		elem->biological = config.readNumEntry( "biological" , -1 );
		elem->number = i;

		i++;
	}
}

void KalziumPlotDialogImpl::getPositions( int /* num */ , double& y , ChemicalElement* element)
{
	int id = whatKCB->currentItem();
	switch ( id )
	{
		case 0:
			y = element->weight;
			break;
		case 1:
			y = element->en;
			break;
		case 2:
			y = element->mp;
			break;
		case 3:
			y = element->ie;
			break;
		case 4:
			y = element->ar;
			break;
		case 5:
			y = element->bp;
			break;
		case 6:
			y = element->density;
			break;
	}
}

doubleList KalziumPlotDialogImpl::getDoubleList( const int f , const int t )
{
	ChemicalElement *element = elementsPtrList.at(f-1);
	int id = whatKCB->currentItem();
	doubleList data;
	double y;
	
	for ( int i = f-1 ; i < t ; ++i )
	{
		switch ( id )
		{
			case 0:
				y = element->weight;
				break;
			case 1:
				y = element->en;
				break;
			case 2:
				y = element->mp;
				break;
			case 3:
				y = element->ie;
				break;
			case 4:
				y = element->ar;
				break;
			case 5:
				y = element->bp;
				break;
			case 6:
				y = element->density;
				break;
		}
		data.append( y );
		element = elementsPtrList.next();
	}
	return data;
}

const double KalziumPlotDialogImpl::getMax( const int f, const int t)
{
	double y;
	doubleList data = getDoubleList( f , t );

	doubleList::Iterator it = data.begin();
	
	y = *it;
	for ( ; it != data.end() ; ++it )
	{
		if ( y < *it )
			y = *it;
//X 		kdDebug() << "*it ist nun: " << *it << endl;
//X 		kdDebug() << "y ist nun: " << y << endl;
	}
	
	kdDebug() << "y ist am Ende: " << y << endl;

	y=y*1.1;
	return y;
}

const double KalziumPlotDialogImpl::getMin( const int f, const int t)
{
	double y;
	doubleList data = getDoubleList( f , t );

	doubleList::Iterator it = data.begin();
	
	y = *it;
	for ( ; it != data.end() ; ++it )
	{
		if ( y > *it )
			y = *it;
//X 		kdDebug() << "*it ist nun: " << *it << endl;
//X 		kdDebug() << "y ist nun: " << y << endl;
	}
	
	kdDebug() << "y ist am Ende: " << y << endl;

	y=y*0.9;
	return y;
}

void KalziumPlotDialogImpl::keyPressEvent( QKeyEvent *e ) 
{
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
	if ( pw->x2() > 0.4 ) {
		pw->setLimits( 0.95*pw->x(), 0.95*pw->x2(), 0.95*pw->y(), 0.95*pw->y2() );
		pw->update();
	}
}

void KalziumPlotDialogImpl::slotZoomOut() 
{
	if ( pw->x2() < 50.0 ) {
		pw->setLimits( 1.05*pw->x(), 1.05*pw->x2(), 1.05*pw->y(), 1.05*pw->y2() );
		pw->update();
	}
}


#include "kalziumplotdialogimpl.moc"
