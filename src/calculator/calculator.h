#ifndef CALCULATOR_H
#define CALCULATOR_H

/***************************************************************************
    Author               : Kashyap R Puranik
    email                : kashthealien@gmail.com

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdialog.h>
#include "ui_calculator.h"

#include "nuclearCalculator.h"
#include "gasCalculator.h"
#include "concCalculator.h"
//#include "massCalculator.h"
#include "molcalcwidget.h"
#include <kactioncollection.h>
//#ifdef HAVE_FACILE
#include <eqchemview.h>
//#endif

/**
 * This widget implements the molecular calculator
 * You can perform basic calculations using this
 * @author Kashyap R Puranik
 */
class calculator : public KDialog
{
    Q_OBJECT

public:
    calculator(QWidget *parent = 0);             // constructor
    ~ calculator();                             // destructor

private:
    Ui::calculator ui;                          // The user interface
    KActionCollection * m_actionCollection;
    nuclearCalculator * m_nuclearCalculator;    // The nuclear calculator
    gasCalculator *     m_gasCalculator;        // The gas calculator
    concCalculator *    m_concCalculator;       // The concentration calculator
//    massCalculator *    m_massCalculator;		// The molecular mass calculator
    MolcalcWidget *     m_moleCalculator;       // The molecular mass calculator
//#ifdef HAVE_FACILE
    EQChemDialog *      m_equationBalancer;     // The equation balancer
//#endif
private slots:
    void slotItemSelection(QTreeWidgetItem *item);  // occurs when an tree item is selected
public slots:
};

#endif
