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
#ifndef gasCalculator_HEADER
#define gasCalculator_HEADER
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
class gasCalculator : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        gasCalculator(QObject *parent, const QVariantList &args);
        ~gasCalculator();
        
        virtual void init();
        virtual QGraphicsWidget *graphicsWidget();
 
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
    private slots:
    // will be added soon
     
    private:
        QGraphicsWidget  *m_widget;
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
        
        Plasma::RadioButton *m_r2,*m_r4,*m_r5,*m_r6;
      
		double m_Moles;                     // Number of moles
	    double m_MolarMass;                 // molarMass
	    Value m_Mass;                       // mass
	    Value m_Temp;                       // Temperature
	    Value m_Pressure;                   // pressure
	    Value m_Vol;                        // volume
	    Value m_Vand_B;                     // vander val's constant b

	    //( Unit conversion library not available for the following quantities)
	    double m_Vand_A;                    // Vander val's constant a      
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(gasCalculator, gasCalculator)
#endif //gasCalculator_HEADER
