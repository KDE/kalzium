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
// Here we avoid loading the header multiple times
#ifndef concentrationCalculator_HEADER
#define concentrationCalculator_HEADER
// We need the Plasma Applet headers
#include <KIcon>
 
#include <Plasma/PopupApplet>
#include <Plasma/Svg>
#include <converter.h>
#include <value.h>

namespace Plasma{
	class QSizeF;
	class ComboBox;
	class SpinBox;
	class TextEdit;
	class Label;
	class Slider;
	class SpinBox;
	class RadioButton;
};

using namespace Conversion;
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

		// Sub-routines involved in calculations of the unit
	    void calculateAmtSolute(void);          // Calculates the amount of solute
	    void calculateAmtSolvent(void);         // Calculates the amount of solvent
	    void calculateMolarMass(void);          // Calculates the molar mass
	    void calculateEqtMass(void);            // Calculates the equivalent mass
	    void calculateMolarMassSolvent(void);   // Calculates the calculate molar mass of the solvent
	    // void calculateDensitySolute(void);       // Calculates the density of solute
	    // void calculateDensitySolvent(void);      // Calculates the density of solvent
	    void calculateConcentration(void);      // calculates the concentration

	    // Functions ( slots ) that occur on changing a value
	    // Sub routines which act as quantity change event handlers

	    void amtSoluteChanged(void);            // occurs when the amount of solute is changed
	    void amtSolventChanged(void);           // occurs when the amount of solvent is changed
	    void molarMassChanged(double);          // occurs when the molar mass of solute is changed
	    void eqtMassChanged(double);            // occurs when the equivalent mass of solute is changed
	    void molarMassSolventChanged(double);   // occurs when the molar mass of solvent is changed
	    void densitySoluteChanged(void);        // occurs when the number of moles is changed
	    void densitySolventChanged(void);       // occurs when the density of solvent is changed
	    void concentrationChanged(double);      // occurs when the concentration is changed
	    void calculate(void);                   // occurs when any quantity is changed

	    double volumeSolvent(void);             // returns volume of solvent in liters
	    double massSolvent(void);               // returns mass of solvent in grams
	    double molesSolvent(void);              // returns number of moles of solvent
	    double densitySolvent(void);            // returns density of solvent in grams per liter
	    double volumeSolute(void);              // returns volume of solute in liters
	    double massSolute(void);                // returns mass of solute in grams
	    double molesSolute(void);               // returns the number of moles of solute
	    double eqtsSolute(void);                // returns the number of equivalents of solute
	    double densitySolute(void);             // returns density of solute in grams per liter

	    void debug(void);
	    void error(int);                 // outputs error messages on the screen
	    private slots:
	    // will be added soon
     
    private:
        QGraphicsWidget  *m_widget;
        Plasma::ComboBox *m_amountSoluteType;
        Plasma::ComboBox *m_amountSoluteUnit1;
        Plasma::ComboBox *m_amountSoluteUnit2;        
        Plasma::ComboBox *m_amountSolventType;
        Plasma::ComboBox *m_amountSolventUnit1;
        Plasma::ComboBox *m_amountSolventUnit2;
        Plasma::ComboBox *m_densitySoluteUnit;
        Plasma::ComboBox *m_densitySolventUnit;
        Plasma::ComboBox *m_concentrationUnit;
        
        Plasma::SpinBox *m_amountSolute;
        Plasma::SpinBox *m_molarMass;
        Plasma::SpinBox *m_eqtMass;
        Plasma::SpinBox *m_densitySolute;
        Plasma::SpinBox *m_amountSolvent;
        Plasma::SpinBox *m_molarMassSolvent;
        Plasma::SpinBox *m_densitySolvent;
        Plasma::SpinBox *m_concentration;   
        
        Plasma::RadioButton *m_r1,*m_r2,*m_r3,*m_r5,*m_r6,*m_r8;
        
        Value m_AmtSolute;                          // amount of solute
	    Value m_AmtSolvent;                         // amount of solvent
	    double m_MolesSolute;                       // amount of solute in moles
	    double m_MolesSolvent;                      // amount of solvent in moles
	    double m_MolarMass;                         // molar mass of solute
	    double m_EqtMass;                           // equivalent mass of solute
	    double m_MolarMassSolvent;                  // molar mass of solvent
	    Value m_DensitySolute;                      // density of solute
	    Value m_DensitySolvent;                     // density of the solvent
	    double m_Concentration;                     // concentration of the solution
      
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(concentrationCalculator, concentrationCalculator)
#endif //concentrationCalculator_HEADER
