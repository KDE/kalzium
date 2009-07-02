#ifndef nuclearCalculator_H
#define nuclearCalculator_H
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

#include <element.h>
#include <isotope.h>

#include <kdebug.h>
#include <prefs.h>
#include <kalziumdataobject.h>
#include <converter.h>
#include <value.h>
#include "ui_nuclearCalculator.h"

using namespace Conversion;

enum ERROR_MODE_NUKE {
	INIT_AMT_ZERO = 0,
	FINAL_AMT_ZERO,
	HALFLIFE_ZERO,
	FINAL_AMT_GREATER
};
enum MODE_CALCULATE_NUKE {
	INIT_AMT =0,
	FINAL_AMT,
	TIME
};

/**
 * Note that all values are stored in SI units in the class.
 * FIXME documentation missing
 */
class nuclearCalculator : public QFrame
{
    Q_OBJECT

public:
    nuclearCalculator(QWidget * parent = 0);
    ~nuclearCalculator();

public slots:
    /// Calculates the initial amount
    void calculateInitAmount();

    /// Calculates the final amount
    void calculateFinalAmount();

    /// Calculates the time required
    void calculateTime();

    void elementChanged(int index);

    /// This function is called when the isotope is changed
    void isotopeChanged(int index);

    /// This function is called when the halfLife is changed
    void halfLifeChanged();

    /// This function is called when any quantity is changed
    void calculate();

    /// This function is called when the initial amount is changed in the UI
    void initAmtChanged();

    /// This function is called when the final amount is changed in the UI
    void finalAmtChanged();

    /// This function is called when the time is changed in the UI
    void timeChanged();

    /// This function is called when the slider in the ui is moved
    void sliderMoved(int);
    
    /// This function is called when the mode is changed
    void setMode(int);
    
    /// This function is called during initialisation
    void init();
    
    /// This function is called when an error occurs
	void error(int mode);
	
private:
    Ui::nuclearCalculator ui;               // The user interface

    Element m_element;                      // Current element
    Isotope m_isotope;                      // current isotope

    Value m_halfLife;                       // The halfLife
    Value m_initAmount;                     // initial amount present
    Value m_finalAmount;                    // amount after time
    Value m_time;                           // the time involved in calculation
    double m_mass;                          // the atomic mass of the isotope
    
    int m_mode;								// the mode of calculation
};

#endif // nuclearCalculator_H
