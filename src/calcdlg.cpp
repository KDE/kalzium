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
    grid->addMultiCellWidget(anal_display_l , 5 , 5 , 0 , 1 );

    kmolcalc = new KMolCalc;



    for ( int n=0 ; n<118 ; n++ ) // read in the values
    {
	config.setGroup(QString::number(n+1));
	symlabel[n]=config.readEntry("Symbol", "Unknown");
	weight[n]=config.readEntry("Weight","0.0");
    }

    //
    // BEGIN - MWG
    //

    QVBoxLayout *buttonVBox = new QVBoxLayout( MWG );
    grid = new QGridLayout( buttonVBox , 8 , 3 );

    valuebox = new QGroupBox( i18n("Enter your values") , MWG , "valuebox" );
    comboboxgrid = new QGridLayout( valuebox , 6 , 6 );
    grid->addWidget( valuebox , 4 , 0 );

    general = new QLabel( i18n("General equiation:"), MWG );
    
    //FIXME insert the correct pixmap
    
    herefrom = new QLabel( i18n("Herefrom you get the 'Law of Mass Action':"), MWG );

    //FIXME insert the correct pixmap

    grid->addWidget( general, 0, 0);
    grid->addWidget( herefrom, 1, 0);

    a = new QLabel( i18n("a: ") , valuebox );
    b = new QLabel( i18n("b: ") , valuebox );
    c = new QLabel( i18n("c: ") , valuebox );
    d = new QLabel( i18n("d: ") , valuebox );
    A = new QLabel( i18n("A: ") , valuebox );
    B = new QLabel( i18n("B: ") , valuebox );
    C = new QLabel( i18n("C: ") , valuebox );
    D = new QLabel( i18n("D: ") , valuebox );

    coeff = new QLabel( i18n("Coefficient: ") , valuebox );
    conc = new QLabel( i18n("Concentration: ") , valuebox );

    QLEa = new QLineEdit( "1" , valuebox );
    QLEb = new QLineEdit( "1" , valuebox );
    QLEc = new QLineEdit( "1" , valuebox );
    QLEd = new QLineEdit( "1" , valuebox );
    QLEA = new QLineEdit( "1" , valuebox );
    QLEB = new QLineEdit( "1" , valuebox );
    QLEC = new QLineEdit( "1" , valuebox );
    QLED = new QLineEdit( "1" , valuebox );

    QCBA = new QComboBox( FALSE , valuebox );
    QCBB = new QComboBox( FALSE , valuebox );
    QCBC = new QComboBox( FALSE , valuebox );
    QCBD = new QComboBox( FALSE , valuebox );
    QCBA->insertItem( i18n("mol/L") );
    QCBB->insertItem( i18n("mol/L") );
    QCBC->insertItem( i18n("mol/L") );
    QCBD->insertItem( i18n("mol/L") );

    resultMWG = new QLabel( MWG );
    grid->addWidget(resultMWG, 5, 0);

    comboboxgrid->addWidget( QLEa, 1 , 1 );
    comboboxgrid->addWidget( QLEb, 2 , 1 );
    comboboxgrid->addWidget( QLEc, 3 , 1 );
    comboboxgrid->addWidget( QLEd, 4 , 1 );
    comboboxgrid->addWidget( QLEA, 1 , 4 );
    comboboxgrid->addWidget( QLEB, 2 , 4 );
    comboboxgrid->addWidget( QLEC, 3 , 4 );
    comboboxgrid->addWidget( QLED, 4 , 4 );

    comboboxgrid->addWidget( QCBA, 1 , 5 );
    comboboxgrid->addWidget( QCBB, 2 , 5 );
    comboboxgrid->addWidget( QCBC, 3 , 5 );
    comboboxgrid->addWidget( QCBD, 4 , 5 );

    comboboxgrid->addWidget( a, 1 , 0 );
    comboboxgrid->addWidget( b, 2 , 0 );
    comboboxgrid->addWidget( c, 3 , 0 );
    comboboxgrid->addWidget( d, 4 , 0 );
    comboboxgrid->addWidget( A, 1 , 3 );
    comboboxgrid->addWidget( B, 2 , 3 );
    comboboxgrid->addWidget( C, 3 , 3 );
    comboboxgrid->addWidget( D, 4 , 3 );

    comboboxgrid->addMultiCellWidget( coeff, 0, 0, 0, 2 );
    comboboxgrid->addMultiCellWidget( conc, 0, 0, 3, 5 );

    resultbutton = new KPushButton( i18n("Calculate"), MWG );
    QObject::connect( resultbutton, SIGNAL( clicked() ), this, SLOT( slotCalculate() ) );
    buttonVBox->addWidget( resultbutton );

    addTab(MWG, i18n("MWG"));
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

void CalcDlg::slotCalculate() 
{
    int ka, kb, kc, kd;   //the 4 coefficients
    double cA, cB, cC, cD; //the 4 concentrations

    //now I will get the entered values
    ka = QLEa->text().toInt();
    kb = QLEb->text().toInt();
    kc = QLEc->text().toInt();
    kd = QLEd->text().toInt();
    cA = QLEA->text().toFloat();
    cB = QLEB->text().toFloat();
    cC = QLEC->text().toFloat();
    cD = QLED->text().toFloat();

    double zaehler = (pow( cA , ka ) * pow( cB , kb ));
    double nenner = (pow( cC , kc ) * pow( cD , kd ));

    QString ergebnis = QString::number( zaehler/nenner );

    resultMWG->setText( ergebnis );
}

#include "calcdlg.moc"
