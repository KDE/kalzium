/***************************************************************************
    copyright            : (C) 2009 by Kashyap R Puranik
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

#ifndef GASCALCULATOR_H
#define GASCALCULATOR_H

#include <KIcon>

#include <Plasma/PopupApplet>
#include <Plasma/Svg>
#include <kunitconversion/converter.h>
#include <kunitconversion/value.h>

#include "ui_gasCalculatorConfig.h"

#define R 0.08206

namespace Plasma{
    class ComboBox;
    class TextEdit;
    class Label;
    class Slider;
    class SpinBox;
    class RadioButton;
    class PushButton;
}

enum ERROR_TYPE_GAS {
    RESET_GAS_MESG = 0,
    VOL_ZERO,
    MOLAR_MASS_ZERO_
};

using namespace KUnitConversion;
// Define our plasma Applet
class gasCalculator : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        gasCalculator(QObject *parent, const QVariantList &args);
        ~gasCalculator();

        virtual void init();
        virtual QGraphicsWidget *graphicsWidget();

    public slots:

        /// Calculates the Pressure
        void calculatePressure();

        /// Calculates the Volume
        void calculateVol();

        /// Calculates the Temperature
        void calculateTemp();

        /// Calculates the number of moles
        void calculateMoles();

        /// Calculates the mass of substance
        void calculateMass();

        /// Calculates the molar mass of the substance
        void calculateMolarMass();

        // Functions ( slots ) that occur on changing a value
        /// occurs when the volume is changed
        void volChanged();

        /// occurs when the temperature is changed
        void tempChanged();

        /// occurs when the pressure is changed
        void pressureChanged();

        /// occurs when the mass is changed
        void massChanged();

        /*
        * occurs when the number of moles is changed
        * @param value is the number of moles
        */
        void molesChanged(int value);

        /*
        * occurs when the molar mass is change\
        * @param value is the molar mass
        */
        void molarMassChanged(int value);

        // occurs when Vander Val's constant a is changed
        void Vand_aChanged();

        // occurs when Vander Val's constant b is changed
        void Vand_bChanged();

        // occurs when any quantity is changed
        void calculate();

        /*
        * This function prints error messages on the screen
        * @param mode is the type of error that occurred, refer ERROR_TYPE_GAS
        */
        void error(int);

        /*
        * This function sets the mode of calculation
        * @param mode is the mode of calculation
        */
        void setMode(int mode);

        // This function initialises/ resets the class
        void reset();

        void configChanged();

    protected:
        void createConfigurationInterface(KConfigDialog *parent);

    protected slots:
        void gasConfigAccepted();

    private:
        Converter *m_converter;
        // The following objects are for the user interface
        QGraphicsWidget  *m_widget;

        Plasma::ComboBox *m_calculationMode;
        Plasma::ComboBox *m_massUnit;
        Plasma::ComboBox *m_pressureUnit;
        Plasma::ComboBox *m_temperatureUnit;
        Plasma::ComboBox *m_volumeUnit;
        Plasma::ComboBox *m_aUnit;
        Plasma::ComboBox *m_bUnit;

        Plasma::SpinBox *m_molarMass;
        Plasma::SpinBox *m_moles;
        Plasma::SpinBox *m_mass;
        Plasma::SpinBox *m_pressure;
        Plasma::SpinBox *m_temperature;
        Plasma::SpinBox *m_volume;
        Plasma::SpinBox *m_Vand_a;
        Plasma::SpinBox *m_Vand_b;

        Plasma::Label   *m_error;

        Plasma::PushButton *m_reset;

        double m_Moles;                     // Number of moles
        double m_MolarMass;                 // molarMass
        Value m_Mass;                       // mass
        Value m_Temp;                       // Temperature
        Value m_Pressure;                   // pressure
        Value m_Vol;                        // volume
        Value m_Vand_B;                     // vander val's constant b

        //( Unit conversion library not available for the following quantities)
        double m_Vand_A;                    // Vander val's constant a

        int m_mode;                         // mode of calculation

        /// Designer Config file
        Ui::gasCalculatorConfig ui;
        bool m_configUpdated;
        // Indicates whether gas is assumed to be ideal by default
        bool m_ideal;

};

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(gasCalculator, gasCalculator)

#endif // GASCALCULATOR_H
