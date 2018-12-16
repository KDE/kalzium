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

#include "gasCalculator.h"

#include <QApplication>
#include <QFontMetrics>
#include <QGraphicsGridLayout>
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
#include <Plasma/Label>
#include <Plasma/LineEdit>
#include <Plasma/PushButton>
#include <Plasma/RadioButton>
#include <Plasma/Slider>
#include <Plasma/SpinBox>

#include <KComboBox>
#include <KIntSpinBox>
#include <KTextEdit>

#include <plasma/svg.h>
#include <plasma/theme.h>

#include <KConfigDialog>
#include <KConfigGroup>

using namespace KUnitConversion;

gasCalculator::gasCalculator(QObject *parent, const QVariantList &args)
: Plasma::PopupApplet(parent, args)
, m_widget(0)
{
    m_converter = new Converter( this );
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPopupIcon("accessories-calculator");
    setHasConfigurationInterface(true);
    setAssociatedApplication("kalzium");
    resize(600, 300);
}

gasCalculator::~gasCalculator()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }
}

void gasCalculator::init()
{
    configChanged();
}

void gasCalculator::configChanged()
{
    KConfigGroup cg = config();

    cg.readEntry("ideal",true);
}

QGraphicsWidget *gasCalculator::graphicsWidget()
{
//FIXME:
// 1.> Currently the spin boxes are integer, please convert them into double
// and uncomment certain lines of code which say 'setDecimals(4)'

    if (!m_widget) {
        m_widget = new QGraphicsWidget(this);
        m_widget->setMinimumSize(500, 300);

        /**************************************************************************/
        //                       Gas Calculator set up
        /**************************************************************************/
        // setup the label
        Plasma::Frame *pHeader = new Plasma::Frame(this);
        pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        pHeader->setText(i18n("Gas Calculator"));

        //setup the grid layout
        QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(m_widget);
        pGridLayout->addItem(pHeader, 0, 0, 1, 3);

        // Set up the user interface
        // Calculation mode
        Plasma::Label *calcModeLabel = new Plasma::Label(this);
        calcModeLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        calcModeLabel->setText(i18n("Calculation Mode:"));

        m_calculationMode = new Plasma::ComboBox(this);
        m_calculationMode->setZValue(3);
        m_calculationMode->nativeWidget()->insertItems(0, QStringList()
            << i18n("Moles / Mass")
            << i18n("Pressure")
            << i18n("Temperature")
            << i18n("Volume")
        );

        pGridLayout->addItem(calcModeLabel, 1, 0);
        pGridLayout->addItem(m_calculationMode, 1, 1);

        // molar mass

        Plasma::Label *molarMassLabel = new Plasma::Label(this);
        molarMassLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        molarMassLabel->setText(i18n("Molar Mass of Gas:"));

        m_molarMass = new Plasma::SpinBox(this);
        m_molarMass->nativeWidget()->setMinimumWidth(80);
        m_molarMass->nativeWidget()->setMaximum(1000000000);
        //m_MolarMass->setDecimals(4);
        m_molarMass->setMaximum(1e+09);

        Plasma::Label *molarMassUnitLabel = new Plasma::Label(this);
        molarMassUnitLabel->nativeWidget()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        molarMassUnitLabel->setText(i18n("u (mass)"));

        pGridLayout->addItem(molarMassLabel, 2, 0);
        pGridLayout->addItem(m_molarMass, 2, 1);
        pGridLayout->addItem(molarMassUnitLabel, 2, 2);

        // moles

        Plasma::Label *molesLabel = new Plasma::Label(this);
        molesLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        molesLabel->setText(i18n("Number of moles:"));

        m_moles = new Plasma::SpinBox(this);
        m_moles->nativeWidget()->setMinimumWidth(80);
        m_moles->nativeWidget()->setMaximum(1000000000);
        //m_Moles->setDecimals(4);
        m_moles->setMaximum(1e+09);

        pGridLayout->addItem(molesLabel, 3, 0);
        pGridLayout->addItem(m_moles, 3, 1);

        // mass

        Plasma::Label *massLabel = new Plasma::Label(this);
        massLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        massLabel->setText(i18n("Mass of the gas:"));

        m_mass = new Plasma::SpinBox(this);
        m_mass->nativeWidget()->setMinimumWidth(80);
        m_mass->nativeWidget()->setMaximum(1000000000);
        //m_Mass->setDecimals(4);
        m_mass->setMaximum(1e+09);

        m_massUnit = new Plasma::ComboBox(this);
        m_massUnit->setZValue(2);
        m_massUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("grams")
         << i18n("tons")
         << i18n("carats")
         << i18n("pounds")
         << i18n("ounces")
         << i18n("troy ounces")
        );
        m_massUnit->setZValue(6);

        pGridLayout->addItem(massLabel, 4, 0);
        pGridLayout->addItem(m_mass, 4, 1);
        pGridLayout->addItem(m_massUnit, 4, 2);

        // pressure

        Plasma::Label *pressureLabel = new Plasma::Label(this);
        pressureLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        pressureLabel->setText(i18n("Pressure of the Gas:"));

        m_pressure = new Plasma::SpinBox(this);
        m_pressure->nativeWidget()->setMinimumWidth(80);
        m_pressure->nativeWidget()->setMaximum(1000000000);
        //m_Pressure->setDecimals(4);
        m_pressure->setMaximum(1e+09);

        m_pressureUnit = new Plasma::ComboBox(this);
        m_pressureUnit->setZValue(2);
        m_pressureUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("atmospheres")
         << i18n("pascal")
         << i18n("bars")
         << i18n("millibars")
         << i18n("decibars")
         << i18n("torrs")
         << i18n("inches of mercury")
        );
        m_pressureUnit->setZValue(5);

        pGridLayout->addItem(pressureLabel, 5, 0);
        pGridLayout->addItem(m_pressure, 5, 1);
        pGridLayout->addItem(m_pressureUnit, 5, 2);

        // temperature

        Plasma::Label *temperatureLabel = new Plasma::Label(this);
        temperatureLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        temperatureLabel->setText(i18n("Temperature:"));

        m_temperature = new Plasma::SpinBox(this);
        m_temperature->nativeWidget()->setMinimumWidth(80);
        m_temperature->nativeWidget()->setMaximum(1000000000);
        //m_Temp->setDecimals(4);
        m_temperature->setMaximum(1e+09);

        m_temperatureUnit = new Plasma::ComboBox(this);
        m_temperatureUnit->setZValue(2);
        m_temperatureUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("kelvins")
         << i18n("celsius")
         << i18n("fahrenheit")
         << i18n("delisles")
         << i18n("r\303\251aumurs")
        );
        m_temperatureUnit->setZValue(4);

        pGridLayout->addItem(temperatureLabel, 6, 0);
        pGridLayout->addItem(m_temperature, 6, 1);
        pGridLayout->addItem(m_temperatureUnit, 6, 2);

        // volume

        Plasma::Label *volumeLabel = new Plasma::Label(this);
        volumeLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        volumeLabel->setText(i18n("Volume of the gas:"));

        m_volume = new Plasma::SpinBox(this);
        m_volume->nativeWidget()->setMinimumWidth(80);
        m_volume->nativeWidget()->setMaximum(1000000000);
        //m_volume->setDecimals(4);
        m_volume->setMaximum(1e+09);

        m_volumeUnit = new Plasma::ComboBox(this);
        m_volumeUnit->setZValue(2);
        m_volumeUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("liter")
         << i18n("cubic feet")
         << i18n("cubic inch")
         << i18n("fluid ounce")
         << i18n("cups")
         << i18n("gallons")
         << i18n("pints")
        );
        m_volumeUnit->setZValue(3);

        pGridLayout->addItem(volumeLabel, 7, 0);
        pGridLayout->addItem(m_volume, 7, 1);
        pGridLayout->addItem(m_volumeUnit, 7, 2);

        // van der waals constant a

        Plasma::Label *vand_aLabel = new Plasma::Label(this);
        vand_aLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        vand_aLabel->setText(i18n("Van der Waals constant 'a':"));

        m_Vand_a = new Plasma::SpinBox(this);
        m_Vand_a->nativeWidget()->setMinimumWidth(80);
        m_Vand_a->nativeWidget()->setMaximum(1000000000);
        //m_Vand_A->setDecimals(4);
        m_Vand_a->setMaximum(1e+09);

        m_aUnit = new Plasma::ComboBox(this);
        m_aUnit->setZValue(2);
        m_aUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("liter^2 atmosphere/mol^2")
        );
        m_aUnit->setZValue(2);

        pGridLayout->addItem(vand_aLabel, 8, 0);
        pGridLayout->addItem(m_Vand_a, 8, 1);
        pGridLayout->addItem(m_aUnit, 8, 2);

        // van der Waals constant b

        Plasma::Label *vand_bLabel = new Plasma::Label(this);
        vand_bLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        vand_bLabel->setText(i18n("Van der Waals constant 'b':"));

        m_Vand_b = new Plasma::SpinBox(this);
        m_Vand_b->nativeWidget()->setMinimumWidth(80);
        m_Vand_b->nativeWidget()->setMaximum(1000000000);
        //m_Vand_B->setDecimals(4);
        m_Vand_b->setMaximum(1e+09);

        m_bUnit = new Plasma::ComboBox(this);
        m_bUnit->setZValue(2);
        m_bUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("liters")
         << i18n("cubic meters")
         << i18n("cubic feet")
         << i18n("cubic inches")
         << i18n("gallons")
         << i18n("cups")
        );
        m_bUnit->setZValue(1);

        pGridLayout->addItem(vand_bLabel, 9, 0);
        pGridLayout->addItem(m_Vand_b, 9, 1);
        pGridLayout->addItem(m_bUnit, 9, 2);

        // Results
        m_error = new Plasma::Label(this);
        pGridLayout->addItem(m_error, 10, 1, 4, 1);

        // Reset
        m_reset = new Plasma::PushButton(this);
        m_reset->setText(i18n("Reset"));
        pGridLayout->addItem(m_reset, 10, 0);

        // Adding objects to the UI done, now initialise
        reset();

        // Connect signals with slots
        //FIXME replace all int with double after using the doubleSpinBox
        connect(m_temperature, SIGNAL(valueChanged(int)),
                this, SLOT(tempChanged()));
        connect(m_temperatureUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(tempChanged()));
        connect(m_volume, SIGNAL(valueChanged(int)),
                this, SLOT(volChanged()));
        connect(m_volumeUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(volChanged()));
        connect(m_pressure, SIGNAL(valueChanged(int)),
                this, SLOT(pressureChanged()));
        connect(m_pressureUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(pressureChanged()));
        connect(m_mass, SIGNAL(valueChanged(int)),
                this, SLOT(massChanged()));
        connect(m_massUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(massChanged()));
        connect(m_moles, SIGNAL(valueChanged(int)),
                this, SLOT(molesChanged(int)));
        connect(m_molarMass, SIGNAL(valueChanged(int)),
                this, SLOT(molarMassChanged(int)));
        connect(m_Vand_a, SIGNAL(valueChanged(int)),
                this, SLOT(Vand_aChanged()));
        connect(m_aUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(Vand_aChanged()));
        connect(m_Vand_b,  SIGNAL(valueChanged(int)),
                this, SLOT(Vand_bChanged()));
        connect(m_bUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(Vand_bChanged()));
        connect(m_calculationMode->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(setMode(int)));
        connect(m_reset, SIGNAL(clicked()),
                this, SLOT(reset()));
    /**************************************************************************/
    // gas Calculator setup complete
    /**************************************************************************/
    }
    return m_widget;
}

void gasCalculator::reset()
{
    error(RESET_GAS_MESG);

    m_molarMass   ->setValue(2.008);
    m_temperature ->setValue(273.0);
    m_volume      ->setValue(22.4024);
    m_pressure    ->setValue(1.0);
    m_Vand_a      ->setValue(0.0);
    m_Vand_b      ->setValue(0.0);
    m_mass        ->setValue(2.016);
    m_moles       ->setValue(1.0);
    // Setup of the UI done

    m_massUnit        ->nativeWidget()->setCurrentIndex(0);
    m_pressureUnit    ->nativeWidget()->setCurrentIndex(0);
    m_temperatureUnit ->nativeWidget()->setCurrentIndex(0);
    m_volumeUnit      ->nativeWidget()->setCurrentIndex(0);
    m_bUnit           ->nativeWidget()->setCurrentIndex(0);
    m_aUnit           ->nativeWidget()->setCurrentIndex(0);
    m_calculationMode ->nativeWidget()->setCurrentIndex(3);

    // Initialise values
    m_Temp = Value(273.0, "kelvins");
    m_MolarMass = 2.016;
    m_Moles = 1.0;
    m_Mass = Value( m_MolarMass * m_Moles, "grams");
    m_Pressure = Value(1.0, "atmosphere");
    m_Vand_A = 0.0;
    m_Vand_B = Value(0.0, "liters");
    m_Vol = Value(22.4024, "liters");
    // Initialisation of values done

    setMode(3);
    molarMassChanged(2);
}
/*
    Note:-

    Van der Val's gas equation
    ( P + n^2 a / V^2) ( V - nb ) = nRT

    where P - pressure
          V - Volume
          n - number of moles
          R - Universal gas constant
          T - temperature

          a,b - Van der Val's constants
*/

// Calculates the Pressure
void gasCalculator::calculatePressure()
{
    double pressure;
    double volume = (m_converter->convert(m_Vol, "liters")).number();
    double temp = (m_converter->convert(m_Temp, "kelvins")).number();
    double b = (m_converter->convert(m_Vand_B, "liters")).number();

    pressure = m_Moles * R * temp / (volume - m_Moles * b) - m_Moles * m_Moles * m_Vand_A / volume / volume;
    m_Pressure = Value(pressure, "atmospheres");
    m_Pressure = m_converter->convert(m_Pressure, m_pressureUnit->nativeWidget()->currentText());
    m_pressure->setValue(m_Pressure.number());

    //pressure =
}

// Calculates the molar mass of the gas
void gasCalculator::calculateMolarMass()
{
    double mass = ((m_converter->convert(m_Mass, "grams")).number());
    double volume = ((m_converter->convert(m_Vol, "liters")).number());
    double pressure = ((m_converter->convert(m_Pressure, "atmospheres")).number());
    double temp = ((m_converter->convert(m_Temp, "kelvins")).number());
    double b = ((m_converter->convert(m_Vand_B, "liters")).number());

    m_MolarMass = mass * R * temp / (pressure + m_Moles * m_Moles * m_Vand_A / volume / volume)\
                  / (volume - m_Moles * b);
    m_molarMass->setValue(m_MolarMass);
}

// Calculates the Volume
void gasCalculator::calculateVol()
{
    double volume;
    double pressure = ((m_converter->convert(m_Pressure, "atmospheres")).number());
    double temp = ((m_converter->convert(m_Temp, "kelvins")).number());
    double b = ((m_converter->convert(m_Vand_B, "liters")).number());

    volume = m_Moles * R * temp / pressure + (m_Moles * b);
    m_Vol = Value(volume, "liters");
    m_Vol = (m_converter->convert(m_Vol, m_volumeUnit->nativeWidget()->currentText()));
    m_volume->setValue(m_Vol.number());
}

// Calculates the Temperature
void gasCalculator::calculateTemp()
{
    double temp;
    double volume = ((m_converter->convert(m_Vol, "liters")).number());
    double pressure = ((m_converter->convert(m_Pressure, "atmospheres")).number());
    double b = ((m_converter->convert(m_Vand_B, "liters")).number());

    temp = (pressure + (m_Moles * m_Moles * m_Vand_A / volume / volume))\
           * (volume - m_Moles * b) / m_Moles / R;
    m_Temp = Value(temp, "kelvins");
    m_Temp = (m_converter->convert(m_Temp, m_temperatureUnit->nativeWidget()->currentText()));
    m_temperature->setValue(m_Temp.number());
}

// Calculates the number of moles
void gasCalculator::calculateMoles()
{
    double volume = ((m_converter->convert(m_Vol, "liters")).number());
    double pressure = ((m_converter->convert(m_Pressure, "atmospheres")).number());
    double temp = ((m_converter->convert(m_Temp, "kelvins")).number());
    double b = ((m_converter->convert(m_Vand_B, "liters")).number());

    m_Moles = (pressure + m_Moles * m_Moles * m_Vand_A / volume / volume)\
              * (volume - m_Moles * b) / R / temp;
    m_moles->setValue(m_Moles);
}

// Calculates the mass of substance
void gasCalculator::calculateMass()
{
    double mass;
    double volume = ((m_converter->convert(m_Vol, "liters")).number());
    double pressure = ((m_converter->convert(m_Pressure, "atmospheres")).number());
    double temp = ((m_converter->convert(m_Temp, "kelvins")).number());
    double b = ((m_converter->convert(m_Vand_B, "liters")).number());

    mass = (pressure + m_Moles * m_Moles * m_Vand_A / volume / volume)\
           * (volume - m_Moles * b) * m_MolarMass / R / temp;
    m_Mass = Value(mass, "grams");
    m_Mass = (m_converter->convert(m_Mass, m_massUnit->nativeWidget()->currentText()));
    m_mass->setValue(m_Mass.number());
}


// Functions ( slots ) that occur on changing a value
// occurs when the volume is changed
void gasCalculator::volChanged()
{
    m_Vol = Value(m_volume->value(), m_volumeUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when the temperature is changed
void gasCalculator::tempChanged()
{
    m_Temp = Value(m_temperature->value(), m_temperatureUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when the pressure is changed
void gasCalculator::pressureChanged()
{
    m_Pressure = Value(m_pressure->value(), m_pressureUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when the mass is changed
void gasCalculator::massChanged()
{
    m_Mass = Value(m_mass->value(), m_massUnit->nativeWidget()->currentText());
    m_Moles = ((m_converter->convert(m_Mass, "grams")).number()) / m_MolarMass;
    m_moles->setValue(m_Moles);
    calculate();
}

//FIXME: change int value to double value in the next two functions after using the doubleSpinBox
// occurs when the number of moles is changed
void gasCalculator::molesChanged(int value)
{
    m_Moles = value;
    m_Mass = Value((m_Moles * m_MolarMass), "grams");
    m_Mass = (m_converter->convert(m_Mass, m_massUnit->nativeWidget()->currentText()));
    m_mass->setValue(m_Mass.number());
    QString temp;
    temp.setNum(value);
    calculate();
}

// occurs when the molar mass is changed
void gasCalculator::molarMassChanged(int value)
{
    if ( value == 0.0 ) {
        error(MOLAR_MASS_ZERO_);
        return;
    }
    m_MolarMass = value;
    m_Mass = Value(m_MolarMass * m_Moles, "grams");
    m_Mass = (m_converter->convert(m_Mass, m_massUnit->nativeWidget()->currentText()));
    m_mass->setValue(m_Mass.number());
    calculate();
}

// occurs when the number of moles is changed
void gasCalculator::Vand_aChanged()
{
    m_Vand_A = m_Vand_a->value();
    calculate();
}

// occurs when the number of moles is changed
void gasCalculator::Vand_bChanged()
{
    m_Vand_B = Value(m_Vand_b->value(), m_bUnit->nativeWidget()->currentText());
    calculate();
}

void gasCalculator::setMode(int mode)
{
    m_mode = mode;

    m_moles->nativeWidget()->setReadOnly(false);
    m_mass->nativeWidget()->setReadOnly(false);
    m_pressure->nativeWidget()->setReadOnly(false);
    m_temperature->nativeWidget()->setReadOnly(false);
    m_volume->nativeWidget()->setReadOnly(false);

    // set the quantity that should be calculated to readOnly
    switch (mode) {
    case 0:
        m_moles->nativeWidget()->setReadOnly(true);
        m_mass->nativeWidget()->setReadOnly(true);
        break;
    case 1:
        m_pressure->nativeWidget()->setReadOnly(true);
        break;
    case 2:
        m_temperature->nativeWidget()->setReadOnly(true);
        break;
    case 3:
        m_volume->nativeWidget()->setReadOnly(true);
        break;
    }

    calculate();
}


// occurs when any quantity is changed
void gasCalculator::calculate()
{
    error(RESET_GAS_MESG);

    switch(m_mode) {
    case 0:
        calculateMoles();
        break;
    case 1:
        calculatePressure();
        break;
    case 2:
        calculateTemp();
        break;
    case 3:
        calculateVol();
        break;
    }
}

void gasCalculator::error(int mode)
{
    switch (mode) {
    case RESET_GAS_MESG:
        m_error->setText("");
        break;
    case VOL_ZERO :
        m_error->setText(i18n("Volume cannot be zero, please enter a valid value."));
        break;
    case MOLAR_MASS_ZERO_:
        m_error->setText(i18n("The molar mass cannot be zero, please enter a non-zero value."));
    default:
        break;
    }
}

void gasCalculator::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget();
    ui.setupUi(widget);
    parent->addPage(widget, i18n("General"), icon());

    ui.ideal->setChecked(m_ideal);

    connect(parent->okButton, &QPushButton::clicked,
            this, &gasCalculator::gasConfigAccepted);
    connect(parent->buttonBox, &QDialogButtonBox::Apply::clicked,
            this, &gasCalculator::gasConfigAccepted);
    connect (ui.ideal, SIGNAL(toggled(bool)), parent, SLOT(settingsModified()));
}

void gasCalculator::gasConfigAccepted()
{
    KConfigGroup cg = config();
    QGraphicsItem::update();

    m_ideal = ui.ideal->isChecked();
    cg.writeEntry("ideal", m_ideal);

    m_configUpdated = true;
    updateConstraints();

    emit configNeedsSaving();
}

#include "gasCalculator.moc"
