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
#include <kpushbutton.h>
#include <klocale.h>
#include <kcombobox.h>
#include <klineedit.h>
#include <kcombobox.h>

//QT-Includes
#include <qlabel.h>
#include <qstring.h>

#include "kalziumconverter.h"


KConvert::KConvert (QWidget *parent, const char *name )  : KalziumConvert (parent,name)
{
    connect( ValueKLE, SIGNAL( returnPressed() ), this, SLOT(slotCalculate()) );
    connect( FromCK, SIGNAL( activated( const QString &) ), this, SLOT(slotSetResultDim(const QString&)) );
    connect( ToCK, SIGNAL( activated( const QString &) ), this, SLOT(slotSetToDim(const QString&)) );
}


long KConvert::toSI( QString value )
{
    int cat = categorieKC->currentItem();
    if ( cat == 0 )
    {//return the value in joule
        //Code fehlt
        
    }
    else if ( cat == 1 )
    {//return the value as meter
        if ( FromCK->currentText() == "nm" ) return value.toLong()*1000000000;
        if ( FromCK->currentText() == "mm" ) return value.toLong()*1000;
    }
}

QString KConvert::toTarget( long convert )
{
    int cat = categorieKC->currentItem();
    if ( cat == 0 )
    {//return the value in joule
        //Code fehlt
        
    }
    else if ( cat == 1 )
    {//return the value as meter
        if ( ToCK->currentText() == "nm" ) convert/=1000000000;
        if ( ToCK->currentText() == "mm" ) convert/=1000;
    }
    return QString::number( convert );
}

//******* Slots ******************************************************
void KConvert::slotCalculate()
{
    long toConvert = toSI( ValueKLE->text() );
    ResultKLE->setText(toTarget( toConvert ));
}

void KConvert::slotSetResultDim( const QString &dim )
{
    ValueDim->setText( dim );
}

void KConvert::slotSetToDim( const QString &dim )
{
    ValueDim->setText( dim );
}
//#include "kalziumconvert.moc"
