/***************************************************************************
                         calcdlg.cpp  -  description
                             -------------------
    begin                : Thu Feb 2002 
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
#include <ksimpleconfig.h>
#include <klocale.h>
#include <kstddirs.h>

//QT-Includes
#include <qlabel.h>
#include <qstring.h>
#include <kglobalsettings.h>
#include <klocale.h>
#include <qlayout.h>
#include <qmultilinedit.h>
#include <qwhatsthis.h>
#include <qinputdialog.h>
#include <qtabwidget.h>
#include <qlineedit.h>

#include "calcdlg.h"
#include "../calculations/kmoledit.h"

#include "iostream.h"
#include "math.h"

#include "calcdlg.moc"

CalcDlg::CalcDlg (QWidget *parent, const char *name )  : QTabWidget (parent,name)
{
        MWG = new QWidget(this);
        KMol = new QWidget(this);
        QGridLayout *grid = new QGridLayout ( KMol, 5, 1 );

        //accepts text input until return is pressed 
        formula = new QLineEdit (KMol);
        formula->setFocus();
        connect(formula, SIGNAL(returnPressed()), this, SLOT(calc()));
        grid->addMultiCellWidget( formula , 1 ,1,0,1 );
        
        QLabel* formula_l = new QLabel (i18n("Formula"), KMol, "formula");
        grid->addWidget( formula_l , 0 , 0 );

        //mol weight output field
        result = new QLabel (KMol);
        result->setFrameStyle (QFrame::Panel | QFrame::Sunken);
        result->setLineWidth (2);
        result->setBackgroundMode(PaletteBase);
        grid->addMultiCellWidget( result, 4, 4 ,0 ,1 );

        QLabel* result_l = new QLabel (i18n("Mw"), KMol, "result");
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

        QLabel* anal_display_l = new QLabel (i18n("Elemental Composition (%)"), KMol, "displaylabel");
        anal_display_l->setFont(KGlobalSettings::generalFont());
        grid->addMultiCellWidget(anal_display_l , 5 , 5 , 0 , 1 );

       kmolcalc = new KMolCalc;
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));


    
    for ( int n=0 ; n<118 ; n++ )
    {
            config.setGroup(QString::number(n+1));
            symlabel[n]=config.readEntry("Symbol", "Unknown");
            weight[n]=config.readEntry("Weight","0.0");
    }

    //MWG
	QGridLayout *calcgrid = new QGridLayout(MWG, 10 , 4, 8);

        //Produkte
	prodconc1 = new QLineEdit (MWG, "");
	prodconc2 = new QLineEdit (MWG, "");
	prodconc3 = new QLineEdit (MWG, "");

        //Edukte
	eduktconc1 = new QLineEdit (MWG, "");
	eduktconc2 = new QLineEdit (MWG, "");
	eduktconc3 = new QLineEdit (MWG, "");
        
        //Produktekoeff
	prodcoeff1 = new QLineEdit (MWG, "");
	prodcoeff2 = new QLineEdit (MWG, "");
	prodcoeff3 = new QLineEdit (MWG, "");

        //Eduktekoeff
	eduktcoeff1 = new QLineEdit (MWG, "");
	eduktcoeff2 = new QLineEdit (MWG, "");
	eduktcoeff3 = new QLineEdit (MWG, "");

        QLabel *title = new QLabel ( i18n("Law of mass action:"), MWG, "" );
        resultMWG = new QLabel ("test" , MWG, "" );
        
        QLabel *edukt = new QLabel ( i18n("Eductconcentrations:"), MWG, "" );
        QLabel *produkt = new QLabel ( i18n("Produktcocentrations:"), MWG, "" );
        QLabel *eduktkoef = new QLabel ( i18n("Productcoef:"), MWG, "" );
        QLabel *produktkoef = new QLabel ( i18n("Productcoef:"), MWG, "" );

	KPushButton *calculateMWG = new KPushButton ( i18n("Calculate"), MWG );
	QObject::connect(calculateMWG, SIGNAL(clicked()), this , SLOT(slotCalc()));

	calcgrid->addWidget(title, 0 , 4 );
	
        calcgrid->addWidget(edukt, 2 , 0 );
	calcgrid->addWidget(produkt, 3 , 0 );
	calcgrid->addWidget(eduktkoef, 2 , 4 );
	calcgrid->addWidget(produktkoef, 3 , 4 );
        calcgrid->addWidget(calculateMWG, 4 , 2 );
        calcgrid->addWidget(resultMWG, 4 , 4 );
        
        calcgrid->addWidget(prodconc1, 3 , 1 );
        calcgrid->addWidget(prodconc2, 3 , 2 );
        calcgrid->addWidget(prodconc3, 3 , 3 );
        calcgrid->addWidget(eduktconc1, 2 , 1 );
        calcgrid->addWidget(eduktconc2, 2 , 2 );
        calcgrid->addWidget(eduktconc3, 2 , 3 );
        
        calcgrid->addWidget(prodcoeff1, 3 , 5 );
        calcgrid->addWidget(prodcoeff2, 3 , 6 );
        calcgrid->addWidget(prodcoeff3, 3 , 7 );
        calcgrid->addWidget(eduktcoeff1, 2 , 5 );
        calcgrid->addWidget(eduktcoeff2, 2 , 6 );
        calcgrid->addWidget(eduktcoeff3, 2 , 7 );
        
        addTab(MWG, i18n("MWG"));
        addTab(KMol, i18n("KMol"));
}

void CalcDlg::slotCalc()
{
    //reading the concentrations
    e1 = eduktconc1->text().toDouble();
    e2 = eduktconc2->text().toDouble();
    e3 = eduktconc3->text().toDouble();
    p1 = eduktconc1->text().toDouble();
    p2 = eduktconc2->text().toDouble();
    p3 = eduktconc3->text().toDouble();

    ecoeff1 = eduktcoeff1->text().toDouble();
    ecoeff1 = eduktcoeff1->text().toDouble();
    ecoeff1 = eduktcoeff1->text().toDouble();
    pcoeff1 = prodcoeff1->text().toDouble();
    pcoeff2 = prodcoeff2->text().toDouble();
    pcoeff3 = prodcoeff3->text().toDouble();

    double tmp1 = (pow(e1,ecoeff1) * pow(e1,ecoeff2) * pow(e3,ecoeff3) );
    double tmp2 = (pow(p1,pcoeff1) * pow(p1,pcoeff2) * pow(p3,pcoeff3) );
    
    resultMWG->setText(QString::number(tmp1/tmp2));
}

void CalcDlg::calc() {
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
void CalcDlg::callEditor() {
  KMolEdit* kmoledit = new KMolEdit(0, "kmoledit", kmolcalc);
  kmoledit->exec();
  delete kmoledit;
}


/** 
 * Clear all text entry / result fields.
 */
void CalcDlg::clear() {
  formula->clear();
  result->clear();
  anal_display->clear();
}

