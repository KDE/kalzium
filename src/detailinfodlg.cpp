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
#include <kdebug.h>

#include <qfont.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstring.h>
#include <qvbuttongroup.h>
#include <qpainter.h>
#include <qpixmap.h>

#include <iostream>
#include "elementkp.h"

#include "detailinfodlg.h"

DetailedInfoDlg::DetailedInfoDlg( const ElementInfo Eleminfo , QWidget *parent, const char *name)
    : KDialogBase(IconList, i18n("Detailed look"), Help|Default|Ok|Apply|Cancel ,Ok, parent,name, true, false)
{
	Data = Eleminfo;

	/////////////////////////////////
	overviewTab = addPage(i18n("Overview"), i18n("Overview"), BarIcon("colorize", KIcon::SizeMedium));
	QVBoxLayout *overviewLayout = new QVBoxLayout( overviewTab );
	QWidget *foo = new QWidget( overviewTab );
	QVBoxLayout *foo_layout = new QVBoxLayout( foo );

	dTab = new DetailedTab( Data ,  foo );
	overviewLayout->addWidget( foo );
	foo_layout->addWidget( dTab );

	showLegendKP = new KPushButton( i18n( "show Legend" ), this );
	connect( showLegendKP , SIGNAL( clicked() ), this , SLOT( slotShowLegend() ) );
	overviewLayout->addWidget( showLegendKP );
	
	dTab->show();
    
    /////////////////////////////////
	
	mainTab = addPage(i18n("Picture"), i18n("What does %1 look like?").arg( Data.Name.utf8() ), BarIcon("redo", KIcon::SizeMedium));
	QVBoxLayout *mainLayout = new QVBoxLayout( mainTab );
	QPixmap pic ( "/home/carsten/cvs/kdeedu/kalzium/src/elementpics/"+Data.Symbol+".jpg" );
	QLabel *test = new QLabel( mainTab );
	test->setPixmap( pic );
	mainLayout->addWidget( test );
	
    /////////////////////////////////
    energyTab = addPage(i18n("Energies"), i18n("Energyinformatino"), BarIcon("roll", KIcon::SizeMedium));
	QVBoxLayout *energyLayout = new QVBoxLayout( energyTab );
	QLabel *ENlabel = new QLabel( QString::number( Data.EN ) , energyTab );
	QLabel *Ionlabel = new QLabel( QString::number( Data.IE ) , energyTab );
	energyLayout->addWidget( ENlabel );
	energyLayout->addWidget( Ionlabel );
	
    /////////////////////////////////
    miscTab = addPage(i18n("Colors"), i18n("Miscilanious"), BarIcon("colorize", KIcon::SizeMedium));
}

void DetailedInfoDlg::slotShowLegend()
{
	kdDebug() << "Legende soll angezeigt werden" << endl;
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
	
	int h = this->height();
	int w = this->width();
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
		kdDebug() << "yo, ist TRUE" << endl;
		p.drawLine( x1 , y2 , 50 , 50 );
	}
	else kdDebug() << "no, es ist FALSE" << endl;
	
	p.end();
}

QColor DetailedTab::PSEColor( QString block ) const
{
    QColor c;
    if ( block == "s" ) c.setRgb( 213 , 233 , 231 );
    else if ( block == "d" ) c.setRgb( 200,230,160 );
    else if ( block == "p" ) c.setRgb( 238,146,138 );
    else if ( block == "f" ) c.setRgb( 190 , 190 , 190 );
    return  c;
};

#include "detailinfodlg.moc"
