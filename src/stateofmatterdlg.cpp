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


StateOfMatterDlg::StateOfMatterDlg (QWidget *parent, const char *name, Kalzium *kalzium_tmp)  : KDialog (parent,name)
{
	kalzium = kalzium_tmp;
	this->setCaption(i18n("Temperatures"));
	main_layout = new QGridLayout(this, 10 , 4, 8);


	///////////////////////////////////////////
	// LCDs

	celsiusLCD = new QLCDNumber( 4, this, "celsiusLCD");
	kelvinLCD = new QLCDNumber( 4, this, "kelvinLCD");
	fahrenheitLCD = new QLCDNumber( 4, this, "fahrenheitLCD");

	QWhatsThis::add(celsiusLCD, i18n("This LCD shows you the current temperature in degrees Celsius."));
	QWhatsThis::add(kelvinLCD, i18n("This LCD shows you the current temperature in Kelvin."));
	QWhatsThis::add(fahrenheitLCD, i18n("This LCD shows you the current temperature in degrees Fahrenheit."));

	main_layout->addWidget(celsiusLCD, 2, 0 );
	main_layout->addWidget(kelvinLCD, 4, 0 );
	main_layout->addWidget(fahrenheitLCD, 6, 0 );

	///////////////////////////////////////////
	// the other QLabel

	QLabel *titletext = new QLabel( this );
	titletext->setText(i18n("State of matter"));
	titletext->setFont(QFont("helvetica", 20, QFont::Bold));
	main_layout->addMultiCellWidget(titletext , 0 , 0 , 0 , 2 , Qt::AlignCenter );

	QLabel *info = new QLabel( i18n("The colors indicate:"), this );
	info->setFont(QFont("helvetica", 12, QFont::Bold));
	main_layout->addMultiCellWidget(info , 1 , 1 , 1 , 2 , Qt::AlignCenter );

	QLabel *solinfo = new QLabel( i18n("solid:"),this );
	main_layout->addWidget(solinfo , 2 , 1 , Qt::AlignRight );

	QLabel *liqinfo = new QLabel( i18n("liquid:"), this );
	main_layout->addWidget(liqinfo , 3 , 1 , Qt::AlignRight );

	QLabel *gasinfo = new QLabel(("vapor:"), this );
	main_layout->addWidget(gasinfo , 4 , 1 , Qt::AlignRight );
	
    QLabel *artiinfo = new QLabel( i18n("artificial"), this );
	main_layout->addWidget(artiinfo , 5 , 1 , Qt::AlignRight );
    
    QLabel *radiaainfo = new QLabel( i18n("radioactive"), this );
	main_layout->addWidget(radiaainfo , 6 , 1 , Qt::AlignRight );

	celsius = new QLabel( this );
	fahrenheit = new QLabel( this );
	kelvin = new QLabel( this );
	celsius->setText(i18n("Degrees Celsius"));
	kelvin->setText(i18n("Kelvin"));
	fahrenheit->setText(i18n("Degrees Fahrenheit"));
	main_layout->addWidget(celsius, 3 , 0, Qt::AlignCenter );
	main_layout->addWidget(kelvin, 5 , 0 , Qt::AlignCenter );
	main_layout->addWidget(fahrenheit, 7 , 0 , Qt::AlignCenter );

	/////////////////////////////////////////////////////
	// 3 KPushButton

	solidbutton = new KPushButton(this); 
	main_layout->addWidget(solidbutton, 2, 2 );
	liquidbutton = new KPushButton(this); 
	main_layout->addWidget(liquidbutton, 3, 2 );
	vaporbutton = new KPushButton(this);
	main_layout->addWidget(vaporbutton, 4, 2 );
	liquidbutton->show();
    artib = new KPushButton(this);
    main_layout->addWidget(artib, 5,2);
    radiob = new KPushButton(this);
    main_layout->addWidget(radiob, 6,2);

	//////////////////////////////////////////////////////
	// the colors will indicate the state of matter

	main_config=KGlobal::config();  
	main_config->setGroup("Colors");

	color_artificial = main_config->readColorEntry("artificial");
	color_liquid = main_config->readColorEntry("liquid");
	color_artificial = main_config->readColorEntry("radioactive");
	color_solid = main_config->readColorEntry("solid");
	color_vapor = main_config->readColorEntry("vapor");


	tempS = new QSlider ( -4500, 273, 1, -25 , QSlider::Vertical, this, "tempSlider" );
	QWhatsThis::add(tempS, i18n("Use this slider to see what state of matter a certain element has at a given temperature."));
	tempS->setTickmarks(QSlider::Below );
	tempS->setTickInterval( 100 );
	main_layout->addMultiCellWidget(tempS , 1 , 8 , 3 , 3 ) ;
	connect( tempS, SIGNAL(valueChanged(int)),  SLOT(tempbeh()));

	//Standard conditions
	StdCondButton = new KPushButton( i18n("Standard Conditions"), this );
	main_layout->addWidget(StdCondButton , 9 , 0 );
	QObject::connect(StdCondButton, SIGNAL(clicked()), this , SLOT(slotStdCond()));
	QWhatsThis::add(StdCondButton, i18n("By clicking on this button you will see how the elements are under standard conditions. As you can see, some are artificial and some are radioactive."));

	//the QInputDialog
	InputButton = new KPushButton( i18n("Enter Temperature"), this );
	main_layout->addWidget(InputButton, 9, 1 );
	QObject::connect(InputButton, SIGNAL(clicked()), this , SLOT(slotSetTemp()));
	QWhatsThis::add(InputButton, i18n("If you click on this button you can enter a temperature."));
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
			kalzium->element[i]->setPaletteBackgroundColor( color_solid );
		if (tempC > (kalzium->element[i]->Data.MP-273) && tempC < (kalzium->element[i]->Data.BP-273) )
			kalzium->element[i]->setPaletteBackgroundColor( color_liquid );
		if ( tempC > (kalzium->element[i]->Data.BP)-273)
			kalzium->element[i]->setPaletteBackgroundColor( color_vapor );
        if (kalzium->element[i]->Data.az == "3")
            kalzium->element[i]->setPaletteBackgroundColor( color_radioactive );
        if (kalzium->element[i]->Data.az == "4")
            kalzium->element[i]->setPaletteBackgroundColor( color_artificial );
	}
}

void StateOfMatterDlg::setButtonsColor()
{
    color_artificial = main_config->readColorEntry("artificial");
	color_liquid = main_config->readColorEntry("liquid");
	color_radioactive = main_config->readColorEntry("radioactive");
	color_solid = main_config->readColorEntry("solid");
	color_vapor = main_config->readColorEntry("vapor");

	artib->setPaletteBackgroundColor( color_artificial );
	liquidbutton->setPaletteBackgroundColor( color_liquid );
	radiob->setPaletteBackgroundColor( color_radioactive );
	solidbutton->setPaletteBackgroundColor( color_solid );
	vaporbutton->setPaletteBackgroundColor( color_vapor );

}

void StateOfMatterDlg::slotSetTemp()
{
	bool ok = false;
	int temp = QInputDialog::getInteger( i18n( "Enter Temperature"), i18n( "Temperature in Celsius:"), -tempS->value(), -275, 5000, 1, &ok, this );
	if (ok) tempS->setValue(-temp);
}

void StateOfMatterDlg::slotStdCond()
{
 	tempS->setValue( -25 );
}
