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

DetailedInfoDlg::DetailedInfoDlg( ElementInfo Eleminfo , QWidget *parent, const char *name)
    : KDialogBase(IconList, i18n("Detailed look"), Help|Default|Ok|Apply|Cancel ,Ok, parent,name, true, false)
{
	Data = Eleminfo;
	overviewTab = addPage(i18n("Overview"), i18n("Overview"), BarIcon("colorize", KIcon::SizeMedium));
	QVBoxLayout *overviewLayout = new QVBoxLayout( overviewTab );
	DetailedTab* dTab = new DetailedTab( Data ,  overviewTab );
	overviewLayout->addWidget( dTab );
	dTab->show();
    
	mainTab = addPage(i18n("Picture"), i18n("What does %1 look like?").arg( Data.Name.utf8() ), BarIcon("redo", KIcon::SizeMedium));
	QVBoxLayout *mainLayout = new QVBoxLayout( mainTab );
	QPixmap pic ( "/home/carsten/cvs/kdeedu/kalzium/src/elementpics/"+Data.Symbol+".jpg" );
	QLabel *test = new QLabel( mainTab );
	test->setPixmap( pic );
	mainLayout->addWidget( test );
	
    energyTab = addPage(i18n("Colors"), i18n("Energieinformation"), BarIcon("roll", KIcon::SizeMedium));
	
    miscTab = addPage(i18n("Colors"), i18n("Miscilanious"), BarIcon("colorize", KIcon::SizeMedium));
    
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
	QFont f2 ( "times", 12 );
	
	p.setFont( f1 );
	p.drawText( x1+dx/2 , y1+dy/2 , Data.Symbol );
	
	p.setFont( f2 );
	p.drawText( x1+5 , y2-5 ,Data.Name.utf8() );
	p.drawText( x1+5 , y2-18,Data.Weight );
	p.drawText( x1+dx/2-20 , y1+dy/2-30 , Data.oxstage);
	p.drawText( x1+dx/2 , y2-20 , x2-x1-dx/2 , 18 , Qt::AlignRight , Data.Weight );

//X 	p.drawText( x1, y1, "ol" );
//X 	p.drawText( x1, y2, "ul" );
//X 	p.drawText( x2, y1, "or" );
//X 	p.drawText( x2, y2, "ur" );

	p.drawLine(x2,y2,x2+100,y2+100);
	p.drawText( x2+95 , y2+101 ,i18n( "Atomic weight: %1 u" ).arg( Data.Weight ));
	
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
