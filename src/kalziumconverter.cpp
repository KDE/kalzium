/***************************************************************************
                   kalziumconverter.cpp  -  description
                             -------------------
    begin                : Sunday Oct 20 2002
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
#include <kcombobox.h>
#include <klineedit.h>
#include <kdebug.h>

//QT-Includes
#include <qstring.h>
#include <qlabel.h>
#include <qslider.h>
#include <qlayout.h>
#include <qwidget.h>
#include <qvbox.h>
#include <qgrid.h>

#include "kalziumconverter.h"
#include "kalziumconverter.moc"

KConvert::KConvert (QWidget *parent, const char *name )  : QWidget (parent,name)
{
	QGridLayout *grid = new QGridLayout( this, 14,5 );

	//the rest
	ToCK = new KComboBox( this );
	FromCK = new KComboBox( this );
	ValueKLE = new KLineEdit( this );
	ResultKLE = new KLineEdit( this );
	ValueDim = new KLineEdit( this );
	ResultDim = new KLineEdit( this );

	categorieKC = new KComboBox( this );

	difflabel = new QLabel( this );
	q = new QLabel( this );
	w = new QLabel( this );
	e = new QLabel( this );
	r = new QLabel( this );

	FromSlider = new QSlider(-18,18,1,0,Qt::Horizontal,this,"slider");
	ToSlider = new QSlider(-18,18,1,0,Qt::Horizontal,this,"slider");

	grid->addWidget( categorieKC,0,0 );
	grid->addWidget( FromSlider, 1,0 );
	grid->addWidget( ToSlider, 2,0 );
	grid->addWidget( ToCK, 3,0 );
	grid->addWidget( FromCK, 4,0 );
	grid->addWidget( ValueKLE, 5,0 );
	grid->addWidget( ValueDim, 7,1 );
	grid->addWidget( ResultKLE, 7,0 );
	grid->addWidget( ResultDim, 5,1 );

	grid->addWidget( difflabel, 9,0 );
	grid->addWidget( q, 10  ,0 );
	grid->addWidget( w,10  ,1 );
	grid->addWidget( e,11 ,0 );
	grid->addWidget( r,11 ,1 );


	connect( FromSlider , SIGNAL( sliderPressed() ), this, SLOT( slotDifference() ) );
	connect( ToSlider , SIGNAL( sliderPressed() ), this, SLOT( slotDifference() ) );
	connect( FromSlider , SIGNAL( sliderReleased( ) ), this, SLOT( slotDifference() ) );
	connect( ToSlider , SIGNAL( sliderReleased() ), this, SLOT( slotDifference() ) );

    connect( ValueKLE, SIGNAL( returnPressed() ), this, SLOT(slotCalculate()) );
    connect( FromCK, SIGNAL( activated( const QString &) ), this, SLOT(slotSetResultDim(const QString&)) );
    connect( ToCK, SIGNAL( activated( const QString &) ), this, SLOT(slotSetToDim(const QString&)) );

    connect( categorieKC, SIGNAL( activated( int ) ), this, SLOT(slotAdjustKomboContent(int)) );

    slotAdjustKomboContent(0);
    slotSetToDim("Joule");
	slotSetResultDim("Joule");
}

void KConvert::slotDifference()
{
	showDifference();
}

void KConvert::showDifference()
{
	int from = FromSlider->value();
	int to = ToSlider->value();
	difflabel->setText(QString::number(from-to));

	q->setText( "von:" );
	w->setText(convertIntToMes( from ));
	e->setText( "nach:" );
	r->setText(convertIntToMes( to ) );
}

QString KConvert::convertIntToMes( int value)
{
	if ( value == 18 ) return "exa";
	if ( value == 17 ) return "10 exa";
	if ( value == 16 ) return "100 exa";
	if ( value == 15 ) return "peta";
	if ( value == 14 ) return "10 peta";
	if ( value == 13 ) return "100 peta";
	if ( value == 12 ) return "tera";
	if ( value == 11 ) return "10 tera";
	if ( value == 10 ) return "100 tera";
	if ( value == 9 ) return "giga";
	if ( value == 8 ) return "10 giga";
	if ( value == 7 ) return "100 giga";
	if ( value == 6 ) return "mega";
	if ( value == 5 ) return "10 mega";
	if ( value == 4 ) return "100 mega";
	if ( value == 3 ) return "kilo";
	if ( value == 2 ) return "hecto";
	if ( value == 1 ) return "deca";
	if ( value == -1 ) return "deci";
	if ( value == -2 ) return "centi";
	if ( value == -3 ) return "milli";
	if ( value == -4 ) return "100 micro";
	if ( value == -5 ) return "10 micro";
	if ( value == -6 ) return "micro";
	if ( value == -7 ) return "100 nano";
	if ( value == -8 ) return "10 nano";
	if ( value == -9 ) return "nano";
	if ( value == -12 ) return "pico";
	if ( value == -15 ) return "femto";
	if ( value == -18 ) return "atto";
	else return "foo";
}

double KConvert::toSI( QString value )
{
	int cat = categorieKC->currentItem();
	double temp = value.toDouble();
	if ( cat == 0 )
	{//return the value in joule
		if ( FromCK->currentItem() == 0 )  {};
		if ( FromCK->currentItem() == 1 )  {};
		if ( FromCK->currentItem() == 2 )  temp*=1.60218e-19;
		if ( FromCK->currentItem() == 3 )  {};
	}
	else if ( cat == 1 )
	{// the value as meter
		if ( FromCK->currentItem() == 0 )  temp/=1000;
		if ( FromCK->currentItem() == 1 )  {};
		if ( FromCK->currentItem() == 2 )  temp*=1000;
	}
	return temp;
}

QString KConvert::toTarget( double convert )
{
	int cat = categorieKC->currentItem();
	if ( cat == 0 )
	{
		if ( ToCK->currentItem() == 0 ) {};
		if ( ToCK->currentItem() == 1 ){};
		if ( ToCK->currentItem() == 2 ) convert*=1.60218e-19;
		if ( ToCK->currentItem() == 3 ){};

	}
	else if ( cat == 1 )
	{//Length
		if ( ToCK->currentItem() == 0 ) convert*=1000;
		if ( ToCK->currentItem() == 1 ){};
		if ( ToCK->currentItem() == 2 ) convert/=1000;
	}
	return QString::number( convert );
}

//******* Slots ******************************************************
void KConvert::slotCalculate()
{
    double toConvert = toSI( ValueKLE->text() );
    ResultKLE->setText(toTarget( toConvert ));
}

void KConvert::slotSetResultDim( const QString &dim )
{
     ValueDim->setText( dim );
}

void KConvert::slotSetToDim( const QString &dim )
{
    ResultDim->setText( dim );
}

void KConvert::slotAdjustKomboContent(int cat)
{
	FromCK->clear();
	ToCK->clear();
	switch (cat){
		case 0: //Energies
			FromCK->insertItem(i18n("Joule"));
			FromCK->insertItem(i18n("Newtonmeter"));
			FromCK->insertItem(i18n("electron-Volt"));
			FromCK->insertItem(i18n("Wattseconds"));
			ToCK->insertItem(i18n("Joule"));
			ToCK->insertItem(i18n("Newtonmeter"));
			ToCK->insertItem(i18n("electron-Volt"));
			ToCK->insertItem(i18n("Wattseconds"));
			return;
		case 1: //Length
			FromCK->insertItem(i18n("mm"));
			FromCK->insertItem(i18n("m"));
			FromCK->insertItem(i18n("km"));
			ToCK->insertItem(i18n("mm"));
			ToCK->insertItem(i18n("m"));
			ToCK->insertItem(i18n("km"));
			return;
	}
}
//#include "kalziumconvert.moc"
