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
#include <kstddirs.h>

//QT-Includes
#include <qlabel.h>
#include <qtabwidget.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qmultilineedit.h>
#include <qstring.h>

#include "calcdlg.h"
#include "../calculations/kmoledit.h"

#include "math.h"

CalcDlg::CalcDlg (QWidget *parent, const char *name )  : QTabWidget (parent,name)
{
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));

    KMol = new QWidget(this);
    QGridLayout *grid = new QGridLayout ( KMol, 5, 1 );

    //accepts text input until return is pressed 
    formula = new QLineEdit (KMol);
    formula->setFocus();
    connect(formula, SIGNAL(returnPressed()), this, SLOT(calc()));
    grid->addMultiCellWidget( formula , 1 ,1,0,1 );

    QLabel* formula_l = new QLabel (i18n("Formula:"), KMol, "formula");
    grid->addWidget( formula_l , 0 , 0 );

    //mol weight output field
    result = new QLabel (KMol);
    result->setFrameStyle (QFrame::Panel | QFrame::Sunken);
    result->setLineWidth (2);
    result->setBackgroundMode(PaletteBase);
    grid->addMultiCellWidget( result, 4, 4 ,0 ,1 );

    QLabel* result_l = new QLabel (i18n("Mw:"), KMol, "result");
    grid->addWidget( result_l, 3 , 0 );

    //calculate button
    KPushButton* calculate = new KPushButton (i18n("Calculate"), KMol, "calculate");
    calculate->setDefault(TRUE);
    connect(calculate, SIGNAL(clicked()), this, SLOT(calc()));
    connect(formula, SIGNAL(returnPressed()), calculate, SLOT(animateClick()));
    grid->addWidget(calculate , 2, 0 );

    //clears FORMULA and RESULT fields
    KPushButton* clear_fields = new KPushButton (i18n("Clear"), KMol, "clear");
    connect(clear_fields, SIGNAL(clicked()), this, SLOT(clear()));
    grid->addWidget(clear_fields, 2,1 );

    //E.A. display field
    anal_display = new QMultiLineEdit (KMol, "display");
    anal_display->setReadOnly(1);
    anal_display->setFocusPolicy(QWidget::NoFocus);
    grid->addMultiCellWidget(anal_display, 6, 6, 0, 1 );

    QLabel* anal_display_l = new QLabel (i18n("Elemental composition (%):"), KMol, "displaylabel");
    grid->addMultiCellWidget(anal_display_l , 5 , 5 , 0 , 1 );

    kmolcalc = new KMolCalc;



    for ( int n=0 ; n<118 ; n++ ) // read in the values
    {
	config.setGroup(QString::number(n+1));
	symlabel[n]=config.readEntry("Symbol", "Unknown");
	weight[n]=config.readEntry("Weight","0.0");
    }
    addTab(KMol, i18n("KMol"));
}

//******* Slots ******************************************************

void CalcDlg::calc() 
{
    QString compound(formula->text());
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
    result->setText(mw);
    anal_display->setText(ea);
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
    formula->clear();
    result->clear();
    anal_display->clear();
}

#include "calcdlg.moc"
