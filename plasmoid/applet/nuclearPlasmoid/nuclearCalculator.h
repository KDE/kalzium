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
#ifndef nuclearCalculator_HEADER
#define nuclearCalculator_HEADER
// We need the Plasma Applet headers
#include <KIcon>
 
#include <Plasma/PopupApplet>
#include <Plasma/Svg>
#include <element.h>
#include <isotope.h>
#include <converter.h>
#include <value.h>
#include <kalziumdataobject.h>

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

// Define our plasma Applet
class nuclearCalculator : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        nuclearCalculator(QObject *parent, const QVariantList &args);
        ~nuclearCalculator();
        
        virtual void init();
        virtual QGraphicsWidget *graphicsWidget();
        
        /// Calculates the initial amount
		void calculateInitAmount(void);

		/// Calculates the final amount
		void calculateFinalAmount(void);

		/// Calculates the time required
		void calculateTime(void);

		void elementChanged(int index);

		/// occurs when the isotope is changed
		void isotopeChanged(int index);

		/// occurs when the halfLife is changed
		void halfLifeChanged(void);

		/// occurs when any quantity is changed
		void calculate(void);

		/// occurs when the initial amount is changed in the UI
		void initAmtChanged(void);

		/// occurs when the final amount is changed in the UI
		void finalAmtChanged(void);

		/// occurs when the time is changed in the UI
		void timeChanged(void);

		/// occurs when the slider in the ui is moved
		void sliderMoved(int);
 
    private slots:
    // will be added soon
     
    private:
        QGraphicsWidget  *m_widget;
        Plasma::ComboBox *m_element;
        Plasma::ComboBox *m_isotope;
        Plasma::ComboBox *m_halfLifeUnit;
        Plasma::ComboBox *m_initType;
        Plasma::ComboBox *m_finalType;
        Plasma::ComboBox *m_initUnit;
        Plasma::ComboBox *m_finalUnit;
        Plasma::ComboBox *m_timeUnit;
        
        Plasma::SpinBox *m_halfLife;
        Plasma::SpinBox *m_initAmt;
        Plasma::SpinBox *m_finalAmt;
        Plasma::SpinBox *m_time;

		Plasma::Slider *m_slider;
        
        Plasma::Label *m_timeInHalfLives;      
        
        Plasma::RadioButton *m_r1;
        Plasma::RadioButton *m_r2;
        Plasma::RadioButton *m_r3;
        
        Element m_Element;                      // Current element
	    Isotope m_Isotope;                      // current isotope
	    Value m_HalfLife;                       // The halfLife
	    Value m_InitAmount;                     // initial amount present
	    Value m_FinalAmount;                    // amount after time
	    Value m_Time;                           // the time involved in calculation
	    double m_Mass;                          // the atomic mass of the isotope        
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(nuclearCalculator, nuclearCalculator)
#endif //nuclearCalculator_HEADER
