/***************************************************************************

        detailinfodlg.cpp  -  description
                             -------------------
    begin                : Tue Apr 8 2003
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

#include <kaction.h>
#include <kcolorbutton.h>
#include <kdebug.h>
#include <kconfig.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kpushbutton.h>

#include <qframe.h>
#include <qfont.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstring.h>
#include <qvbuttongroup.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qregexp.h>

#include <iostream>
#include "elementkp.h"

#include "detailinfodlg.h"
#include "infodialog.h"

DetailedInfoDlg::DetailedInfoDlg( const ElementInfo Eleminfo , QWidget *parent, const char *name)
    : KDialogBase(IconList, i18n("Detailed Look"), Help|Default|Ok|Apply|Cancel ,Ok, parent,name, true, false)
{
	Data = Eleminfo;

	/////////////////////////////////
	overviewTab = addPage(i18n("Overview"), i18n("Overview"), BarIcon("colorize", KIcon::SizeMedium));
	QVBoxLayout *overviewLayout = new QVBoxLayout( overviewTab );
	QWidget *overviewWidget = new QWidget( overviewTab );
	QVBoxLayout *foo_layout = new QVBoxLayout( overviewWidget );

	dTab = new DetailedTab( Data , overviewWidget );
	overviewLayout->addWidget( overviewWidget );
	showLegendKP = new KPushButton( i18n( "Show Legend" ), overviewTab );
	connect( showLegendKP , SIGNAL( clicked() ), this , SLOT( slotShowLegend() ) );
	overviewLayout->addWidget( showLegendKP );
	foo_layout->addWidget( dTab );

	dTab->show();

    /////////////////////////////////

	mainTab = addPage(i18n("Picture"), i18n("What does %1 look like?").arg( Data.Name.utf8() ), BarIcon("redo", KIcon::SizeMedium));
	QVBoxLayout *mainLayout = new QVBoxLayout( mainTab );
	QPixmap pic ( "/home/carsten/cvs/kdeedu/kalzium/src/elementpics/"+Data.Symbol+".jpg" );
	QLabel *test = new QLabel( mainTab );
	test->setPixmap( pic );
	mainLayout->addWidget( test );

    /////////////////////////////////
    energyTab = addPage(i18n("Energies"), i18n("Energyinformation"), BarIcon("roll", KIcon::SizeMedium));
	QVBoxLayout *energyLayout = new QVBoxLayout( energyTab );
	QLabel *ENlabel = new QLabel( i18n( "Electronegativity: %1" ).arg( Data._EN ) , energyTab );
	QLabel *Ionlabel = new QLabel(i18n( "Ionizationenergie: %1" ).arg( Data._IE ) , energyTab );
	QLabel *MPlabel = new QLabel(i18n( "Meltingpoint: %1" ).arg( Data._MP ) , energyTab );
	QLabel *BPlabel = new QLabel(i18n( "Boilingpoint: %1" ).arg( Data._BP ) , energyTab );
	energyLayout->addWidget( ENlabel );
	energyLayout->addWidget( Ionlabel );
	energyLayout->addWidget( MPlabel );
	energyLayout->addWidget( BPlabel );

    /////////////////////////////////
    chemicalTab = addPage(i18n("Chemical Data"), i18n("Chemical Data"), BarIcon("colorize", KIcon::SizeMedium));
	QVBoxLayout *chemicalLayout = new QVBoxLayout( chemicalTab );
	QLabel *orbtisLabel = new QLabel( Data.orbits , chemicalTab );
	QLabel *symbolLabel = new QLabel( i18n( "Symbol: %1" ).arg( Data.Symbol ) , chemicalTab  );
	QLabel *densityLabel = new QLabel( i18n( "Density: %1").arg( Data._Density ) , chemicalTab );
	QLabel *blockLabel  = new QLabel( i18n( "Block: %1" ).arg( Data.Block ) , chemicalTab );
	QLabel *atomrad    = new QLabel( i18n( "Atomic Radius: %1" ).arg( Data._AR ) , chemicalTab );
	QLabel *atomweightLabel = new QLabel( i18n( "Atomic Weigth: %1" ).arg( Data.Weight ) , chemicalTab );
	chemicalLayout->addWidget( orbtisLabel );
	chemicalLayout->addWidget( symbolLabel );
	chemicalLayout->addWidget( densityLabel);
	chemicalLayout->addWidget( blockLabel  );
	chemicalLayout->addWidget( atomrad );
	chemicalLayout->addWidget( atomweightLabel);
    
	/////////////////////////////////
    miscTab = addPage(i18n("Miscellaneous"), i18n("Miscellaneous"), BarIcon("colorize", KIcon::SizeMedium));
	QVBoxLayout *miscLayout = new QVBoxLayout( miscTab );
	QLabel *discovered_label = new QLabel( i18n("Discovered: %1").arg(Data.date ) , miscTab );
	miscLayout->addWidget( discovered_label );
}

void DetailedInfoDlg::slotShowLegend()
{
	if ( dTab->showLegend )
	{
		dTab->showLegend = false;
		showLegendKP->setText( i18n( "Show Legend" ) );
	}
	else
	{
		dTab->showLegend = true;
		showLegendKP->setText( i18n( "Hide Legend" ) );
	}

	dTab->repaint();
}

DetailedTab::DetailedTab( ElementInfo& Eleminfo , QWidget *parent, const char *name ) : QWidget( parent, name )
{
	Data = Eleminfo;
}

void DetailedTab::paintEvent( QPaintEvent* )
{
	QPainter p;
	p.begin(this);

	//the needed values

	//int h = this->height();
	//int w = this->width();
	int h = 500;
	int w = 400;
	int dy = h/4;
	int dx = w/3;
	///////////////////

	//calculation of the corners
	int x1,x2,y1,y2;

	x1 =  w/2-dx;
	y1 =  h/2-dy;
	x2 = x1 + dx;
	y2 = y1 + dy;

	p.setBrush(Qt::SolidPattern);
	p.setBrush( PSEColor( Data.Block ));
	p.drawRect( x1 , y1 , dx , dy );

	QFont f1 ( "times", 18, QFont::Bold );
	QFont f2 ( "times", 10 );
	QFont f3 ( "times", 8, QFont::Bold );

	p.setFont( f1 );
	p.drawText( x1+dx/2 , y1+dy/2 , Data.Symbol );  //Symbol

	p.setFont( f2 );
	p.drawText( x1+5 , y2-5 ,Data.Name.utf8() );            //Name
	p.drawText( x1 , y1+dy/2-50 ,x2-x1 , 18 , Qt::AlignCenter , Data.oxstage);    //Oxidationszahlen
	p.drawText( x1+dx/2 , y2-20 , x2-x1-dx/2 , 18 , Qt::AlignRight , Data.Weight ); //Weight

	p.setFont( f3 );
	p.drawText( x1+dx/2-20 , y1+dy/2-18 , 20, 18, Qt::AlignRight , QString::number( Data.number ));

	//show or hide the legend
	if ( showLegend )
	{
		p.drawLine( x1 , y2 , 50 , 50 );
	}

	p.end();
}

QColor DetailedTab::PSEColor( const QString &block ) const
{
    QColor c;
    if ( block == "s" ) c.setRgb( 213 , 233 , 231 );
    else if ( block == "d" ) c.setRgb( 200,230,160 );
    else if ( block == "p" ) c.setRgb( 238,146,138 );
    else if ( block == "f" ) c.setRgb( 190 , 190 , 190 );
    return  c;
};

#include "detailinfodlg.moc"
