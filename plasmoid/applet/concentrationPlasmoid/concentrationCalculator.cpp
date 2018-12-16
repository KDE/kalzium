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

#include "concentrationCalculator.h"

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

#include <KComboBox>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KIntSpinBox>
#include <KTextEdit>

#include <Plasma/ComboBox>
#include <Plasma/LineEdit>
#include <Plasma/Label>
#include <Plasma/Frame>
#include <Plasma/GroupBox>
#include <Plasma/RadioButton>
#include <Plasma/SpinBox>
#include <Plasma/Slider>
#include <Plasma/PushButton>

#include <plasma/svg.h>
#include <plasma/theme.h>

using namespace KUnitConversion;

concentrationCalculator::concentrationCalculator(QObject *parent, const QVariantList &args)
: Plasma::PopupApplet(parent, args)
, m_widget(0)
{
    m_converter = new Converter(this);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPopupIcon("accessories-calculator");
    setHasConfigurationInterface(true);
    setAssociatedApplication("kalzium");
    resize(700, 400);
}

concentrationCalculator::~concentrationCalculator()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }
}

void concentrationCalculator::init()
{
    configChanged();
}

void concentrationCalculator::configChanged()
{
    KConfigGroup cg = config();

    m_soluteMass = cg.readEntry("soluteMass",true);
    m_solventVolume = cg.readEntry("solventVolume",true);
}

QGraphicsWidget *concentrationCalculator::graphicsWidget()
{
//FIXME:
// 1.> Currently the spin boxes are integer, please convert them into double
// and uncomment certain lines of code which say 'setDecimals(4)'

    if (!m_widget) {
        m_widget = new QGraphicsWidget(this);
        m_widget->setMinimumSize(600, 350);

        // setup the label
        Plasma::Frame *pHeader = new Plasma::Frame(this);
        pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        pHeader->setText(i18n("Concentration Calculator"));

        //setup the layout
        QGraphicsLinearLayout *pVLayout = new QGraphicsLinearLayout(Qt::Vertical,m_widget);
        Plasma::GroupBox *pGroupBox1 = new Plasma::GroupBox(this);
        QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(pGroupBox1);
        pGridLayout->addItem(pHeader, 0, 0, 1, 4);
        pVLayout->addItem(pGroupBox1);

        // Set up the user interface

        // 1 Calculation mode

        Plasma::Label *calcModeLabel = new Plasma::Label(this);
        calcModeLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        calcModeLabel->setText(i18n("Calculation Mode:"));

        m_calculationMode = new Plasma::ComboBox(this);

        m_calculationMode->setZValue(3);
        m_calculationMode->nativeWidget()->insertItems(0, QStringList()
         << i18n("Amount Solute")
         << i18n("Molar Mass of Solute")
         << i18n("Equivalent Mass")
         << i18n("Amount Solvent")
         << i18n("Molar Mass of Solvent")
         << i18n("Concentration")
        );

        pGridLayout->addItem(calcModeLabel, 1, 0);
        pGridLayout->addItem(m_calculationMode, 1, 1);
        // 2 amount solute

        Plasma::Label *amtSltLabel = new Plasma::Label(this);
        amtSltLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        amtSltLabel->setText(i18n("Amount of solute:"));

        m_amountSolute = new Plasma::SpinBox(this);
        m_amountSolute->nativeWidget()->setMaximum(1000000000);
        //amtSolute->setDecimals(4);
        m_amountSolute->setMaximum(1e+09);

        m_amountSoluteType = new Plasma::ComboBox(this);
        m_amountSoluteType->setZValue(2);
        m_amountSoluteType->nativeWidget()->insertItems(0, QStringList()
         << i18n("Mass")
         << i18n("volume")
         << i18n("moles")
        );

        m_amountSoluteUnit = new Plasma::ComboBox(this);
        m_amountSoluteUnit->setZValue(6);
        m_amountSoluteUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("grams")
         << i18n("tons")
         << i18n("carats")
         << i18n("pounds")
         << i18n("ounces")
         << i18n("troy ounces")
        );

        pGridLayout->addItem(amtSltLabel, 2, 0);
        pGridLayout->addItem(m_amountSolute, 2, 1);
        pGridLayout->addItem(m_amountSoluteType, 2, 3);
        pGridLayout->addItem(m_amountSoluteUnit, 2, 2);
        // 3 molar mass solute

        Plasma::Label *molarMassLabel = new Plasma::Label(this);
        molarMassLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        molarMassLabel->setText(i18n("Molar mass of solute:"));

        m_molarMass = new Plasma::SpinBox(this);
        m_molarMass->nativeWidget()->setMaximum(1000000000);
        //m_MolarMass->setDecimals(4);
        m_molarMass->setMaximum(1e+09);

        Plasma::Label *molarMassUnit = new Plasma::Label(this);
        molarMassUnit->nativeWidget()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        molarMassUnit->setText(i18n("u (mass)"));

        pGridLayout->addItem(molarMassLabel, 3, 0);
        pGridLayout->addItem(m_molarMass, 3, 1);
        pGridLayout->addItem(molarMassUnit, 3, 2);

        // 4 equivalent mass solute
        Plasma::Label *eqtMassLabel = new Plasma::Label(this);
        eqtMassLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        eqtMassLabel->setText(i18n("Equivalent mass of solute:"));

        m_eqtMass = new Plasma::SpinBox(this);
        m_eqtMass->nativeWidget()->setMaximum(1000000000);

        //m_eqtMass->setDecimals(4);
        m_eqtMass->setMaximum(1e+09);

        Plasma::Label *eqtMassUnit = new Plasma::Label(this);
        eqtMassUnit->nativeWidget()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        eqtMassUnit->setText(i18n("u (mass)"));

        pGridLayout->addItem(eqtMassLabel, 4, 0);
        pGridLayout->addItem(m_eqtMass, 4, 1);
        pGridLayout->addItem(eqtMassUnit, 4, 2);

        // 5 density solute

        Plasma::Label *densitySoluteLabel = new Plasma::Label(this);
        densitySoluteLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        densitySoluteLabel->setText(i18n("Density of solute:"));

        m_densitySolute = new Plasma::SpinBox(this);
        m_densitySolute->nativeWidget()->setMaximum(1000000000);
        //m_densitySolute->setDecimals(4);
        m_densitySolute->setMaximum(1e+09);

        m_densitySoluteUnit = new Plasma::ComboBox(this);
        m_densitySoluteUnit->setZValue(5);
        m_densitySoluteUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("grams per liter")
         << i18n("grams per milliliter")
         << i18n("kilograms per cubic meter")
         << i18n("kilograms per liter")
         << i18n("ounces per cubic inch")
         << i18n("ounces per cubic foot")
         << i18n("pounds per cubic inch")
         << i18n("pounds per cubic foot")
         << i18n("pounds per cubic yard")
        );
        pGridLayout->addItem(densitySoluteLabel, 5, 0);
        pGridLayout->addItem(m_densitySolute, 5, 1);
        pGridLayout->addItem(m_densitySoluteUnit, 5, 2);

        // 6 amount solvent
        Plasma::Label *amtSlvtLabel = new Plasma::Label(this);
        amtSlvtLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        amtSlvtLabel->setText(i18n("Amount of solvent:"));

        m_amountSolvent = new Plasma::SpinBox(this);
        m_amountSolvent->nativeWidget()->setMaximum(1000000000);
        //m_amountSolvent->setDecimals(4);
        m_amountSolvent->setMaximum(1e+09);

        m_amountSolventType = new Plasma::ComboBox(this);
        m_amountSolventType->setZValue(2);
        m_amountSolventType->nativeWidget()->insertItems(0, QStringList()
         << i18n("volume")
         << i18n("Mass")
         << i18n("moles")
        );
        m_amountSolventType->setZValue(3);

        m_amountSolventUnit = new Plasma::ComboBox(this);
        m_amountSolventUnit->setZValue(4);
        m_amountSolventUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("liter")
         << i18n("cubic meters")
         << i18n("cubic feet")
         << i18n("cubic inch")
         << i18n("cubic mile")
         << i18n("fluid ounce")
         << i18n("cups")
         << i18n("gallons")
         << i18n("pints")
        );

        pGridLayout->addItem(amtSlvtLabel, 6, 0);
        pGridLayout->addItem(m_amountSolvent, 6, 1);
        pGridLayout->addItem(m_amountSolventType, 6, 3);
        pGridLayout->addItem(m_amountSolventUnit, 6, 2);

        // 7 molar mass solvent

        Plasma::Label *molarMassSolvtLabel = new Plasma::Label(this);
        molarMassSolvtLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        molarMassSolvtLabel->setText(i18n("Molar mass of solvent:"));

        m_molarMassSolvent = new Plasma::SpinBox(this);
        m_molarMassSolvent->nativeWidget()->setMaximum(1000000000);
        //m_MolarMassSolvent->setDecimals(4);
        m_molarMassSolvent->setMaximum(1e+09);

        Plasma::Label *molarMassSolvtUnit = new Plasma::Label(this);
        molarMassSolvtUnit->nativeWidget()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        molarMassSolvtUnit->setText(i18n("u (mass)"));

        pGridLayout->addItem(molarMassSolvtLabel, 7, 0);
        pGridLayout->addItem(m_molarMassSolvent, 7, 1);
        pGridLayout->addItem(molarMassSolvtUnit, 7, 2);

        // 8 density of solvent

        Plasma::Label *densitySolventLabel = new Plasma::Label(this);
        densitySolventLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        densitySolventLabel->setText(i18n("Density of solvent:"));

        m_densitySolvent = new Plasma::SpinBox(this);
        m_densitySolvent->nativeWidget()->setMaximum(1000000000);
        //m_densitySolvent->setDecimals(4);
        m_densitySolvent->setMaximum(1e+09);

        m_densitySolventUnit = new Plasma::ComboBox(this);
        m_densitySolventUnit->setZValue(3);
        m_densitySolventUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("grams per liter")
         << i18n("grams per milliliter")
         << i18n("kilograms per cubic meter")
         << i18n("kilograms per liter")
         << i18n("ounces per cubic inch")
         << i18n("ounces per cubic foot")
         << i18n("pounds per cubic inch")
         << i18n("pounds per cubic foot")
         << i18n("pounds per cubic yard")
        );

        pGridLayout->addItem(densitySolventLabel, 8, 0);
        pGridLayout->addItem(m_densitySolvent, 8, 1);
        pGridLayout->addItem(m_densitySolventUnit, 8, 2);

        // 9 Concentration

        Plasma::Label *concentrationLabel = new Plasma::Label(this);
        concentrationLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        concentrationLabel->setText(i18n("Concentration:"));

        m_concentration = new Plasma::SpinBox(this);
        m_concentration->nativeWidget()->setMaximum(1000000000);
        //m_concentration->setDecimals(4);
        m_concentration->setMaximum(1e+09);

        m_concentrationUnit = new Plasma::ComboBox(this);
        m_concentrationUnit->setZValue(2);
        m_concentrationUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("molar")
         << i18n("Normal")
         << i18n("molal")
         << i18n("% ( mass )")
         << i18n("% ( volume )")
         << i18n("% ( moles )")
        );

        pGridLayout->addItem(concentrationLabel, 9, 0);
        pGridLayout->addItem(m_concentration, 9, 1);
        pGridLayout->addItem(m_concentrationUnit, 9, 2);

        // 11 reset
        m_reset = new Plasma::PushButton(this);
        m_reset->setText(i18n("Reset"));
        pGridLayout->addItem(m_reset, 10, 0);

        // 10 Results
        m_error = new Plasma::Label(this);
        pGridLayout->addItem(m_error, 10, 1, 4, 1);

        // Done adding elements to the UI, now initialise
        reset();

        // Connect signals with slots (when a change of selection in the UI takes place,
        // corresponding quantity should be updated in the class.)
        connect(m_amountSolute, SIGNAL(valueChanged(int)),
                this, SLOT(amountSoluteChanged()));
        connect(m_amountSoluteType->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(amountSoluteTypeChanged()));
        connect(m_amountSoluteUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(amountSoluteChanged()));
        connect(m_molarMass, SIGNAL(valueChanged(int)),
                this, SLOT(molarMassChanged(int)));
        connect(m_eqtMass, SIGNAL(valueChanged(int)),
                this, SLOT(eqtMassChanged(int)));
        connect(m_densitySolute, SIGNAL(valueChanged(int)),
                this, SLOT(densitySoluteChanged()));
        connect(m_densitySoluteUnit->nativeWidget(),  SIGNAL(activated(int)),
                this, SLOT(densitySoluteChanged()));
        connect(m_amountSolvent, SIGNAL(valueChanged(int)),
                this, SLOT(amountSolventChanged()));
        connect(m_amountSolventType->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(amountSolventTypeChanged()));
        connect(m_amountSolventUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(amountSolventChanged()));
        connect(m_amountSolventUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(amountSolventChanged()));
        connect(m_molarMassSolvent, SIGNAL(valueChanged(int)),
                this, SLOT(molarMassSolventChanged(int)));
        connect(m_densitySolvent, SIGNAL(valueChanged(int)),
                this, SLOT(densitySolventChanged()));
        connect(m_densitySolventUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(densitySolventChanged()));
        connect(m_concentration, SIGNAL(valueChanged(int)),
                this, SLOT(concentrationChanged(int)));
        connect(m_concentrationUnit->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(concentrationChanged(int)));
        connect(m_calculationMode->nativeWidget(), SIGNAL(activated(int)),
                this, SLOT(setMode(int)));
        connect(m_reset, SIGNAL(clicked()),
                this, SLOT(reset()));
        /**************************************************************************/
        //              concentration Calculator setup complete
        /**************************************************************************/
    }
    return m_widget;
}

void concentrationCalculator::reset()
{
    /**************************************************************************/
    //                       concentration Calculator set up
    /**************************************************************************/
    error(RESET_CONC_MESG);
    // initialise the initially selected values
    m_amountSolute     ->setValue(117.0);
    m_molarMass        ->setValue(58.5);
    m_eqtMass          ->setValue(58.5);
    m_densitySolute    ->setValue(2.7);
    m_amountSolvent    ->setValue(1.0);
    m_molarMassSolvent ->setValue(18.0);
    m_densitySolvent   ->setValue(1000.0);
    m_concentration    ->setValue(2.0);
    m_mode = 5;
    // Setup of the UI done

    // Initialise values
    m_AmtSolute = Value(117.0, "grams");
    m_AmtSolvent = Value(1.0, "liter");
    m_MolarMass = 58.5;
    m_EqtMass = 58.5;
    m_MolesSolute = 2.0;
    m_MolesSolvent = 55.5;
    m_MolarMassSolvent = 18.0;
    m_DensitySolute = Value(2.7, "grams per milliliter");
    m_Concentration = 2.0;
    m_DensitySolvent = Value(1000.0, "grams per liter");

    m_amountSoluteType   ->nativeWidget()->setCurrentIndex(0);
    m_amountSolventType  ->nativeWidget()->setCurrentIndex(0);
    m_amountSoluteUnit   ->nativeWidget()->setCurrentIndex(0);
    m_amountSolventUnit  ->nativeWidget()->setCurrentIndex(0);
    m_densitySolventUnit ->nativeWidget()->setCurrentIndex(0);
    m_densitySoluteUnit  ->nativeWidget()->setCurrentIndex(0);
    m_concentrationUnit  ->nativeWidget()->setCurrentIndex(0);
    m_calculationMode    ->nativeWidget()->setCurrentIndex(5);

    setMode(5);
    calculate();
    // Initialisation of values done
}
// Calculates the amount of solute
void concentrationCalculator::calculateAmountSolute()
{
    int type1 = m_concentrationUnit->nativeWidget()->currentIndex();
    int type2 = m_amountSoluteType->nativeWidget()->currentIndex();

    double molesSolute, eqtsSolute, massSolute, volSolute;     // variables
    int mode = 0;
    /*
     * mode = 1 (molesSolute) mode = 2 eqtsSolute, mode = 3 mass, 4 volume
     */
    // Calculate the number of moles of the solute
    switch (type1) {
    // calculate the number of moles of solute
    case 0: // molarity specified
        molesSolute = m_Concentration * volumeSolvent();
        mode = 1;
        break;
        // Calculate the number of equivalents of solute
    case 1: // Normality specified
        eqtsSolute = m_Concentration * volumeSolvent();
        mode = 2;
        break;
        // Calculate the number of moles of solute
    case 2: // molality specified
        molesSolute = m_Concentration * massSolvent() / 1000.0;
        mode = 1;
        break;
        // Calculate the mass of solute
    case 3: // mass percentage specified
        if (m_Concentration >= 100.0) {
            error(PERCENTAGE_ZERO);
        }
        massSolute = m_Concentration / (100.0 - m_Concentration) * massSolvent();
        mode = 3;
        break;
        // Calculate the volume of solute
    case 4: // volume percentage specified
        if (m_Concentration >= 100.0) {
            error(PERCENTAGE_ZERO);
        }
        volSolute = m_Concentration / (100.0 - m_Concentration) * volumeSolvent();
        mode = 4;
        break;
        // Calculate the moles of solute
    case 5: //mole percentage specified
        if (m_Concentration >= 100.0) {
            error(PERCENTAGE_ZERO);
        }
        molesSolute = m_Concentration / (100.0 - m_Concentration) * molesSolvent();
        mode = 1;
        break;
    default:
        break;
    }

    // We have the amount of solvent in some form (moles, equivalents, mass, volume etc)
    // Now we have to present it in the UI
    switch (type2) {
    case 0: // amount should be specified in terms of mass
        switch (mode) {
        case 1: // we should get mass from moles
            massSolute = molesSolute * m_MolarMass;
            break;
        case 2: // we should obtain mass from number of equivalents
            massSolute = eqtsSolute * m_EqtMass;
            break;
        case 3: // we already know the mass of the solute
            break;
        case 4: // we should get the mass from volume
            massSolute = volSolute * densitySolute();
            break;
        }
        // update mass of solute
        m_AmtSolute = Value(massSolute, "grams");
        m_AmtSolute = (m_converter->convert(m_AmtSolute, \
                       m_amountSoluteUnit->nativeWidget()->currentText()));
        m_amountSolute->setValue(m_AmtSolute.number());
        break;

    case 1: // amount should be specified in terms of volume
        // validate density
        if (densitySolute() == 0) {
            error(DENSITY_ZERO);
            return;
        }
        switch (mode) {
        case 1: // we should get the volume from moles
            volSolute = molesSolute * m_MolarMass / densitySolute();
            break;
        case 2: // we should get the volume from equivalents
            volSolute = eqtsSolute * m_EqtMass / densitySolute();
            break;
        case 3: // we should get the volume from mass
            volSolute = massSolute / densitySolute();
            break;
        case 4: // we already know the volume
            break;
        }
        // update volume of solute
        m_AmtSolute = Value(volSolute, "liters");
        m_AmtSolute = (m_converter->convert(m_AmtSolute, \
                       m_amountSoluteUnit->nativeWidget()->currentText()));
        m_amountSolute->setValue(m_AmtSolute.number());
        break;

    case 2: // amount should be specified in terms of moles
        switch (mode) {
        case 1: // we already know the moles of solute
            break;
        case 2: // we should obtain moles from equivalents (not possible)
            molesSolute = 0.0;
            break;
        case 3: // we should obtain moles from mass
            molesSolute = massSolute / m_MolarMass;
            break;
        case 4: // we should obtain moles from volume
            molesSolute = volSolute * densitySolute() / m_MolarMass;
            break;
        }
        // Update the number of moles
        m_MolesSolute = molesSolute;
        m_amountSolute->setValue(molesSolute);
        break;
    }
    return;
}

// Calculates the molar mass
void concentrationCalculator::calculateMolarMass()
{
    // molarity / molality / mole fraction required
    int type = m_concentrationUnit->nativeWidget()->currentIndex();
    int type2 = m_amountSolventType->nativeWidget()->currentIndex();
    double numMoles;
    switch (type) {
    case 0:     //molarity specified
        // number of moles = volume * concentration
        numMoles = volumeSolvent() * m_Concentration;
        break;
    case 1:     // cannot be calculated (insufficient data)
        error(INSUFFICIENT_DATA_MOLE);
        return;
        break;
    case 2:     // molality specified
        numMoles = massSolvent() / 1000.0 * m_Concentration;
        break;
    case 3:     // cannot be calculated (insufficient data)
    case 4:
        error(INSUFFICIENT_DATA_MOLE);
        return;
        break;
    case 5:     // mole fraction specified
        numMoles = m_Concentration / (100.0 - m_Concentration) * molesSolvent();
        break;
    }

    if (type2 == 2) { // amount of solute is specified in moles, cannot calculate
        error(INSUFFICIENT_DATA_MOLES);
        return;
    } else {
        if (numMoles == 0.0) {
            error(MOLES_ZERO);
            return;
        }
        m_MolarMass = massSolute() / numMoles;
        m_molarMass->setValue(m_MolarMass);
    }
}

// Calculates the equivalent mass
void concentrationCalculator::calculateEqtMass()
{
    // Normality required
    int type = m_concentrationUnit->nativeWidget()->currentIndex();
    int type2 = m_amountSoluteType->nativeWidget()->currentIndex();

    double numEqts;
    switch (type) {
        // Normality required
    case 0: // molarity not sufficient
        error(INSUFFICIENT_DATA_EQT);
        return;
        break;
    case 1: // normality specified
        numEqts = volumeSolvent() * m_Concentration;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        error(INSUFFICIENT_DATA_EQT);
        return;
        break;
    }

    if (type2 == 2) {    // Amount of solute specified in moles, cannot calculate
        error(INSUFFICIENT_DATA_MOLES);
    } else {
        if (numEqts == 0.0) {
            error(EQTS_ZERO);
            return;
        }
        m_EqtMass = massSolute() / numEqts;
        m_eqtMass->setValue(m_EqtMass);
    }
    return;
}

// Calculates the calculate molar mass of the solvent
void concentrationCalculator::calculateMolarMassSolvent()
{
    // molarity / molality / mole fraction required
    int type = m_concentrationUnit->nativeWidget()->currentIndex();
    int type2 = m_amountSolventType->nativeWidget()->currentIndex();
    double numMoles;
    switch (type) {
    case 0:         // molarity specified
    case 1:         // normality specified
    case 2:         // molality specified
    case 3:         // mass fraction specified
    case 4:         // volume fraction specified
        error(INSUFFICIENT_DATA_SOLVENT);
        return;
        break;      // cannot be calculated (insufficient data)
    case 5:         // mole fraction specified
        numMoles = (100.0 - m_Concentration) / m_Concentration * molesSolute();
        break;
    }

    if (type2 == 2) {    // amount specified in moles
        error(INSUFFICIENT_DATA_MOLES);
    } else {
        m_MolarMassSolvent = massSolvent() / numMoles;
        m_molarMassSolvent->setValue(m_MolarMassSolvent);
    }

    return;
}

// Calculates the amount of solvent
void concentrationCalculator::calculateAmountSolvent()
{
    int type1 = m_concentrationUnit->nativeWidget()->currentIndex();
    int type2 = m_amountSolventType->nativeWidget()->currentIndex();

    double moleSolvent, massSolvent, volSolvent;

    int mode = 0;               // Indicates the mode in which we have calculated the amount of solvent
    /*
     * mode = 1 (molessolvent) mode = 2 eqtssolvent, mode = 3 mass, 4 volume
     */
    // Calculate the number of moles of the solvent
    if (m_Concentration == 0.0) {
        error(CONC_ZERO);
        return;
    }

    switch (type1) {
        // calculate the number of moles of solvent
    case 0: // molarity specified
        volSolvent = molesSolute() / m_Concentration;
        mode = 3;
        break;
        // Calculate the number of equivalents of solvent
    case 1: // Normality specified
        volSolvent = eqtsSolute() / m_Concentration;
        mode = 3;
        break;
        // Calculate the number of moles of solvent
    case 2: // molality specified
        massSolvent = molesSolute() * 1000.0 / m_Concentration;
        mode = 2;
        break;
        // Calculate the mass of solvent
    case 3: // mass percentage specified
        massSolvent = (100.0 - m_Concentration) / m_Concentration;
        massSolvent *= massSolute();
        mode = 2;
        break;
        // Calculate the volume of solvent
    case 4: // volume percentage specified
        volSolvent = (100.0 - m_Concentration) / m_Concentration;
        volSolvent *= volumeSolute();
        mode = 3;
        break;
        // Calculate the moles of solvent
    case 5: //mole percentage specified
        moleSolvent = (100.0 - m_Concentration) / m_Concentration;
        moleSolvent *= molesSolute();
        mode = 1;
        break;
    default:
        break;
    }

    // We have the amount of solvent in some form (moles, equivalents, mass, volume etc)
    // Now we have to present it in the UI
    if (densitySolvent() == 0.0) {
        error(DENSITY_ZERO);
        return;
    }
    if (m_MolarMassSolvent == 0.0) {
        error(MOLAR_SOLVENT_ZERO);
        return;
    }
    switch (type2) {
    case 0: // amount should be specified interms of volume
        switch (mode) {
        case 1: // obtain volume from moles
            volSolvent = moleSolvent * m_MolarMassSolvent / densitySolvent();
            break;
        case 2: // obtain volume from mass
            volSolvent = massSolvent / densitySolvent();
            break;
        case 3: // volume already known
            break;
        }
        m_AmtSolvent = Value(volSolvent, "liters");
        m_AmtSolvent = (m_converter->convert(m_AmtSolvent, \
                        m_amountSolventUnit->nativeWidget()->currentText()));
        m_amountSolvent->setValue(m_AmtSolvent.number());
        break;

    case 1: // amount should be specified in terms of mass
        switch (mode) {
        case 1: // obtain mass from moles
            massSolvent = moleSolvent / m_MolarMassSolvent;
            break;
        case 2: // mass already known
            break;
        case 3: // obtain mass from volume
            massSolvent = volSolvent / densitySolvent();
            break;
        }
        m_AmtSolvent = Value(massSolvent, "grams");
        m_AmtSolvent = (m_converter->convert(m_AmtSolvent, \
                        m_amountSolventUnit->nativeWidget()->currentText()));
        m_amountSolvent->setValue(m_AmtSolvent.number());
        break;

    case 2: // amount should be specified in terms of moles
        switch (mode) {
        case 1: // moles already known
            break;
        case 2: // obtain moles from mass
            moleSolvent = massSolvent / m_MolarMassSolvent;
            break;
        case 3: // obtain moles from volume
            moleSolvent = volSolvent * densitySolvent() / m_MolarMassSolvent;
            break;
        }
        m_MolesSolvent =  moleSolvent;
        m_amountSolvent->setValue(moleSolvent);
        break;
    }
    return;
}

// calculates the concentration
void concentrationCalculator::calculateConcentration()
{
    int type = m_concentrationUnit->nativeWidget()->currentIndex();

    if (volumeSolvent() == 0.0) {
        error(VOLUME_ZERO);
        return;
    }
    if (massSolvent() == 0.0) {
        error(MASS_ZERO);
        return;
    }
    if (molesSolvent() == 0.0) {
        error(MOLES_ZERO);
        return;
    }
    switch (type) {
    case 0:     // molarity
        m_Concentration = molesSolute() / volumeSolvent();
        break;
    case 1:     // normality
        m_Concentration = eqtsSolute() / volumeSolvent();
        break;
    case 2:     // molality
        m_Concentration = molesSolute() * 1000.0 / massSolvent();
        break;
    case 3:     // mass fraction
        m_Concentration = massSolute() / (massSolute() + massSolvent()) * 100.0;
        break;
    case 4:     // volume fraction
        m_Concentration = volumeSolute() / (volumeSolute() + volumeSolvent()) * 100.0;
        break;
    case 5:     // mole fraction
        m_Concentration = molesSolute() / (molesSolute() + molesSolvent()) * 100.0;
        break;
    default:
        break;
    }
    m_concentration->setValue(m_Concentration);
    return;
}

double concentrationCalculator::volumeSolvent()
{
    int type = m_amountSolventType->nativeWidget()->currentIndex();
    double volume;
    switch (type) {
    case 0:
        volume = (m_converter->convert(m_AmtSolvent, "liter")).number();
        break;
    case 1:
        volume = massSolvent() / densitySolvent();
        break;
    case 2:
        volume = massSolvent() / densitySolvent();
    default:
        break;
    }
    return volume;
}

double concentrationCalculator::molesSolvent()
{
    int type = m_amountSolventType->nativeWidget()->currentIndex();

    double moles;
    switch (type) {
    case 0:
        moles = massSolvent() / m_MolarMassSolvent;
        break;
    case 1:
        moles = massSolvent() / m_MolarMassSolvent;
        break;
    case 2:
        moles = m_MolesSolvent;
        break;
    default:
        break;
    }
    return moles;

}
double concentrationCalculator::massSolvent()
{
    int type = m_amountSolventType->nativeWidget()->currentIndex();
    double mass;
    switch (type) {
    case 0:
        mass = volumeSolvent() * densitySolvent();
        break;
    case 1:
        mass = (m_converter->convert(m_AmtSolvent, "gram")) .number();
        break;
    case 2:
        mass = m_MolesSolvent * m_MolarMassSolvent;
        break;
    default:
        break;
    }
    return mass;
}

double concentrationCalculator::densitySolvent()
{
    return ((m_converter->convert(m_DensitySolvent, "grams per liter")).number());
}

double concentrationCalculator::volumeSolute()
{
    int type = m_amountSoluteType->nativeWidget()->currentIndex();
    double volume;
    switch (type) {
    case 0:
        volume = massSolute() / densitySolute();
        break;
    case 1:
        volume = (m_converter->convert(m_AmtSolute, "liter")).number();
        break;
    case 2:
        volume = massSolute() / densitySolute();
    default:
        break;
    }
    return volume;
}

double concentrationCalculator::molesSolute()
{
    int type = m_amountSoluteType->nativeWidget()->currentIndex();

    double moles;
    if (m_MolarMass == 0.0) {
        error(MOLAR_MASS_ZERO);
        return 1.0;
    }
    switch (type) {
    case 0:
        moles = massSolute() / m_MolarMass;
        break;
    case 1:
        moles = massSolute() / m_MolarMass;
        break;
    case 2:
        moles = m_MolesSolute;
        break;
    default:
        break;
    }
    return moles;
}

double concentrationCalculator::eqtsSolute()
{
    int type = m_amountSoluteType->nativeWidget()->currentIndex();
    double eqts;
    if (m_EqtMass == 0.0) {
        error(EQT_MASS_ZERO);
        return 1.0;
    }
    switch (type) {
    case 0:
        eqts = massSolute() / m_EqtMass;
        break;
    case 1:
        eqts = massSolute() / m_EqtMass;
        break;
    case 2:
        // Cannot be calculated
        error(INSUFFICIENT_DATA_MOLES);
        eqts = 1.0;
        break;
    default:
        break;
    }
    return eqts;

}

double concentrationCalculator::massSolute()
{
    int type = m_amountSoluteType->nativeWidget()->currentIndex();
    double mass;
    switch (type) {
    case 0:
        mass = (m_converter->convert(m_AmtSolute, "gram")) .number();
        break;
    case 1:
        mass = volumeSolute() * densitySolute();
        break;
    case 2:
        mass = m_MolesSolute * m_MolarMass;
        break;
    default:
        break;
    }
    return mass;
}

double concentrationCalculator::densitySolute()
{
    return ((m_converter->convert(m_DensitySolute, "grams per liter")).number());
}


// occurs when the amount of solute is changed
void concentrationCalculator::amountSoluteChanged()
{
    int type = m_amountSoluteType->nativeWidget()->currentIndex();
    switch (type) {
    case 0:
    case 1:
        m_AmtSolute = Value(m_amountSolute->value(), m_amountSoluteUnit->nativeWidget()->currentText());
        break;
    case 2:
        m_MolesSolute = m_amountSolute->value();
        break;
    }
    calculate();
}

// occurs when the type in which amount of solute is specified is changed
void concentrationCalculator::amountSoluteTypeChanged()
{
    int type = m_amountSoluteType->nativeWidget()->currentIndex();
    if (type == 0) {         // amount of solute specified in terms of mass
        m_amountSoluteUnit->nativeWidget()->clear();
        m_amountSoluteUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("grams")
         << i18n("tons")
         << i18n("carats")
         << i18n("pounds")
         << i18n("ounces")
         << i18n("troy ounces")
        );
        m_amountSoluteUnit->show();
        m_AmtSolute = Value(m_amountSolute->value(), m_amountSoluteUnit->nativeWidget()->currentText());
    } else if (type == 1) { // amount of solute is specified in terms of volume
        m_amountSoluteUnit->nativeWidget()->clear();
        m_amountSoluteUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("liter")
         << i18n("cubic meters")
         << i18n("cubic feet")
         << i18n("cubic inch")
         << i18n("cubic mile")
         << i18n("fluid ounce")
         << i18n("cups")
         << i18n("gallons")
         << i18n("pints")
        );
        m_amountSoluteUnit->show();
        m_AmtSolute = Value(m_amountSolute->value(), m_amountSoluteUnit->nativeWidget()->currentText());
    } else {                 // amount of solute is specified in terms of moles
        m_MolesSolute = m_amountSolute->value();
        m_amountSoluteUnit->hide();
    }
    calculate();
}
// occurs when the amount of solvent is changed
void concentrationCalculator::amountSolventChanged()
{
    int type = m_amountSolventType->nativeWidget()->currentIndex();
    switch (type) {     // amount of solvent specified in terms of volume
    case 0:
    case 1:
        m_AmtSolvent = Value(m_amountSolvent->value(), m_amountSolventUnit->nativeWidget()->currentText());
        break;
    case 2:
        m_MolesSolvent = m_amountSolvent->value();
        break;
    }
    calculate();
}

void concentrationCalculator::amountSolventTypeChanged()
{
    int type = m_amountSolventType->nativeWidget()->currentIndex();
    if (type == 0) {     // amount of solvent specified in terms of volume
        m_amountSolventUnit->nativeWidget()->clear();
        m_amountSolventUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("liter")
         << i18n("cubic meters")
         << i18n("cubic feet")
         << i18n("cubic inch")
         << i18n("cubic mile")
         << i18n("fluid ounce")
         << i18n("cups")
         << i18n("gallons")
         << i18n("pints")
        );
        m_amountSolventUnit->show();
        m_AmtSolvent = Value(m_amountSolvent->value(), m_amountSolventUnit->nativeWidget()->currentText());
    } else if (type == 1) { // amount of solvent is specified in terms of mass
        m_amountSolventUnit->nativeWidget()->clear();
        m_amountSolventUnit->nativeWidget()->insertItems(0, QStringList()
         << i18n("grams")
         << i18n("tons")
         << i18n("carats")
         << i18n("pounds")
         << i18n("ounces")
         << i18n("troy ounces")
        );
        m_amountSolventUnit->show();
        m_AmtSolvent = Value(m_amountSolvent->value(), m_amountSolventUnit->nativeWidget()->currentText());
    } else { // amount is specified in terms of moles
        m_amountSolventUnit->hide();
        m_MolesSolvent = m_amountSolvent->value();
    }
    calculate();
}

//FIXME replace 'int value' with 'double value'
// occurs when the molar mass of solute is changed
void concentrationCalculator::molarMassChanged(int value)
{
    m_MolarMass = value;
    calculate();
}

// occurs when the equivalent mass of solute is changed
void concentrationCalculator::eqtMassChanged(int value)
{
    m_EqtMass = value;
    calculate();
}

// occurs when the molar mass of solvent is changed
void concentrationCalculator::molarMassSolventChanged(int value)
{
    m_MolarMassSolvent = value;
    calculate();
}

// occurs when the number of moles is changed
void concentrationCalculator::densitySoluteChanged()
{
    m_DensitySolute = Value(m_densitySolute->value(), m_densitySoluteUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when the density of solvent is changed
void concentrationCalculator::densitySolventChanged()
{
    m_DensitySolvent = Value(m_densitySolvent->value(), m_densitySolventUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when the concentration is changed
void concentrationCalculator::concentrationChanged(int value)
{
    m_Concentration = value;
    calculate();
}

// This function is called when the mode of calculation is changed
void concentrationCalculator::setMode (int mode)
{
    // If there is no change, return.
    if (m_mode == mode) {
        return;
    }

    // set all to writable
    m_amountSolute->nativeWidget()->setReadOnly(false);
    m_molarMass->nativeWidget()->setReadOnly(false);
    m_eqtMass->nativeWidget()->setReadOnly(false);
    m_amountSolvent->nativeWidget()->setReadOnly(false);
    m_molarMassSolvent->nativeWidget()->setReadOnly(false);
    m_concentration->nativeWidget()->setReadOnly(false);

    // set the value that should be calculated to readOnly
    switch (mode) {
    case 0:     // Calculate the amount of solute
        m_amountSolute->nativeWidget()->setReadOnly(true);
        break;
    case 1:     // Calculate the molar mass of solute
        m_molarMass->nativeWidget()->setReadOnly(true);
        break;
    case 2:     // Calculate the equivalent mass of solute
        m_eqtMass->nativeWidget()->setReadOnly(true);
        break;
    case 3:     // Calculate the amount of solvent
        m_amountSolvent->nativeWidget()->setReadOnly(true);
        break;
    case 4:     // Calculate the molar mass of solvent
        m_molarMassSolvent->nativeWidget()->setReadOnly(true);
        break;
    case 5:     // Calculate the concentration of the solution
        m_concentration->nativeWidget()->setReadOnly(true);
        break;
    }

    m_mode = mode;
    calculate();
}
// occurs when any quantity is changed
void concentrationCalculator::calculate()
{
    error(RESET_CONC_MESG);
    switch (m_calculationMode->nativeWidget()->currentIndex()) {
    case 0: // Calculate the amount of solute
        if (m_concentrationUnit->nativeWidget()->currentIndex() > 2 && m_concentration->value() > 100) {
            error(PERCENTAGE_ZERO);
            return;
        }
        calculateAmountSolute();
        break;
    case 1: // Calculate the molar mass of solute
        calculateMolarMass();
        break;
    case 2: // Calculate the equivalent mass of solute
        calculateEqtMass();
        break;
    case 3: // Calculate the amount of solvent
        calculateAmountSolvent();
        break;
    case 4: // Calculate the molar mass of solvent
        calculateMolarMassSolvent();
        break;
    case 5: // Calculate the concentration of the solution
        calculateConcentration();
        break;
    }
    return;
}

void concentrationCalculator::error(int mode)
{

    switch (mode) {
    case RESET_CONC_MESG:
        m_error->setText("");
        break;
    case PERCENTAGE_ZERO:
        m_error->setText(i18n("Percentage should be less than 100.0, please enter a valid value."));
        break;
    case DENSITY_ZERO:
        m_error->setText(i18n("Density cannot be zero, please enter a valid value."));
        break;
    case MASS_ZERO:
        m_error->setText(i18n("Mass cannot be zero, please enter a valid value."));
        break;
    case VOLUME_ZERO:
        m_error->setText(i18n("Volume cannot be zero, please enter a valid value."));
        break;
    case MOLES_ZERO:
        m_error->setText(i18n("Number of moles cannot be zero, please enter a valid value."));
        break;
    case MOLAR_SOLVENT_ZERO:
        m_error->setText(i18n("Molar mass of solvent is zero, please enter a valid value."));
        break;
    case EQTS_ZERO:
        m_error->setText(i18n("Number of equivalents is zero - cannot calculate equivalent mass."));
        break;
    case CONC_ZERO:
        m_error->setText(i18n("Concentration is zero, please enter a valid value."));
        break;
    case INSUFFICIENT_DATA_EQT:
        m_error->setText(i18n("Insufficient data to calculate the required value, please specify normality."));
        break;
    case INSUFFICIENT_DATA_MOLE:
        m_error->setText(i18n("Insufficient data, please specify molarity / mole fraction / molality to calculate."));
        break;
    case INSUFFICIENT_DATA_MOLES:
        m_error->setText(i18n("Amount is specified in moles, cannot calculate molar/equivalent masses. Please specify mass/volume."));
        break;
    case INSUFFICIENT_DATA_SOLVENT:
        m_error->setText(i18n("You can only calculate the molar mass of a solvent if the mole fraction is specified."));
        break;
    case MOLAR_MASS_ZERO:
        m_error->setText(i18n("Molar mass cannot be zero, please enter a valid value."));
        break;
    case EQT_MASS_ZERO:
        m_error->setText(i18n("Equivalent mass cannot be zero, please enter a valid value."));
        break;
    default:
        break;
    }
}

void concentrationCalculator::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget();
    ui.setupUi(widget);
    parent->addPage(widget, i18n("General"), icon());

    ui.soluteMass->setChecked(m_soluteMass);
    ui.solventVolume->setChecked(m_solventVolume);

    connect(parent->okButton, &QPushButton::clicked,
            this, &concentrationCalculator::configAccepted);
    connect(parent->buttonBox, &QDialogButtonBox::Apply::clicked,
                this, &concentrationCalculator::configAccepted);

    connect (ui.soluteMass, SIGNAL(toggled(bool)), parent, SLOT(settingsModified()));
    connect (ui.solventVolume, SIGNAL(toggled(bool)), parent, SLOT(settingsModified()));
}

void concentrationCalculator::configAccepted()
{
    KConfigGroup cg = config();
    QGraphicsItem::update();

    m_soluteMass = ui.soluteMass->isChecked();
    cg.writeEntry("soluteMass", m_soluteMass);

    m_solventVolume = ui.solventVolume->isChecked();
    cg.writeEntry("solventVolume", m_solventVolume);

    m_configUpdated = true;
    updateConstraints();

    emit configNeedsSaving();
}
#include "concentrationCalculator.moc"
