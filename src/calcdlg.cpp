/***************************************************************************
                         calcdlg.cpp  -  description
                             -------------------
    begin                : Thu Dec 2001 
    copyright            : (C) 2001, 2002 by Carsten Niehaus                     
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
#include <ksimpleconfig.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <klineedit.h>

//QT-Includes
#include <qlabel.h>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qstring.h>

#include "calcdlg.h"
#include "kmolui.h"
#include "kalziumconverter.h"
#include "../calculations/kmoledit.h"

CalcDlg::CalcDlg (QWidget *parent, const char *name )  : QTabWidget (parent,name)
{
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));

    KMol = new KMolUI(this);

    //accepts text input until return is pressed 
    connect(KMol->formula, SIGNAL(returnPressed()), this, SLOT(calc()));

    //calculate button
    connect(KMol->calculate, SIGNAL(clicked()), this, SLOT(calc()));
    connect(KMol->formula, SIGNAL(returnPressed()), KMol->calculate, SLOT(animateClick()));

    //clears FORMULA and RESULT fields
    connect(KMol->clear_fields, SIGNAL(clicked()), this, SLOT(clear()));

    kmolcalc = new KMolCalc;

    for ( int n=0 ; n<109 ; n++ ) // read in the values
    {
        config.setGroup(QString::number(n+1));
        symlabel[n]=config.readEntry("Symbol", "Unknown");
        weight[n]=config.readEntry("Weight","0.0");
    }

    KConvert *convert = new KConvert(this);
    
    addTab(KMol, i18n("KMol"));
    addTab(convert, i18n("Conversions"));
}

//******* Slots ******************************************************

void CalcDlg::calc() 
{
    QString compound(KMol->formula->text());
    if (compound.isEmpty()) {
	clear();
	return;
    }
    QString errors(kmolcalc->readFormula(compound));
    QString mw, ea;
    double weight = kmolcalc->getWeight();
    if (errors == "OK") {
	mw.setNum(weight);
	ea = kmolcalc->getEmpFormula() + " :\n" + kmolcalc->getEA();
    } else {
	mw = "???";
	ea = i18n("ERROR: \n") + errors + "\n";
    }
    KMol->result->setText(mw);
    KMol->anal_display->setText(ea);
}

/**
* Call the built in editor to edit the definitions file.
*/
void CalcDlg::callEditor() 
{
    KMolEdit* kmoledit = new KMolEdit(0, "kmoledit", kmolcalc);
    kmoledit->exec();
    delete kmoledit;
}

/** 
* Clear all text entry / result fields.
*/
void CalcDlg::clear() 
{
    KMol->formula->clear();
    KMol->result->clear();
    KMol->anal_display->clear();
}

#include "calcdlg.moc"
