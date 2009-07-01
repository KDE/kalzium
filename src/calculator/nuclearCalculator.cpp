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

#include "nuclearCalculator.h"
#include <math.h>

using namespace Conversion;

nuclearCalculator::nuclearCalculator(QWidget * parent)
        : QFrame(parent)
{
    ui . setupUi(this);

    /**************************************************************************/
    //                       Nuclear Calculator set up                        //
    /**************************************************************************/
    KalziumDataObject *kdo = KalziumDataObject::instance();

    // add all element names to the comboBox in the user interface
    foreach(Element * e, kdo -> ElementList) {
        ui . element -> addItem(e -> dataAsString(ChemicalDataObject::name));
    }
///FIXME
    /* The last three elemenents will be removed because information is not available
       and causes the program to crash when selected. */
    int count = ui . element -> count();
    ui . element -> removeItem(count - 1);
    ui . element -> removeItem(count - 2);
    ui . element -> removeItem(count - 3);
    // Add all isotope names of Uranium ( by default )to the isotope comboBox
    QList<Isotope*> list = KalziumDataObject::instance() -> isotopes(92);
    QString isotope;
    foreach(Isotope * i , list) {
        isotope . setNum(i -> mass());
        ui . isotope  -> addItem(isotope);
    }

    // initialise the data, initially selected values ( Uranium, 92, 238)
    ui . element    -> setCurrentIndex(91);
    ui . isotope    -> setCurrentIndex(18);
    ui . halfLife   -> setValue(list . at(18) -> halflife());
    QString tempStr;
    tempStr . setNum(list . at(18) -> mass());
    ui . mass -> setText(tempStr);

    // Setup of the UI done
    // Initialise values
    m_initAmount  = Value(0.0, "g") ;
    m_finalAmount = Value(0.0, "g");
    m_mass = list . at(18) -> mass();
    m_time = Value(0.0, "y");
    m_halfLife = Value(list . at(18) -> halflife(), "y");

    m_element = * KalziumDataObject::instance() -> element(92);
    m_isotope = * list . at(18);

    // Initialisation of values done
    // Connect signals with slots
    connect(ui . element ,  SIGNAL(activated(int)),
            this, SLOT(elementChanged(int)));
    connect(ui . isotope ,  SIGNAL(activated(int)),
            this, SLOT(isotopeChanged(int)));
    connect(ui . halfLife , SIGNAL(valueChanged(double)),
            this, SLOT(halfLifeChanged()));
    connect(ui . halfLife_unit , SIGNAL(activated(int)),
            this, SLOT(halfLifeChanged()));
    connect(ui . initAmt ,  SIGNAL(valueChanged(double)),
            this, SLOT(initAmtChanged()));
    connect(ui . initAmt_unit ,  SIGNAL(activated(int)),
            this, SLOT(initAmtChanged()));
    connect(ui . initAmtType ,  SIGNAL(activated(int)),
            this, SLOT(initAmtChanged()));
    connect(ui . finalAmt  ,  SIGNAL(valueChanged(double)),
            this, SLOT(finalAmtChanged()));
    connect(ui . finalAmt_unit  ,  SIGNAL(activated(int)),
            this, SLOT(finalAmtChanged()));
    connect(ui . finalAmtType ,  SIGNAL(activated(int)),
            this, SLOT(finalAmtChanged()));
    connect(ui . time  ,  SIGNAL(valueChanged(double)),
            this, SLOT(timeChanged()));
    connect(ui . time_unit  ,  SIGNAL(activated(int)),
            this, SLOT(timeChanged()));
    connect(ui . slider  , SIGNAL(valueChanged(int)),
            this, SLOT(sliderMoved(int)));

    /**************************************************************************/
    // Nuclear Calculator setup complete
    /**************************************************************************/
}

nuclearCalculator:: ~nuclearCalculator()
{

}

// This function is executed when the element is changed
void nuclearCalculator::elementChanged(int index)
{
    // set the newly chosen element
    m_element = * KalziumDataObject::instance() -> element(index + 1);

    // Add all isotope names of Uranium ( by default ) to the isotope comboBox
    QList<Isotope*> list = KalziumDataObject::instance() -> isotopes(index + 1);
    QString isotope;                        // A temporary string
    ui . isotope  -> clear();               // Clear the contents of the combo box

    // update the combobox with isotopes of the new element
    foreach(Isotope * i , list) {
        isotope . setNum(i -> mass());
        ui . isotope  -> addItem(isotope);
    }

    // Set the halfLife to that of the first isotope of the element.
    ui . halfLife  -> setValue(list. at(0) -> halflife());
    // Recalculate and update
    calculate();
}

// This function is executed when the isotope is changed
void  nuclearCalculator::isotopeChanged(int index)
{
    // update the nuclear Calculator
    int elementNumber = ui.element  -> currentIndex() + 1;
    QList<Isotope*> list = KalziumDataObject::instance() -> isotopes(elementNumber);
    m_isotope = * list . at(index);

    // get the halfLife of the new isotope
    double halfLife = list . at(index) -> halflife();
    m_mass = list . at(index) -> mass();

    // A string used for searching the right unit
    QString halfLifeUnit = list . at(index) -> halflifeUnit();
    halfLifeUnit = (halfLifeUnit == "y") ? "year" : "seconds";

    QString tempStr;                // A temporary string
    tempStr . setNum(m_mass);
    ui . mass -> setText(tempStr);
    // Update the UI with the halfLife value
    ui . halfLife -> setValue(halfLife);
    int x = ui . halfLife_unit -> findText(halfLifeUnit);
    if (x >= 0)
        ui . halfLife_unit -> setCurrentIndex(x);
    m_halfLife = Value(halfLife , halfLifeUnit);
    // Recalculate and update
    calculate();
}

// This function is executed when the halfLife is changed
void  nuclearCalculator::halfLifeChanged()
{
    // update the halfLife value
    m_halfLife = Value(ui . halfLife -> value(), ui . halfLife_unit -> currentText());
    // recalculate the required
    calculate();
}

void  nuclearCalculator::initAmtChanged()
{

    // If quantity is specified in terms of mass, quantity <- ( mass , unit)
    if (ui . initAmtType -> currentIndex() == 0)
        m_initAmount = Value(ui . initAmt -> value(), ui . initAmt_unit -> currentText());

    // If quantity is specified in terms of moles quantity <- ( moles * atomicMass, unit )
    else
        m_initAmount = Value(((ui . initAmt -> value()) * m_mass), \
                             ui . initAmt_unit -> currentText());

    calculate();
}

void  nuclearCalculator::finalAmtChanged()
{
    // If quantity is specified in terms of mass, quantity <- ( mass , unit)
    if (ui . finalAmtType -> currentIndex() == 0)
        m_finalAmount = Value(ui . finalAmt -> value(), \
                              ui . finalAmt_unit -> currentText());
    // If quantity is specified in terms of moles quantity <- ( moles * atomicMass, unit )
    else
        m_finalAmount = Value(((ui . finalAmt -> value()) * m_mass), \
                              ui . finalAmt_unit -> currentText());

    calculate();
}

void  nuclearCalculator::sliderMoved(int numHlives)
{
    double num = numHlives / 10.0;
    m_time = Value(num * m_halfLife . number() , m_halfLife . unit());

    ui . time -> setValue(m_time . number());
    ui . time_unit -> setCurrentIndex(ui . halfLife_unit -> currentIndex());
    ui . numHalfLives -> setText(m_time . toString());
}

void  nuclearCalculator::timeChanged()
{
    m_time = Value(ui . time -> value(), ui . time_unit -> currentText());

    calculate();

}

void  nuclearCalculator::calculate()
{

    // Validate the values involved in calculation
    if (m_halfLife . number() == 0.0)
        return;

    if (ui . r1  -> isChecked()) {   // Calculate initial amount
        if (m_finalAmount . number() == 0.0)
            return;
        calculateInitAmount();
        ui . initAmt  -> setValue(m_initAmount . number());
    }
    // Calulate final Amount after time
    else if (ui . r2  -> isChecked()) {
        if (m_initAmount . number() == 0.0)
            return;
        calculateFinalAmount();
        ui . finalAmt -> setValue(m_finalAmount . number());
    } else { // Calculate Time
        // final amount greater than initial
        if (ui . finalAmt  -> value() > ui . initAmt  -> value())
            return;
        // one of the amounts is 0.0
        if (ui . finalAmt  -> value() == 0.0)
            return;
        calculateTime();
        ui . time  -> setValue(m_time . number());
    }
}

void nuclearCalculator::calculateInitAmount(void)
{
    // If no time has elapsed, initial and final amounts are the same
    m_initAmount = m_finalAmount;
    if (m_time . number() == 0.0) {
        return;
    }
    // Calculate the number of halfLives that have elapsed
    double ratio = (Converter::self()->convert(m_time , m_halfLife . unit() \
                    -> singular()) . number()) / m_halfLife . number();
    // find out the initial amount
    m_initAmount = Value(m_initAmount . number() * pow(2.0 , ratio), m_initAmount . unit());
    // Convert into the required units
    m_initAmount = Converter::self()->convert(m_initAmount, m_initAmount . unit() -> singular());
}

void nuclearCalculator::calculateFinalAmount(void)
{
    // If no time has elapsed, initial and final amounts are the same
    m_finalAmount = m_initAmount;
    if (m_time . number() == 0.0) {

        return;
    }
    // Calculate the number of halfLives that have elapsed
    double ratio = (Converter::self()->convert(m_time , m_halfLife . unit() \
                    -> singular()) . number()) / m_halfLife . number();
    // Calculate the final amount
    m_finalAmount = Value(m_finalAmount . number() / pow(2.0, ratio) , m_initAmount . unit());
    // Convert into the required units
    m_finalAmount = Converter::self()->convert(m_finalAmount, m_finalAmount . unit() -> singular());
}

void nuclearCalculator::calculateTime(void)
{
    // If initial and final masses are the same ( both units and value )
    // the time is also 0
    if (m_initAmount . number() == m_finalAmount . number() && \
            m_initAmount . unit() == m_finalAmount . unit()) {
        m_time = Value(0.0, m_time . unit());
        return;
    }

    // calculate the ratio of final to initial masses
    double ratio = (Converter::self()->convert(m_initAmount , m_finalAmount . unit() \
                    -> singular()) . number()) / m_finalAmount . number();
    // The number of halfLives ( log 2 ( x ) = log x / log 2 )
    double numHalfLives = log(ratio) / log(2.0);
    double time_value = numHalfLives  * m_halfLife . number();
    // Calculate the total time taken
    Value temp = Value(time_value, m_halfLife . unit());
    m_time = Converter::self()->convert(temp , m_time . unit() -> singular());
    return;
}

#include "nuclearCalculator.moc"


