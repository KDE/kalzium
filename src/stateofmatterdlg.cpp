/***************************************************************************
                         stateofmatterdlg.cpp  -  description
                             -------------------
    begin                : Wed Feb 06 2002 
    copyright            : (C) 2002 by Carsten Niehaus 
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
#include <klocale.h>

//QT-Includes
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qslider.h>
#include <qfont.h>
#include <qlabel.h>
#include <qcolor.h>
#include <qlcdnumber.h>

#include <qinputdialog.h>

#include <kpushbutton.h>
#include <kconfig.h>

#include "elementkp.h"
#include "stateofmatterdlg.h"
#include "stateofmatterdlg.moc"
#include "kalzium.h"


#include <iostream.h>

StateOfMatterDlg::StateOfMatterDlg (QWidget *parent, const char *name, Kalzium *kalzium_tmp)  : KDialog (parent,name)
{
	kalzium = kalzium_tmp;
	this->setCaption(i18n("Temperatures"));
	QGridLayout *tempgrid = new QGridLayout(this, 10 , 4, 8);


	///////////////////////////////////////////
	// LCDs

	celsiusLCD = new QLCDNumber( 4, this, "celsiusLCD");
	kelvinLCD = new QLCDNumber( 4, this, "kelvinLCD");
	fahrenheitLCD = new QLCDNumber( 4, this, "fahrenheitLCD");

	QWhatsThis::add(celsiusLCD, i18n("This LCD shows you the current temperature in degree celsius"));
	QWhatsThis::add(kelvinLCD, i18n("This LCD shows you the current temperature in kelvin"));
	QWhatsThis::add(fahrenheitLCD, i18n("This LCD shows you the current temperature in degree fahrenheit"));

	tempgrid->addWidget(celsiusLCD, 2, 0 );
	tempgrid->addWidget(kelvinLCD, 4, 0 );
	tempgrid->addWidget(fahrenheitLCD, 6, 0 );

	///////////////////////////////////////////
	// the other QLabel

	QLabel *titletext = new QLabel( this );
	titletext->setText(i18n("State of matter"));
	titletext->setFont(QFont("helvetica", 20, QFont::Bold));
	tempgrid->addMultiCellWidget(titletext , 0 , 0 , 0 , 2 , Qt::AlignCenter );

	QLabel *info = new QLabel( this );
	info->setText(i18n("The colors indicate:"));
	info->setFont(QFont("helvetica", 12, QFont::Bold));
	tempgrid->addMultiCellWidget(info , 1 , 1 , 1 , 2 , Qt::AlignCenter );

	QLabel *solinfo = new QLabel( this );
	solinfo->setText(i18n("solid:"));
	tempgrid->addWidget(solinfo , 2 , 1 , Qt::AlignRight );

	QLabel *liqinfo = new QLabel( this );
	liqinfo->setText(i18n("liquid:"));
	tempgrid->addWidget(liqinfo , 3 , 1 , Qt::AlignRight );

	QLabel *gasinfo = new QLabel( this );
	gasinfo->setText(i18n("vapor:"));
	tempgrid->addWidget(gasinfo , 4 , 1 , Qt::AlignRight );
	
    QLabel *artiinfo = new QLabel( i18n("artificial"), this );
	tempgrid->addWidget(artiinfo , 5 , 1 , Qt::AlignRight );
    
    QLabel *radiaainfo = new QLabel( i18n("radioactive"), this );
	tempgrid->addWidget(radiaainfo , 6 , 1 , Qt::AlignRight );

	celsius = new QLabel( this );
	fahrenheit = new QLabel( this );
	kelvin = new QLabel( this );
	celsius->setText(i18n("Degree Celsius"));
	kelvin->setText(i18n("Kelvin"));
	fahrenheit->setText(i18n("Degree Fahrenheit"));
	tempgrid->addWidget(celsius, 3 , 0, Qt::AlignCenter );
	tempgrid->addWidget(kelvin, 5 , 0 , Qt::AlignCenter );
	tempgrid->addWidget(fahrenheit, 7 , 0 , Qt::AlignCenter );

	/////////////////////////////////////////////////////
	// 3 KPushButton

	solidbutton = new KPushButton(this); 
	tempgrid->addWidget(solidbutton, 2, 2 );
	liquidbutton = new KPushButton(this); 
	tempgrid->addWidget(liquidbutton, 3, 2 );
	vaporbutton = new KPushButton(this);
	tempgrid->addWidget(vaporbutton, 4, 2 );
	liquidbutton->show();
    KPushButton *artib = new KPushButton(this);
    tempgrid->addWidget(artib, 5,2);
    KPushButton *radiob = new KPushButton(this);
    tempgrid->addWidget(radiob, 6,2);

	//////////////////////////////////////////////////////
	// the colors will indicate the state of matter

	main_config=KGlobal::config();  
	main_config->setGroup("Colors");

	color_solid = main_config->readColorEntry("solid");
	color_vapor = main_config->readColorEntry("vapor");
	color_liquid = main_config->readColorEntry("liquid");

	liquidbutton->setPalette(color_liquid);
	solidbutton->setPalette(color_solid);
	vaporbutton->setPalette(color_vapor);
	radiob->setPalette(main_config->readColorEntry("radioactive"));
	artib->setPalette(main_config->readColorEntry("artificial"));

	tempS = new QSlider ( -4500, 273, 1, -25 , QSlider::Vertical, this, "tempSlider" );
	QWhatsThis::add(tempS, i18n("Use this slider to see what state of matter a certain elements has at a given temperature"));
	tempS->setTickmarks(QSlider::Below );
	tempS->setTickInterval( 100 );
	tempgrid->addMultiCellWidget(tempS , 1 , 8 , 3 , 3 ) ;
	connect( tempS, SIGNAL(valueChanged(int)),  SLOT(tempbeh()));

	tempbeh();

	//Standard conditions
	StdCondButton = new KPushButton( i18n("Standard Conditions"), this );
	tempgrid->addWidget(StdCondButton , 9 , 0 );
	QObject::connect(StdCondButton, SIGNAL(clicked()), this , SLOT(slotStdCond()));
	QWhatsThis::add(StdCondButton, i18n("By clicking on this button you will see how the elemens are under standard conditions. As you can see, some are artificial and some are radioactive."));

	//the QInputDialog
	InputButton = new KPushButton( i18n("Enter Temperature"), this );
	tempgrid->addWidget(InputButton, 9, 1 );
	QObject::connect(InputButton, SIGNAL(clicked()), this , SLOT(slotSetTemp()));
	QWhatsThis::add(InputButton, i18n("If you click on this button you can enter a temperature"));
	tempS->setValue( -25 );
}

void StateOfMatterDlg::tempbeh()
{
	int tempC = -1*( tempS->value() );
	celsiusLCD->display(tempC);
	kelvinLCD->display(tempC+273);
	fahrenheitLCD->display(((tempC*9)/5)+32);
	for (int i = 0; i < 114; i++)
	{
		if (tempC < (kalzium->element[i]->Data.MP-273))
			kalzium->element[i]->setPalette( color_solid );
		if (tempC > (kalzium->element[i]->Data.MP-273) && tempC < (kalzium->element[i]->Data.BP-273) )
			kalzium->element[i]->setPalette( color_liquid );
		if ( tempC > (kalzium->element[i]->Data.BP)-273)
			kalzium->element[i]->setPalette( color_vapor );
        if (kalzium->element[i]->Data.az == "3")
            kalzium->element[i]->setPalette( QPalette(main_config->readColorEntry("radioactive")));
        if (kalzium->element[i]->Data.az == "4")
            kalzium->element[i]->setPalette( QPalette(main_config->readColorEntry("artificial")));
	}
}

void StateOfMatterDlg::slotSetTemp()
{
	bool ok = FALSE;
	int temp = QInputDialog::getInteger( i18n( "Enter Temperature"), i18n( "Temperature in Celsius:"), -tempS->value(), -275, 5000, 1, &ok, this );
	if (ok) tempS->setValue(-temp);
}

void StateOfMatterDlg::slotStdCond()
{
 	tempS->setValue( -25 );
}
