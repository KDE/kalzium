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

#include "nuclearCalculator.h"

#include <QApplication>
#include <QFontMetrics>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QSizeF>
#include <QSlider>
#include <QSpinBox>

#include <Plasma/ComboBox>
#include <Plasma/Frame>
#include <Plasma/GroupBox>
#include <Plasma/Label>
#include <Plasma/LineEdit>
#include <Plasma/PushButton>
#include <Plasma/RadioButton>
#include <Plasma/Slider>
#include <Plasma/SpinBox>
#include <Plasma/Svg>
#include <Plasma/Theme>

#include <KComboBox>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KIntSpinBox>
#include <KTextEdit>

#include <math.h>


using namespace KUnitConversion;

nuclearCalculator::nuclearCalculator(QObject *parent, const QVariantList &args)
: Plasma::PopupApplet(parent, args)
, m_widget(0)
{
    m_converter = new Converter(this);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPopupIcon("accessories-calculator");
    setHasConfigurationInterface(true);
    setAssociatedApplication("kalzium");
    resize(600, 450);
}

nuclearCalculator::~nuclearCalculator()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }
}

void nuclearCalculator::init()
{
    configChanged();
}

void nuclearCalculator::configChanged()
{
    KConfigGroup cg = config();

    m_massOnly = cg.readEntry("massOnly",true);
}

void nuclearCalculator::reset()
{
    const int ISOTOPE_NUM = 22;
    error(RESET_NUKE_MESG);

    // Add all isotope names of Uranium (by default) to the isotope comboBox
    QList<Isotope*> list = KalziumDataObject::instance()->isotopes(92);
    QString iso;

    m_isotope->clear();
    foreach (Isotope *i, list) {
        iso.setNum(i->mass());
        m_isotope->addItem(iso);
    }

    // initialize the data, initially selected values (Uranium, 92, 238)
    m_element->nativeWidget()->setCurrentIndex(91);
    m_isotope->nativeWidget()->setCurrentIndex(ISOTOPE_NUM);
    m_halfLife->setValue(list.at(ISOTOPE_NUM)->halflife());
    m_initAmt->setValue(6.0);
    m_finalAmt->setValue(3.0);
    m_time->setValue(list.at(ISOTOPE_NUM)->halflife());

    m_halfLifeUnit->nativeWidget()->setCurrentIndex(0);
    m_initType->nativeWidget()->setCurrentIndex(0);
    m_finalType->nativeWidget()->setCurrentIndex(0);
    m_initUnit->nativeWidget()->setCurrentIndex(0);
    m_finalUnit->nativeWidget()->setCurrentIndex(0);
    m_timeUnit->nativeWidget()->setCurrentIndex(0);
    m_calculationMode  ->nativeWidget()->setCurrentIndex(2);

    // Setup of the UI done
    // Initialize values
    m_InitAmount  = Value(6.0, "g") ;
    m_FinalAmount = Value(3.0, "g");
    m_Mass = list.at(ISOTOPE_NUM)->mass();
    m_Time = Value((list.at(ISOTOPE_NUM)->halflife()), "y");
    m_HalfLife = Value(list.at(ISOTOPE_NUM)->halflife(), "y");

    m_Element = * KalziumDataObject::instance()->element(92);
    m_Isotope = * list.at(ISOTOPE_NUM);

    setMode(2);
    // Initialization of values done
}

QGraphicsWidget *nuclearCalculator::graphicsWidget()
{
//FIXME:
// Currently the spin boxes are integer, please convert them into double after
// doubleSpinBoxes are available

    if (!m_widget) {
        // Position all UI elements
        m_widget = new QGraphicsWidget(this);
        m_widget->setMinimumSize(550, 400);
        Plasma::Frame *pHeader = new Plasma::Frame(this);
        pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        pHeader->setText(i18n("Nuclear Calculator"));

        Plasma::GroupBox *pGroupBox1 = new Plasma::GroupBox(this);
        Plasma::GroupBox *pGroupBox2 = new Plasma::GroupBox(this);

        QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(pGroupBox1);
        QGraphicsGridLayout *pGridLayout2 = new QGraphicsGridLayout(pGroupBox2);
        QGraphicsLinearLayout *pVLayout = new QGraphicsLinearLayout(Qt::Vertical, m_widget);
        pVLayout->addItem(pGroupBox1);
        pVLayout->addItem(pGroupBox2);
        // pVLayout->addItem(new Plasma::)

        // here comes the element - isotope and halfLife info part
        Plasma::Label *eleLabel = new Plasma::Label(this);
        eleLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        eleLabel->setText(i18n("Element name:"));
        Plasma::Label *isoLabel = new Plasma::Label(this);
        isoLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        isoLabel->setText(i18n("Isotope mass:"));
        Plasma::Label *hLifeLabel = new Plasma::Label(this);
        hLifeLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        hLifeLabel->setText(i18n("Half-Life:"));

        m_element = new Plasma::ComboBox(this);
        m_element->setZValue(2);
        m_isotope = new Plasma::ComboBox(this);
        m_isotope->setZValue(1);

        m_halfLife = new Plasma::SpinBox(this);
        m_halfLife->nativeWidget()->setMinimumWidth(100);
        m_halfLife->nativeWidget()->setMaximum(1000000000);
        //m_halfLife->setDecimals(4);
        m_halfLife->setMaximum(1e+09);

        m_halfLifeUnit = new Plasma::ComboBox(this);
        m_halfLifeUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("year")
         << i18n("seconds")
         << i18n("minutes")
         << i18n("hours")
         << i18n("days")
         << i18n("weeks"));
        m_halfLifeUnit->setZValue(6);

        pGridLayout->addItem(pHeader, 0, 0, 1, 4);
        pGridLayout->addItem(eleLabel, 1, 0);
        pGridLayout->addItem(m_element, 1, 1);
        pGridLayout->addItem(isoLabel, 2, 0);
        pGridLayout->addItem(m_isotope, 2, 1);
        pGridLayout->addItem(hLifeLabel, 3, 0);
        pGridLayout->addItem(m_halfLifeUnit, 3, 2);
        pGridLayout->addItem(m_halfLife, 3, 1);

        // Here comes the amount and time part

        // Calculation mode
        Plasma::Label *calcModeLabel = new Plasma::Label(this);
        calcModeLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        calcModeLabel->setText(i18n("Calculation Mode:"));

        m_calculationMode = new Plasma::ComboBox(this);
        m_calculationMode->setZValue(3);
        m_calculationMode->nativeWidget()->insertItems(0, QStringList()
            << i18n("Initial amount")
            << i18n("Final amount")
            << i18n("Time")
        );

        Plasma::Label *initLabel = new Plasma::Label(this);
        initLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        initLabel->setText(i18n("Initial amount:"));
        Plasma::Label *finalLabel = new Plasma::Label(this);
        finalLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        finalLabel->setText(i18n("Final amount:"));
        Plasma::Label *timeLabel = new Plasma::Label(this);
        timeLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        timeLabel->setText(i18n("Time:"));
/*x        Plasma::Label *m_sliderLabel = new Plasma::Label(this);
        m_sliderLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_sliderLabel->setText(i18n("Time in Half-Lives"));*/

        m_numHalfLives = new Plasma::Label(this);
        m_numHalfLives->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_numHalfLives->setText(i18n("0 seconds"));
        m_initAmt = new Plasma::SpinBox(this);
        m_initAmt->nativeWidget()->setMinimumWidth(200);
        m_initAmt->nativeWidget()->setMaximum(1000000000);
        //m_initAmt->setDecimals(4);
        m_initAmt->setMaximum(1e+09);
        m_finalAmt = new Plasma::SpinBox(this);
        m_finalAmt->nativeWidget()->setMinimumWidth(200);
        m_finalAmt->nativeWidget()->setMaximum(1000000000);
        //m_finalAmt->setDecimals(4);
        m_finalAmt->setMaximum(1e+09);
        m_time = new Plasma::SpinBox(this);
        m_time->nativeWidget()->setMinimumWidth(200);
        m_time->nativeWidget()->setMaximum(1000000000);
        //m_time->setDecimals(4);
        m_time->setMaximum(1e+09);

        m_initUnit = new Plasma::ComboBox(this);
        m_initUnit->setZValue(2);
        m_initUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("grams")
         << i18n("tons")
         << i18n("carats")
         << i18n("pounds")
         << i18n("ounces")
         << i18n("troy ounces"));
        m_initUnit->setZValue(3);

        m_finalUnit = new Plasma::ComboBox(this);
        m_finalUnit->setZValue(2);
        m_finalUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("grams")
         << i18n("tons")
         << i18n("carats")
         << i18n("pounds")
         << i18n("ounces")
         << i18n("troy ounces"));
        m_finalUnit->setZValue(2);

        m_timeUnit = new Plasma::ComboBox(this);
        m_timeUnit->setZValue(2);
        m_timeUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("year")
         << i18n("seconds")
         << i18n("minutes")
         << i18n("hours")
         << i18n("days")
         << i18n("weeks"));
        m_timeUnit->setZValue(1);

        m_initType = new Plasma::ComboBox(this);
        m_initType->setZValue(2);
        m_initType->nativeWidget()->insertItems(0, QStringList()
         << i18n("Mass")
         << i18n("moles"));
        m_initType->setZValue(2);

        m_finalType = new Plasma::ComboBox(this);
        m_finalType->setZValue(2);
        m_finalType->nativeWidget()->insertItems(0, QStringList()
         << i18n("Mass")
         << i18n("moles"));
        m_finalType->setZValue(1);

        /*m_slider = new Plasma::Slider(this);
        m_slider->setRange(0, 100);
        m_slider->setOrientation(Qt::Horizontal);
        m_slider->setMaximum(100); */

        m_error = new Plasma::Label(this);

        m_reset = new Plasma::PushButton(this);
        m_reset->setText(i18n("Reset"));

        pGridLayout2->addItem(calcModeLabel, 5, 0);
        pGridLayout2->addItem(initLabel, 6, 0);
        pGridLayout2->addItem(finalLabel, 7, 0);
        pGridLayout2->addItem(timeLabel, 8, 0);
//         pGridLayout2->addItem(m_sliderLabel, 9, 0);
        pGridLayout2->addItem(m_error, 10, 1, 1, 3);
        pGridLayout2->addItem(m_reset, 10, 0);

        pGridLayout2->addItem(m_calculationMode, 5, 1);
        pGridLayout2->addItem(m_initAmt, 6, 1);
        pGridLayout2->addItem(m_finalAmt, 7, 1);
        pGridLayout2->addItem(m_time, 8, 1);
//         pGridLayout2->addItem(m_slider, 9, 1);

        pGridLayout2->addItem(m_initType, 6, 3);
        pGridLayout2->addItem(m_finalType, 7, 3);
        pGridLayout2->addItem(m_numHalfLives, 9, 2);

        pGridLayout2->addItem(m_initUnit, 6, 2);
        pGridLayout2->addItem(m_finalUnit, 7, 2);
        pGridLayout2->addItem(m_timeUnit, 8, 2);

        // Positioning of UI elements done
        // Now add required properties to the UI widgets

        /**************************************************************************/
        //                       Nuclear Calculator set up                        //
        /**************************************************************************/
        KalziumDataObject *kdo = KalziumDataObject::instance();

        // add all element names to the comboBox in the user interface
        foreach (Element *e, kdo->ElementList) {
            m_element->nativeWidget()->addItem(e->dataAsString(ChemicalDataObject::name));
        }
            ///FIXME
        /* The last three elements will be removed because information is not available
            and causes the program to crash when selected. */
        int count = m_element->nativeWidget()->count();
        m_element->nativeWidget()->removeItem(count - 1);
        m_element->nativeWidget()->removeItem(count - 2);
        m_element->nativeWidget()->removeItem(count - 3);
        // Add all isotope names of Uranium (by default) to the isotope comboBox
        reset();

        // Connect signals with slots
        connect(m_element->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(elementChanged(int)));
        connect(m_isotope->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(isotopeChanged(int)));
//FIXME change int to double in the following signals after finding doubleSpinBox
        connect(m_halfLife, SIGNAL(valueChanged(int)),
                    this, SLOT(halfLifeChanged()));
        connect(m_halfLifeUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(halfLifeChanged()));
        connect(m_initAmt, SIGNAL(valueChanged(int)),
                this, SLOT(initAmtChanged()));
        connect(m_initUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(initAmtChanged()));
        connect(m_initType->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(initAmtChanged()));
        connect(m_finalAmt, SIGNAL(valueChanged(int)),
                this, SLOT(finalAmtChanged()));
        connect(m_finalUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(finalAmtChanged()));
        connect(m_finalType->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(finalAmtChanged()));
        connect(m_time, SIGNAL(valueChanged(int)),
                this, SLOT(timeChanged()));
        connect(m_timeUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(timeChanged()));
/*        connect(m_slider, SIGNAL(valueChanged(int)),
                this, SLOT(sliderMoved(int)));*/
        connect(m_calculationMode->nativeWidget(), SIGNAL(activated(int)),
                        this, SLOT(setMode(int)));
        connect(m_reset, SIGNAL(clicked()),
                        this, SLOT(reset()));
        /**************************************************************************/
        // Nuclear Calculator setup complete
        /**************************************************************************/
    }
    return m_widget;
}

void nuclearCalculator::elementChanged (int index)
{
    // set the newly chosen element
    m_Element = * KalziumDataObject::instance()->element(index + 1);

    // Add all isotope names of Uranium (by default) to the isotope comboBox
    QList<Isotope*> list = KalziumDataObject::instance()->isotopes(index + 1);
    QString isotope;                  // A temporary string
    m_isotope->clear();               // Clear the contents of the combo box
    // update the combobox with isotopes of the new element
    foreach (Isotope *i, list) {
        isotope.setNum(i->mass());
        m_isotope->addItem(isotope);
    }

    // Set the halfLife to that of the first isotope of the element.
    m_halfLife->setValue(list. at(0)->halflife());
    // Recalculate and update
    calculate();
}

void nuclearCalculator::isotopeChanged(int index)
{
    // update the nuclear Calculator
    int elementNumber = m_element->nativeWidget()->currentIndex() + 1;
    QList<Isotope*> list = KalziumDataObject::instance()->isotopes(elementNumber);
    m_Isotope = * list.at(index);

    // get the halfLife of the new isotope
    double halfLife = list.at(index)->halflife();
    m_Mass = list.at(index)->mass();

    // A string used for searching the right Unit
    QString halfLifeUnit = list . at(index)->halflifeUnit();
    halfLifeUnit = (halfLifeUnit == "y") ? "year" : "seconds";

    // Update the UI with the halfLife value
    m_halfLife->setValue(halfLife);
    int x = m_halfLifeUnit->nativeWidget()->findText(halfLifeUnit);
    if (x >= 0) {
        m_halfLifeUnit->nativeWidget()->setCurrentIndex(x);
    }
    m_HalfLife = Value(halfLife, halfLifeUnit);
    // Recalculate and update
    calculate();
}

void nuclearCalculator::halfLifeChanged()
{
    // update the halfLife value
    m_HalfLife = Value(m_halfLife->value(), m_halfLifeUnit->nativeWidget()->currentText());
    // recalculate the required
    calculate();
}

void nuclearCalculator::initAmtChanged()
{

    // If quantity is specified in terms of mass, quantity <- (mass , Unit)
    if (m_initType->nativeWidget()->currentIndex() == 0) {
        m_InitAmount = Value(m_initAmt->value(), m_initUnit->nativeWidget()->currentText());
    } else { // If quantity is specified in terms of moles quantity <- (moles * atomicMass, Unit)
        m_InitAmount = Value(((m_initAmt->value()) * m_Mass), \
                             m_initUnit->nativeWidget()->currentText());
    }

    calculate();
}

void nuclearCalculator::finalAmtChanged()
{
    // If quantity is specified in terms of mass, quantity <- (mass , Unit)
    if (m_finalType->nativeWidget()->currentIndex() == 0) {
        m_FinalAmount = Value(m_finalAmt->value(), \
                              m_finalUnit->nativeWidget()->currentText());
    // If quantity is specified in terms of moles quantity <- (moles * atomicMass, Unit)
    } else {
        m_FinalAmount = Value(((m_finalAmt->value()) * m_Mass), \
                              m_finalUnit->nativeWidget()->currentText());
    }

    calculate();
}

void nuclearCalculator::timeChanged()
{
    m_Time = Value(m_time->value(), m_timeUnit->nativeWidget()->currentText());

    calculate();
}

/*void nuclearCalculator::sliderMoved(int numHlives)
{
    double num = numHlives / 10.0;
    m_Time = Value(num * m_HalfLife. number() , m_HalfLife. unit());

    m_time->setValue(m_Time. number());
    m_timeUnit->nativeWidget()->setCurrentIndex(m_halfLifeUnit->nativeWidget()->currentIndex());
    m_numHalfLives->setText(m_Time.toString());
}*/

void nuclearCalculator::calculate()
{
    error(RESET_NUKE_MESG);
    // Validate the values involved in calculation
    if (m_HalfLife. number() == 0.0) {
        error(HALFLIFE_ZERO);
        return;
    }

    switch (m_mode) {
    case 0: // Calculate initial amount
        if (m_FinalAmount.number() == 0.0) {
            error(FINAL_AMT_ZERO);
            return;
        }
        calculateInitAmount();
        break;
    case 1: // Calculate final Amount after time
        if (m_InitAmount.number() == 0.0) {
            error(INIT_AMT_ZERO);
            return;
        }
        calculateFinalAmount();
        break;
    case 2: // Calculate Time
        // If final amount greater than initial, error
    if (m_FinalAmount.number() > (m_converter->convert(m_InitAmount,
        m_FinalAmount.unit()->symbol())).number()) {
            error(FINAL_AMT_GREATER);
            return;
        } else if (m_finalAmt->value() == 0.0) { // final amount is 0.0
            error(FINAL_AMT_ZERO);
            return;
        }
        calculateTime();
        break;
    }
}

void nuclearCalculator::setMode(int mode)
{
    m_mode = mode;

    m_initAmt->nativeWidget()->setReadOnly(false);
    m_finalAmt->nativeWidget()->setReadOnly(false);
    m_time->nativeWidget()->setReadOnly(false);

    // set the quantity that should be calculated to readOnly
    switch (mode) {
    case 0:
        m_initAmt->nativeWidget()->setReadOnly(true);
//        showSlider(true);
        break;
    case 1:
        m_finalAmt->nativeWidget()->setReadOnly(true);
//        showSlider(true);
        break;
    case 2:
        m_time->nativeWidget()->setReadOnly(true);
//        showSlider(false);
        break;
    }

    calculate();
}

void nuclearCalculator::showSlider(bool /*show*/)
{
// TODO
/*    if (show) {
        m_sliderLabel->hide();
        m_slider->hide();
        m_numHalfLives->hide();
    } else {
        m_sliderLabel->show();
        m_slider->show();
        m_numHalfLives->show();
    }
*/
}

void nuclearCalculator::calculateInitAmount()
{
    // If no time has elapsed, initial and final amounts are the same
    m_InitAmount = m_FinalAmount;
    if (m_Time. number() == 0.0) {
        m_initAmt->setValue(m_InitAmount . number());
        return;
    }
    // Calculate the number of halfLives that have elapsed
    double ratio = (m_converter->convert(m_Time, m_HalfLife. unit() \
                    ->symbol()). number()) /m_HalfLife. number();
    // find out the initial amount
    m_InitAmount = Value(m_InitAmount. number() * pow(2.0 , ratio), m_InitAmount. unit());
    // Convert into the required units
    m_InitAmount = m_converter->convert(m_InitAmount, m_InitAmount. unit()->symbol());
    m_initAmt->setValue(m_InitAmount . number());
}

void nuclearCalculator::calculateFinalAmount()
{
    // If no time has elapsed, initial and final amounts are the same
    m_FinalAmount = m_InitAmount;
    if (m_Time. number() == 0.0) {
        m_finalAmt->setValue(m_FinalAmount.number());
        return;
    }
    // Calculate the number of halfLives that have elapsed
    double ratio = (m_converter->convert(m_Time , m_HalfLife. unit() \
                    ->symbol()). number()) / m_HalfLife. number();
    // Calculate the final amount
    m_FinalAmount = Value(m_FinalAmount . number() / pow(2.0, ratio), m_InitAmount. unit());
    // Convert into the required units
    m_FinalAmount = m_converter->convert(m_FinalAmount, m_FinalAmount. unit()->symbol());
    m_finalAmt->setValue(m_FinalAmount. number());
}

void nuclearCalculator::calculateTime()
{
    // If initial and final masses are the same (both units and value)
    // the time is also 0
    if (m_InitAmount.number() == m_FinalAmount.number() && \
            m_InitAmount. unit() == m_FinalAmount . unit()) {
        m_Time = Value(0.0, m_Time. unit());
        m_time->setValue(m_Time. number());
        return;
    }

    // calculate the ratio of final to initial masses
    double ratio = (m_converter->convert(m_InitAmount , m_FinalAmount.unit()\
                    ->symbol())).number() / m_FinalAmount.number();
    // The number of halfLives (log 2 (x) = log x / log 2)
    double numHalfLives = log(ratio) / log(2.0);
    double time_value = numHalfLives  * m_HalfLife . number();
    // Calculate the total time taken
    Value temp = Value(time_value, m_HalfLife. unit());
    m_Time = m_converter->convert(temp , m_Time.unit()->symbol());
    m_time->setValue(m_Time. number());
}

void nuclearCalculator::error(int mode)
{
    switch (mode) { // Depending on the mode, set the error messages.
    case RESET_NUKE_MESG:
        m_error->setText("");
        break;
    case INIT_AMT_ZERO:
        m_error->setText(i18n("Initial amount cannot be zero."));
        break;
    case FINAL_AMT_ZERO:
        m_error->setText(i18n("Final amount cannot be zero."));
        break;
    case HALFLIFE_ZERO:
        m_error->setText(i18n("Time is zero, please enter a valid value."));
        break;
    case FINAL_AMT_GREATER:
        m_error->setText(i18n("Final amount cannot be greater than initial amount."));
        break;
    }
}

void nuclearCalculator::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget();
    ui.setupUi(widget);
    parent->addPage(widget, i18n("General"), icon());

    ui.massOnly->setChecked(m_massOnly);

    connect(parent->okButton, &QPushButton::clicked,
            this, &nuclearCalculator::configAccepted);
    connect(parent->buttonBox, &QDialogButtonBox::Apply::clicked,
            this, &nuclearCalculator::configAccepted);
    connect(ui.massOnly, SIGNAL(toggled(bool)), parent, SLOT(settingsModified()));
}

void nuclearCalculator::configAccepted()
{
    KConfigGroup cg = config();
    QGraphicsItem::update();

    m_massOnly = ui.massOnly->isChecked();
    cg.writeEntry("massOnly", m_massOnly);

    m_configUpdated = true;
    updateConstraints();

    emit configNeedsSaving();
}
#include "nuclearCalculator.moc"
