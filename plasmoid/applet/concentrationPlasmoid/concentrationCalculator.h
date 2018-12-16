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

#ifndef CONCENTRATIONCALCULATOR_H
#define CONCENTRATIONCALCULATOR_H

#include <KIcon>

#include <Plasma/PopupApplet>
#include <Plasma/Svg>
#include <KUnitConversion/Converter>
#include <KUnitConversion/Value>

#include "ui_concentrationCalculatorConfig.h"
namespace Plasma{
    class ComboBox;
    class SpinBox;
    class TextEdit;
    class Label;
    class Slider;
    class RadioButton;
    class PushButton;
}

enum ERROR_TYPE_CONC {
    RESET_CONC_MESG = 0,
    PERCENTAGE_ZERO,
    DENSITY_ZERO,
    MASS_ZERO,
    VOLUME_ZERO,
    MOLES_ZERO,
    MOLAR_MASS_ZERO,
    EQT_MASS_ZERO,
    MOLAR_SOLVENT_ZERO,
    EQTS_ZERO,
    CONC_ZERO,
    INSUFFICIENT_DATA_EQT,
    INSUFFICIENT_DATA_MOLE,
    INSUFFICIENT_DATA_MOLES,
    INSUFFICIENT_DATA_SOLVENT
};

using namespace KUnitConversion;

// Define our plasma Applet
class concentrationCalculator : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        concentrationCalculator(QObject *parent, const QVariantList &args);
        ~concentrationCalculator();

        virtual void init();
        virtual QGraphicsWidget *graphicsWidget();

    public slots:
        // Sub-routines involved in calculations of the unit
        void calculateAmountSolute();       // Calculates the amount of solute
        void calculateAmountSolvent();      // Calculates the amount of solvent
        void calculateMolarMass();          // Calculates the molar mass
        void calculateEqtMass();            // Calculates the equivalent mass
        void calculateMolarMassSolvent();   // Calculates the calculate molar mass of the solvent
        void calculateConcentration();      // calculates the concentration

        // Functions (slots) that occur on changing a value
        // Sub routines which act as quantity change event handlers

        void amountSoluteChanged();         // occurs when the amount of solute is changed
        void amountSoluteTypeChanged();     // occurs when the mode of amount of solute is changed
        void amountSolventChanged();        // occurs when the amount of solvent is changed
        void amountSolventTypeChanged();    // occurs when the mode of amount of solvent is changed
        void molarMassChanged(int);         // occurs when the molar mass of solute is changed
        void eqtMassChanged(int);           // occurs when the equivalent mass of solute is changed
        void molarMassSolventChanged(int);  // occurs when the molar mass of solvent is changed
        void densitySoluteChanged();        // occurs when the number of moles is changed
        void densitySolventChanged();       // occurs when the density of solvent is changed
        void concentrationChanged(int);     // occurs when the concentration is changed
        void calculate();                   // occurs when any quantity is changed

        double volumeSolvent();             // returns volume of solvent in liters
        double massSolvent();               // returns mass of solvent in grams
        double molesSolvent();              // returns number of moles of solvent
        double densitySolvent();            // returns density of solvent in grams per liter
        double volumeSolute();              // returns volume of solute in liters
        double massSolute();                // returns mass of solute in grams
        double molesSolute();               // returns the number of moles of solute
        double eqtsSolute();                // returns the number of equivalents of solute
        double densitySolute();             // returns density of solute in grams per liter

        /**
         * outputs error messages on the screen
         * @param type is the type of error that occurred (refer ERROR_TYPE_CONC)
         */
        void error(int type);

        /// Performs the initialisation for the class
        void reset();

        /**
         * Sets the mode of calculation eg, volume, amount of substance etc
         * @param mode is the mode of calculation.
         */
        void setMode(int mode);

        void configChanged();

    protected:
        void createConfigurationInterface(KConfigDialog *parent);

    protected slots:
        void configAccepted();

    private:
        Converter *m_converter;
        QGraphicsWidget  *m_widget;
        Plasma::ComboBox *m_amountSoluteType;
        Plasma::ComboBox *m_amountSoluteUnit;
        Plasma::ComboBox *m_amountSolventType;
        Plasma::ComboBox *m_amountSolventUnit;
        Plasma::ComboBox *m_densitySoluteUnit;
        Plasma::ComboBox *m_densitySolventUnit;
        Plasma::ComboBox *m_concentrationUnit;
        Plasma::ComboBox *m_calculationMode;

        Plasma::SpinBox *m_amountSolute;
        Plasma::SpinBox *m_molarMass;
        Plasma::SpinBox *m_eqtMass;
        Plasma::SpinBox *m_densitySolute;
        Plasma::SpinBox *m_amountSolvent;
        Plasma::SpinBox *m_molarMassSolvent;
        Plasma::SpinBox *m_densitySolvent;
        Plasma::SpinBox *m_concentration;
        Plasma::Label   *m_error;

        Plasma::PushButton *m_reset;

        Value m_AmtSolute;                  // amount of solute
        Value m_AmtSolvent;                 // amount of solvent
        double m_MolesSolute;               // amount of solute in moles
        double m_MolesSolvent;              // amount of solvent in moles
        double m_MolarMass;                 // molar mass of solute
        double m_EqtMass;                   // equivalent mass of solute
        double m_MolarMassSolvent;          // molar mass of solvent
        Value m_DensitySolute;              // density of solute
        Value m_DensitySolvent;             // density of the solvent
        double m_Concentration;             // concentration of the solution
        int m_mode;                         // mode of calculation

        /// Designer Config file
        Ui::concentrationCalculatorConfig ui;
        bool m_configUpdated;
        // Indicates solute is always given in terms of mass
        bool m_soluteMass;                    // For configuration
        // Indicates solvent is always given in terms of volume
        bool m_solventVolume;                // For configuration
};

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(concentrationCalculator, concentrationCalculator)

#endif // CONCENTRATIONCALCULATOR_H
