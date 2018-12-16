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

#ifndef NUCLEARCALCULATOR_H
#define NUCLEARCALCULATOR_H

#include <KIcon>

#include <Plasma/PopupApplet>
#include <Plasma/Svg>
#include <element.h>
#include <isotope.h>
#include <KUnitConversion/Converter>
#include <KUnitConversion/Value>

#include "kalziumdataobject.h"

#include "ui_nuclearCalculatorConfig.h"

namespace Plasma{
    class ComboBox;
    class SpinBox;
    class TextEdit;
    class Label;
    class Slider;
    class RadioButton;
    class PushButton;
}

// This is the enumeration for the error type required in the error(int mode) function
enum ERROR_MODE_NUKE {
    RESET_NUKE_MESG = 0,
    INIT_AMT_ZERO,
    FINAL_AMT_ZERO,
    HALFLIFE_ZERO,
    FINAL_AMT_GREATER
};

using namespace KUnitConversion;
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

    public slots:
        /// Calculates the initial amount
        void calculateInitAmount();

        /// Calculates the final amount
        void calculateFinalAmount();

        /// Calculates the time required
        void calculateTime();

        /// occurs when the element is changed
        void elementChanged(int index);

        /// occurs when the isotope is changed
        void isotopeChanged(int index);

        /// occurs when the halfLife is changed
        void halfLifeChanged();

        /// occurs when any quantity is changed
        void calculate();

        /// occurs when the initial amount is changed in the UI
        void initAmtChanged();

        /// occurs when the final amount is changed in the UI
        void finalAmtChanged();

        /// occurs when the time is changed in the UI
        void timeChanged();

#if 0
        /*
            * occurs when the slider in the ui is moved
            * @param x is 10 times the number of halfLives indicated in the slider
            */
        void sliderMoved(int x);
#endif

        /*
            * Used to display error messages
            * @param mode is the error mode, refer ERROR_MODE_NUKE.
            */
        void error(int mode);

        /*
        * Sets the mode of calculation eg, time, initial amount etc
        * @param mode is the mode of calculation.
        */
        void setMode(int mode);

        /// This function is called to reset / initialise the calculator
        void reset();

        /// This function hides the slider just under the time spinBox in the UI
        void showSlider(bool show);

        void configChanged();

    private slots:
    // will be added soon

    protected:
        void createConfigurationInterface(KConfigDialog *parent);
        void configAccepted();
    private:
    Converter *m_converter;
        QGraphicsWidget  *m_widget;
        Plasma::ComboBox *m_element;
        Plasma::ComboBox *m_isotope;
        Plasma::ComboBox *m_halfLifeUnit;
        Plasma::ComboBox *m_initType;
        Plasma::ComboBox *m_finalType;
        Plasma::ComboBox *m_initUnit;
        Plasma::ComboBox *m_finalUnit;
        Plasma::ComboBox *m_timeUnit;
        Plasma::ComboBox *m_calculationMode;

        Plasma::SpinBox *m_halfLife;
        Plasma::SpinBox *m_initAmt;
        Plasma::SpinBox *m_finalAmt;
        Plasma::SpinBox *m_time;

//      Plasma::Slider *m_slider;
        Plasma::PushButton *m_reset;
        Plasma::Label *m_numHalfLives;
        Plasma::Label *m_error;
//        Plasma::Label *m_sliderLabel;
        Element m_Element;                      // Current element
        Isotope m_Isotope;                      // current isotope
        Value m_HalfLife;                       // The halfLife
        Value m_InitAmount;                     // initial amount present
        Value m_FinalAmount;                    // amount after time
        Value m_Time;                           // the time involved in calculation
        double m_Mass;                          // the atomic mass of the isotope
        int m_mode;

        /// Designer Config file
        Ui::nuclearCalculatorConfig ui;
        bool m_configUpdated;
        bool m_massOnly;
};

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(nuclearCalculator, nuclearCalculator)

#endif // NUCLEARCALCULATOR_H
