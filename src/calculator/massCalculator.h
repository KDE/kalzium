#ifndef massCalculator_H
#define massCalculator_H
/***************************************************************************
    copyright            : Kashyap R Puranik
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

#include <kdebug.h>
#include <prefs.h>
// #include <chemistry.h>

#include "ui_massCalculator.h"

class QTimer;
/**
 * This file implements the mass calculator class. This uses the FreeChem library
 * which is a derivative of the gnome chemistry utilities. This file implements
 * only the GUI part of the class. The other implementations are in the library.
 */
 
// using namespace gcu;

class massCalculator : public QFrame
{
    Q_OBJECT

public:
    massCalculator(QWidget * parent = 0);
    ~massCalculator();

public slots:
    // Sub-routines involved in calculations of the unit
    void calculateMass(void);               // Calculates the amount of solute

    // Functions ( slots ) that occur on changing a value
    // Sub routines which act as quantity change event handlers

    void formulaChanged(void);            // occurs when the amount of solute is changed

private:
	
    void debug(void);				 // used for debugging purposes only
    void error(int);                 // outputs error messages on the screen
    void tick(void);
    
//    gcu::Formula* m_formula;
    Ui::massCalculator ui;           // The user interface
	QTimer* m_timer;					 // single shot timer
//	Formula* m_formula;
};

#endif // massCalculator_H

