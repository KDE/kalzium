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

#include "kalziumconverter.h"

KConvert::KConvert (QWidget *parent, const char *name )  : KalziumConvert (parent,name)
{
    connect( ValueKLE, SIGNAL( returnPressed() ), this, SLOT(slotCalculate()) );
    connect( FromCK, SIGNAL( activated( const QString &) ), this, SLOT(slotSetResultDim(const QString&)) );
    connect( ToCK, SIGNAL( activated( const QString &) ), this, SLOT(slotSetToDim(const QString&)) );
    connect( categorieKC, SIGNAL( activated( int ) ), this, SLOT(slotAdjustKomboContent(int)) );

    slotAdjustKomboContent(0);
    slotSetToDim("Joule");
    slotSetResultDim("Joule");

    FromSlider = new QSlider(-18,18,1,0,Qt::Horizontal,this,"slider");
    ToSlider = new QSlider(-18,18,1,0,Qt::Horizontal,this,"slider");
    FromSlider->hide(); ToSlider->show();
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

void KConvert::slotSetResultDim( const QString &dim ) const
{
    ValueDim->setText( dim );
}

void KConvert::slotSetToDim( const QString &dim ) const
{
    ResultDim->setText( dim );
}

void KConvert::showSlider()
{
    FromSlider->show();
    ToSlider->show();
}

void KConvert::hideSlider()
{
//    FromSlider->hide();
//    ToSlider->hide();
}

void KConvert::slotAdjustKomboContent(int cat)
{
    FromCK->clear();
    ToCK->clear();
    switch (cat){
	case 0: //Energies
	    hideSlider();
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
	    hideSlider();
	    FromCK->insertItem(i18n("mm"));
	    FromCK->insertItem(i18n("m"));
	    FromCK->insertItem(i18n("km"));
	    ToCK->insertItem(i18n("mm"));
	    ToCK->insertItem(i18n("m"));
	    ToCK->insertItem(i18n("km"));
	    return;
	case 2:
	    showSlider();
	default:
	    return;
    }
}
//#include "kalziumconvert.moc"
