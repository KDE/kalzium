/***************************************************************************
 *   Copyright (C) 2009     by Kashyap R Puranik, kashthealien@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QDialog>

#include <KActionCollection>

#include "ui_calculator.h"
#include "nuclearCalculator.h"
#include "gasCalculator.h"
#include "concCalculator.h"
#include "titrationCalculator.h"
#include "molcalcwidget.h"

#include <config-kalzium.h>
#ifdef HAVE_FACILE
#include <eqchemview.h>
#endif

/**
 * This widget implements the body of the calculator widget,
 * various calculators like the nuclear Calculator will be added to this.
 *
 * @author Kashyap R Puranik
 */
class calculator : public QDialog
{
    Q_OBJECT

public:
    /*
     * The class constructor and destructor, takes in a Widget as parent
     */
    calculator(QWidget *parent = nullptr);             // constructor
    ~ calculator();                             // destructor

private:
    Ui::calculator ui;                          // The user interface
    KActionCollection * m_actionCollection;

    // These are the various calculator widgets that will be added to this calculator

    nuclearCalculator * m_nuclearCalculator;    // The nuclear calculator
    gasCalculator *     m_gasCalculator;        // The gas calculator
    concCalculator *    m_concCalculator;       // The concentration calculator
    titrationCalculator *    m_titraCalculator;       // The concentration calculator
    MolcalcWidget *     m_moleCalculator;       // The molecular mass calculator
#ifdef HAVE_FACILE
    EQChemDialog *      m_equationBalancer;     // The equation balancer
#endif
protected slots:
    /**
     * invoke the help of the correct chapter
     */
    virtual void slotHelp();
private slots:

    // occurs when an tree item is selected, opens the corresponding calculator
    void slotItemSelection(QTreeWidgetItem *item);
public slots:
};

#endif // CALCULATOR_H
