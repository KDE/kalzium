/***************************************************************************
                          elementkp.cpp  -  description
                             -------------------
    begin                : Mon Dec 10 2001
    copyright            : (C) 2001 by Carsten Niehaus
    email                : cniehaus@gmx.de
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
#include <kconfig.h>
#include <kdialog.h>
#include <khtml_part.h>
#include <klocale.h>
#include <kstatusbar.h>
#include <kstddirs.h>

//QT-Includes
#include <qfont.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qwhatsthis.h>

#include "elementkp.h"
#include "elementkp.moc"
#include "kalzium.h" 

    ElementKP::ElementKP(const QString& text, QWidget *parent, ElementInfo ElemInfo, const char *name, int
	    AElemNo, KStatusBar *zeiger)	: KPushButton(text,parent,name)

{
    ElemNo = AElemNo;
    zeigerle=zeiger;
    pmenu = new QPopupMenu();
    Data = ElemInfo; 
}

void ElementKP::mousePressEvent( QMouseEvent *mouse )
{
    if (mouse->button() == RightButton)
    {
        if (Data.Name == "0")
        {
            pmTitle = new QLabel( i18n( "Name: Unnamed") , pmenu );
        }
        else
        {
            pmTitle = new QLabel( i18n( "Name: %1").arg(i18n(Data.Name.utf8())) , pmenu );
        }
        if (Data.Weight == "0")
        {
            pmWeight = new QLabel( i18n( "Atomic Weight: Unknown") , pmenu );
        }
        else
        {
            pmWeight = new QLabel( i18n( "Atomic Weight: %1 u").arg(Data.Weight) , pmenu );
        }
        pmenu->clear();
        pmenu->insertItem( pmTitle );
        pmenu->insertSeparator();
        pmenu->insertItem( pmWeight );
        pmenu->popup( QCursor::pos() );
    }
    else
    {
        slotShowData();
    }
}

void ElementKP::lookup()
{
    KHTMLPart *html = new KHTMLPart();
    
    KConfig *mainc = KGlobal::config();
    mainc->setGroup( "WLU" );
    QString url = mainc->readEntry( "adress" ) + Data.Symbol.lower()  + ".html";

    const KURL site(url);
    html->openURL(site);
    html->show();
}

//when the mousepointer is over a button
void ElementKP::enterEvent(QEvent *)
{
    showName();	
}

//when the mousepointer leaves the button
void ElementKP::leaveEvent(QEvent *)
{
    zeigerle->message(i18n("Kalzium ")+KALZIUM_VERSION);
}

void ElementKP::showName()
{
    //shows the name now in the statusbar
    zeigerle->message(i18n(Data.Name.utf8()));
}

void ElementKP::slotShowData()
{

    KDialog *ausgabe = new KDialog ( this, "" );
    QGridLayout * grid = new QGridLayout(ausgabe, 15, 2, 8);
    if (Data.Name != "Unnamed")
    {
        QWhatsThis::add(ausgabe, i18n("In this dialog Kalzium shows you information about %1").arg(i18n(Data.Name.utf8().data())));
    }
    else
    {
        QWhatsThis::add(ausgabe, i18n("This dialog displays information about this unnamed element"));
    }
    if (Data.Name != "Unnamed") ausgabe->setCaption(i18n("About %1").arg(i18n(Data.Name.utf8())));
    else ausgabe->setCaption(i18n("About this element"));

    //this QLabel is just the "title" of the whole Dialog
    QLabel *title = new QLabel ( ausgabe );
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("helvetica", 20, QFont::Bold));
    title->setText(i18n(Data.Name.utf8()));
    grid->addWidget(title,0,1);

    //Atomic Weight
    QLabel *weightLabel = new QLabel ( ausgabe );
    if (Data.Weight == "0")
    {
        weightLabel->setText(i18n("Atomic Weight: unknown"));
    }
    else
    {
        weightLabel->setText(i18n("Atomic Weight: %1 u").arg(Data.Weight));
    }
    grid->addMultiCellWidget ( weightLabel, 2, 2, 0, 2 );

    QLabel *blockLabel = new QLabel ( ausgabe );
    blockLabel->setText(i18n("Block: %1").arg(Data.Block));
    grid->addMultiCellWidget ( blockLabel, 3, 3, 0, 2 );

    QLabel *numberLabel = new QLabel ( ausgabe );
    numberLabel->setText(i18n("Element number: %1").arg(Data.number));
    grid->addMultiCellWidget ( numberLabel, 4, 4, 0, 2 );

    //When has the element been discovered?
    QLabel *dateLabel = new QLabel (ausgabe );
    if (Data.date == "0")
    {
        dateLabel->setText(i18n("%1 was known to ancient cultures").arg(i18n(Data.Name.utf8())));
    }
    if (Data.date == "3333")
    {
        dateLabel->setText(i18n("%1 has not been discovered yet").arg(i18n(Data.Name.utf8())));
    }
    else
    {
        dateLabel->setText(i18n("%1 has been known since %2").arg(i18n(Data.Name.utf8())).arg(Data.date));
    }
    grid->addMultiCellWidget ( dateLabel, 5,5,0,2 );

    //Electronegativity
    QLabel *ENLabel = new QLabel ( ausgabe );
    if (Data.EN == -1)
    {
        ENLabel->setText(i18n("Electronegativity: Unkown"));
    }
    else
    {
        ENLabel->setText(i18n("Electronegativity: %1").arg(Data.EN));
    }
    QWhatsThis::add(ENLabel, i18n("This is the electronegativity of Pauling"));
    grid->addMultiCellWidget ( ENLabel, 6, 6, 0, 2 );

    //Symbol
    QLabel *symbolLabel = new QLabel ( ausgabe );
    if (Data.Symbol == "0")
    {
        symbolLabel->setText(i18n("Symbol: None"));
    }
    else
    {
        symbolLabel->setText(i18n("Symbol: %1").arg(Data.Symbol));
    }
    grid->addMultiCellWidget ( symbolLabel, 7, 7, 0, 2 );
    QLabel *MPLabel = new QLabel ( ausgabe );
    if (Data.MP == -1)
    {
        MPLabel->setText(i18n("Melting point: Unknown"));
    }
    else
    {
        MPLabel->setText(i18n("Melting point: %1 C").arg(-273.15+Data.MP));
    }
    grid->addMultiCellWidget ( MPLabel, 9, 9, 0, 2 );

    //Boilingpoint
    QLabel *BPLabel = new QLabel ( ausgabe );
    if (Data.BP == -1)
    {
        BPLabel->setText(i18n("Boiling point: Unknown"));
    }
    else
    {
        BPLabel->setText(i18n("Boiling point: %1 C").arg(-273.15+Data.BP));
    }
    grid->addMultiCellWidget ( BPLabel, 8, 8, 0, 2 );

    //Density
    QLabel *Density = new QLabel ( ausgabe );
    if (Data.Density == -1)
    {
        Density->setText(i18n("Density: Unknown"));
    }
    else
    {
        Density->setText(i18n("Density: %1").arg(Data.Density));
    }
    grid->addMultiCellWidget ( Density, 10, 10, 0, 2 );
    
    //Ionisationenergie
    QLabel *IonEnergie = new QLabel ( ausgabe );
    if (Data.IE == -1)
    {
        IonEnergie->setText(i18n("Ionization Energie: Unknown"));
    }
    else
    {
        IonEnergie->setText(i18n("Ionization Energie: %1").arg(Data.IE));
    }
    grid->addMultiCellWidget ( IonEnergie, 11, 11, 0, 2 );
    
    //Atomicradius
    QLabel *AtomRad = new QLabel ( ausgabe );
    if (Data.AR == -1)
    {
        AtomRad->setText(i18n("Atomic Radius: Unknown"));
    }
    else
    {
        AtomRad->setText(i18n("Atomradius: %1").arg(Data.AR));
    }
    grid->addMultiCellWidget ( AtomRad, 12, 12, 0, 2 );

    // click on this button to load webpage for element
    KPushButton *web = new KPushButton(i18n("Web &Lookup"),ausgabe);
    QObject::connect(web, SIGNAL(clicked()), this , SLOT(lookup()));
    QWhatsThis::add(web, i18n("Click on this button to open a webpage with more information about %1").arg(i18n(Data.Name.utf8())));
    grid->addWidget( web , 13 , 1 );

    //click on this button to close the Dialog
    KPushButton *exit = new KPushButton( i18n("&Close"), ausgabe );
    QObject::connect (exit, SIGNAL(clicked()), ausgabe, SLOT(hide()));
    QWhatsThis::add(exit, i18n("Click on this button to close this dialog"));
    grid->addWidget( exit , 14 , 1 );

    ausgabe->show();
}

