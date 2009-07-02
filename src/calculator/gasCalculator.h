#ifndef gasCalculator_H
#define gasCalculator_H
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

#include "ui_gasCalculator.h"

#define R 0.08206

using namespace Conversion;

enum ERROR_TYPE_GAS {
    VOL_ZERO = 0,
};

/**
 * FIXME
 */
class gasCalculator : public QFrame
{
    Q_OBJECT

public:
    gasCalculator(QWidget * parent = 0);
    ~gasCalculator();

public slots:
    void calculatePressure(void);        // Calculates the Pressure
    void calculateVol(void);             // Calculates the Volume
    void calculateTemp(void);        // Calculates the Temperature
    void calculateMoles(void);           // Calculates the number of moles
    void calculateMass(void);        // Calculates the mass of substance
    void calculateMolarMass(void);       // Calculates the molar mass of the substance

    // Functions ( slots ) that occur on changing a value
    void volChanged(void);               // occurs when the volume is changed
    void tempChanged(void);          // occurs when the temperature is changed
    void pressureChanged(void);          // occurs when the pressure is changed
    void massChanged(void);              // occurs when the mass is changed
    void molesChanged(double value);    // occurs when the number of moles is changed
    void molarMassChanged(double value);     // occurs when the molar mass is changed
    void Vand_aChanged(void);        // occurs when Vander Val's constant a is changed
    void Vand_bChanged(void);        // occurs when Vander Val's constant b is changed
    void calculate(void);            // occurs when any quantity is changed
    void error(int);

    void debug(void);
private:
    Ui::gasCalculator ui;               // The user interface

    double m_moles;                     // Number of moles
    double m_molarMass;                 // molarMass
    Value m_mass;                       // mass
    Value m_temp;                       // Temperature
    Value m_pressure;                   // pressure
    Value m_vol;                        // volume
    Value m_Vand_b;                     // vander val's constant b

    //( Unit conversion library not available for the following quantities)
    double m_Vand_a;                    // Vander val's constant a
};

#endif // gasCalculator_H
