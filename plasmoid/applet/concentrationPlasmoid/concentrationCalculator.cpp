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
#include <QPainter>
#include <QFontMetrics>
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <KTextEdit>
#include <QSlider>
#include <QSpinBox>
#include <KComboBox>
#include <QSizeF>
#include <QLabel>
#include <Plasma/ComboBox>
#include <Plasma/LineEdit>
#include <Plasma/Label>
#include <Plasma/Frame>
#include <Plasma/GroupBox>
#include <Plasma/RadioButton>
#include <Plasma/SpinBox>
#include <Plasma/Slider>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
 
#include <plasma/svg.h>
#include <plasma/theme.h>

using namespace Conversion;

concentrationCalculator::concentrationCalculator(QObject *parent, const QVariantList &args)
: Plasma::PopupApplet(parent, args)
, m_widget(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPopupIcon("accessories-calculator");
    setHasConfigurationInterface(true);
    resize(800, 600);
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
} 
 
 
QGraphicsWidget *concentrationCalculator::graphicsWidget()
{
//FIXME:
// 1.> Also currently the spin boxes are integer, please convert them into double
// and uncomment certain lines of code which say 'setDecimals(4)'
// 2.> The radio buttons allow multiple selection which should not happen, they should be 
// grouped somehow.

	if (!m_widget) {	
		m_widget = new QGraphicsWidget(this);
		
		// setup the label
	    Plasma::Frame *pHeader = new Plasma::Frame(this);
	    pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	    pHeader->setText(i18n("concentration Calculator"));
	    
	    //setup the layout
	    QGraphicsLinearLayout *pVLayout = new QGraphicsLinearLayout(Qt::Vertical,m_widget);
	    Plasma::GroupBox *pGroupBox1 = new Plasma::GroupBox(this);
		QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(pGroupBox1);
		pGridLayout->addItem(pHeader, 0, 0, 1, 5);
		pVLayout->addItem(pGroupBox1);
		
		// Set up the user interface
		// 1 amount solute
		m_r1 = new Plasma::RadioButton(this);
		
		Plasma::Label *amtSltLabel = new Plasma::Label(this);
	    amtSltLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    amtSltLabel->setText(i18n("Amount of solute:"));
	    
	    m_amountSolute = new Plasma::SpinBox(this);
	    //amtSolute->setDecimals(4);
        m_amountSolute->setMaximum(1e+09);
        	    
	    m_amountSoluteType = new Plasma::ComboBox(this);
	    m_amountSoluteType->setZValue(2);
        m_amountSoluteType->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("Mass", 0)
         << tr2i18n("volume", 0)
         << tr2i18n("moles", 0)
        );
        m_amountSoluteType->setZValue(5);
        
   	    m_amountSoluteUnit = new Plasma::ComboBox(this);
	    m_amountSoluteUnit->setZValue(2);
	    m_amountSoluteUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams", 0)
         << tr2i18n("tons", 0)
         << tr2i18n("carats", 0)
         << tr2i18n("pounds", 0)
         << tr2i18n("ounces", 0)
         << tr2i18n("troy ounces", 0)
        );
        m_amountSoluteUnit->setZValue(5);
   	    m_amountSoluteUnit2 = new Plasma::ComboBox(this);
	    m_amountSoluteUnit2->setZValue(2);
	    m_amountSoluteUnit2->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("liter", 0)
         << tr2i18n("cubic meters", 0)
         << tr2i18n("cubic feet", 0)
         << tr2i18n("cubic inch", 0)
         << tr2i18n("cubic mile", 0)
         << tr2i18n("fluid ounce", 0)
         << tr2i18n("cups", 0)
         << tr2i18n("gallons", 0)
         << tr2i18n("pints", 0)
        );
        m_amountSoluteUnit2->setZValue(5);
        
	    pGridLayout->addItem(m_r1, 1, 0);
	    pGridLayout->addItem(amtSltLabel, 1, 1);
	    pGridLayout->addItem(m_amountSolute, 1, 2);
	    pGridLayout->addItem(m_amountSoluteType, 1, 3);
	    pGridLayout->addItem(m_amountSoluteUnit, 1, 4);
	    pGridLayout->addItem(m_amountSoluteUnit2, 1, 5);
	    // 2 molar mass solute
	    
	    m_r2 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *molarMassLabel = new Plasma::Label(this);
	    molarMassLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassLabel->setText(i18n("Molar mass of solute:"));
	    
	    m_molarMass = new Plasma::SpinBox(this);
        //m_MolarMass->setDecimals(4);
        m_molarMass->setMaximum(1e+09);
	    
	    Plasma::Label *molarMassUnit = new Plasma::Label(this);
	    molarMassUnit->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassUnit->setText(i18n("u (mass)"));
	    
	    pGridLayout->addItem(m_r2, 2, 0);
	    pGridLayout->addItem(molarMassLabel, 2, 1);
	    pGridLayout->addItem(m_molarMass, 2, 2);
	    pGridLayout->addItem(molarMassUnit, 2, 4);
	    
	    // 3 equivalent mass solute
	    
	    m_r3 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *eqtMassLabel = new Plasma::Label(this);
	    eqtMassLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    eqtMassLabel->setText(i18n("Equivalent mass of solute:"));
	    
	    m_eqtMass = new Plasma::SpinBox(this);
	    
	    //m_eqtMass->setDecimals(4);
        m_eqtMass->setMaximum(1e+09);
        
	    Plasma::Label *eqtMassUnit = new Plasma::Label(this);
	    eqtMassUnit->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    eqtMassUnit->setText(i18n("u (mass)"));
		
		pGridLayout->addItem(m_r3, 3, 0);
		pGridLayout->addItem(eqtMassLabel, 3, 1);
	    pGridLayout->addItem(m_eqtMass, 3, 2);
	    pGridLayout->addItem(eqtMassUnit, 3, 4);

		// 4 density solute
				
		Plasma::Label *densitySoluteLabel = new Plasma::Label(this);
	    densitySoluteLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    densitySoluteLabel->setText(i18n("Density of solute:"));
	    
	    m_densitySolute = new Plasma::SpinBox(this);
        //m_densitySolute->setDecimals(4);
        m_densitySolute->setMaximum(1e+09);	    
        
  	    m_densitySoluteUnit = new Plasma::ComboBox(this);
	    m_densitySoluteUnit->setZValue(2);
	    m_densitySoluteUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams per liter", 0)
         << tr2i18n("grams per milliliter", 0)
         << tr2i18n("kilograms per cubic meter", 0)
         << tr2i18n("kilograms per liter", 0)
         << tr2i18n("ounces per cubic inch", 0)
         << tr2i18n("ounces per cubic foot", 0)
         << tr2i18n("pounds per cubic inch", 0)
         << tr2i18n("pounds per cubic foot", 0)
         << tr2i18n("pounds per cubic yard", 0)
        );
	    m_densitySoluteUnit->setZValue(4);
	    pGridLayout->addItem(densitySoluteLabel, 4, 1);
	    pGridLayout->addItem(m_densitySolute, 4, 2);
	    pGridLayout->addItem(m_densitySoluteUnit, 4, 4);
	    
		// 5 amount solvent
		
		m_r5 = new Plasma::RadioButton(this);
		
		Plasma::Label *amtSlvtLabel = new Plasma::Label(this);
	    amtSlvtLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    amtSlvtLabel->setText(i18n("Amount of solvent:"));
	    
	    m_amountSolvent = new Plasma::SpinBox(this);
        //m_amountSolvent->setDecimals(4);
        m_amountSolvent->setMaximum(1e+09);
        	    
	    m_amountSolventType = new Plasma::ComboBox(this);
	    m_amountSolventType->setZValue(2);
	    m_amountSolventType->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("volume", 0)
         << tr2i18n("Mass", 0)
         << tr2i18n("moles", 0)
        );
        m_amountSolventType->setZValue(3);
        
   	    m_amountSolventUnit = new Plasma::ComboBox(this);
	    m_amountSolventUnit->setZValue(2);
	    m_amountSolventUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams", 0)
         << tr2i18n("tons", 0)
         << tr2i18n("carats", 0)
         << tr2i18n("pounds", 0)
         << tr2i18n("ounces", 0)
         << tr2i18n("troy ounces", 0)
        );
        m_amountSolventUnit->setZValue(3);
        
   	    m_amountSolventUnit2 = new Plasma::ComboBox(this);
	    m_amountSolventUnit2->setZValue(2);
	    m_amountSolventUnit2->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("liter", 0)
         << tr2i18n("cubic meters", 0)
         << tr2i18n("cubic feet", 0)
         << tr2i18n("cubic inch", 0)
         << tr2i18n("cubic mile", 0)
         << tr2i18n("fluid ounce", 0)
         << tr2i18n("cups", 0)
         << tr2i18n("gallons", 0)
         << tr2i18n("pints", 0)
        );
        m_amountSolventUnit2->setZValue(3);
        
   	    pGridLayout->addItem(m_r5, 5, 0);
	    pGridLayout->addItem(amtSlvtLabel, 5, 1);
	    pGridLayout->addItem(m_amountSolvent, 5, 2);
	    pGridLayout->addItem(m_amountSolventType, 5, 3);
	    pGridLayout->addItem(m_amountSolventUnit, 5, 4);
	    pGridLayout->addItem(m_amountSolventUnit2, 5, 5);
	    
	    // 6 molar mass solvent
	    
	    m_r6 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *molarMassSolvtLabel = new Plasma::Label(this);
	    molarMassSolvtLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassSolvtLabel->setText(i18n("Molar mass of solvent:"));
	    
	    m_molarMassSolvent = new Plasma::SpinBox(this);
        //m_MolarMassSolvent->setDecimals(4);
        m_molarMassSolvent->setMaximum(1e+09);
        	    
	    Plasma::Label *molarMassSolvtUnit = new Plasma::Label(this);
	    molarMassSolvtUnit->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassSolvtUnit->setText(i18n("u (mass)"));
	    
	    pGridLayout->addItem(m_r6, 6, 0);
	    pGridLayout->addItem(molarMassSolvtLabel, 6, 1);
	    pGridLayout->addItem(m_molarMassSolvent, 6, 2);
	    pGridLayout->addItem(molarMassSolvtUnit, 6, 4);
	    
	    // 7 density of solvent
	    
	    Plasma::Label *densitySolventLabel = new Plasma::Label(this);
	    densitySolventLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    densitySolventLabel->setText(i18n("Density of solvent:"));
	    
	    m_densitySolvent = new Plasma::SpinBox(this);
        //m_densitySolvent->setDecimals(4);
        m_densitySolvent->setMaximum(1e+09);
        	    
  	    m_densitySolventUnit = new Plasma::ComboBox(this);
	    m_densitySolventUnit->setZValue(2);
	    m_densitySolventUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams per liter", 0)
         << tr2i18n("grams per milliliter", 0)
         << tr2i18n("kilograms per cubic meter", 0)
         << tr2i18n("kilograms per liter", 0)
         << tr2i18n("ounces per cubic inch", 0)
         << tr2i18n("ounces per cubic foot", 0)
         << tr2i18n("pounds per cubic inch", 0)
         << tr2i18n("pounds per cubic foot", 0)
         << tr2i18n("pounds per cubic yard", 0)
        );
        m_densitySolventUnit->setZValue(2);
        
   	    pGridLayout->addItem(densitySolventLabel, 7, 1);
	    pGridLayout->addItem(m_densitySolvent, 7, 2);
	    pGridLayout->addItem(m_densitySolventUnit, 7, 4);
	    
	    // Concentration
	    
	    m_r8 = new Plasma::RadioButton(this);
	    
		Plasma::Label *concentrationLabel = new Plasma::Label(this);
	    concentrationLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    concentrationLabel->setText(i18n("Molar mass of solvent:"));

		m_concentration = new Plasma::SpinBox(this);
		//m_concentration->setDecimals(4);
		m_concentration->setMaximum(1e+09);
		
		m_concentrationUnit = new Plasma::ComboBox(this);
	    m_concentrationUnit->setZValue(2);
	    m_concentrationUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("molar", 0)
         << tr2i18n("Normal", 0)
         << tr2i18n("molal", 0)
         << tr2i18n("% ( mass )", 0)
         << tr2i18n("% ( volume )", 0)
         << tr2i18n("% ( moles )", 0)
        );
        m_concentrationUnit->setZValue(1);
        
	    pGridLayout->addItem(m_r8, 8, 0);
	    pGridLayout->addItem(concentrationLabel, 8, 1);
	    pGridLayout->addItem(m_concentration, 8, 2);
	    pGridLayout->addItem(m_concentrationUnit, 8, 4);
	    
	    // Results
	    m_error = new Plasma::Label(this);
	    pGridLayout->addItem(m_error, 9, 0, 5, 2);
	    // Done adding elements to the UI, now initialise
	    
	    /**************************************************************************/
	    //                       concentration Calculator set up
	    /**************************************************************************/

	    // initialise the initially selected values
	    m_amountSolute            -> setValue(117.0);
	    m_molarMass            -> setValue(58.5);
	    m_eqtMass              -> setValue(58.5);
	    m_densitySolute        -> setValue(2.7);
	    m_amountSolvent           -> setValue(1.0);
	    m_molarMassSolvent     -> setValue(18.0);
	    m_densitySolvent       -> setValue(1000.0);
	    m_concentration        -> setValue(2.0);
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
	    // Initialisation of values done
	    // Connect signals with slots ( when a change of selection in the UI takes place,
	    // corresponding quantity should be updated in the class. )
	    connect(m_amountSolute, SIGNAL(valueChanged(int)),
	            this, SLOT(amountSoluteChanged()));
	    connect(m_amountSoluteType->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(amountSoluteChanged()));
	    connect(m_amountSoluteUnit->nativeWidget() , SIGNAL(activated(int)),
	            this, SLOT(amountSoluteChanged()));
	    connect(m_amountSoluteUnit2->nativeWidget(), SIGNAL(activated(int)),
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
	            this, SLOT(amountSolventChanged()));
	    connect(m_amountSolventUnit->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(amountSolventChanged()));
	    connect(m_amountSolventUnit->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(amountSolventChanged()));
	    connect(m_amountSolventUnit2->nativeWidget(), SIGNAL(activated(int)),
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

	    m_amountSoluteUnit2-> hide();           // Mass is the default mode of specification of amount of solvent
	    m_amountSolventUnit-> hide();           // volume is the default for solvent
	    /**************************************************************************/
	    //              concentration Calculator setup complete
	    /**************************************************************************/
	}    
    return m_widget;
}

// Calculates the amount of solute
void concentrationCalculator::calculateAmountSolute(void)
{
    int type1 = m_concentrationUnit->nativeWidget()-> currentIndex();
    int type2 = m_amountSoluteType->nativeWidget()-> currentIndex();

    double molesSolute , eqtsSolute, massSolute, volSolute;     // variables
    int mode = 0;
    /*
     * mode = 1 ( molesSolute) mode = 2 eqtsSolute, mode = 3 mass, 4 volume
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
            error(PERCENTAGE);
        }
        massSolute = m_Concentration / (100.0 - m_Concentration) * massSolvent();
        mode = 3;
        break;
        // Calculate the volume of solute
    case 4: // volume percentage specified
        if (m_Concentration >= 100.0) {
            error(PERCENTAGE);
        }
        volSolute = m_Concentration / (100.0 - m_Concentration) * volumeSolvent();
        mode = 4;
        break;
        // Calculate the moles of solute
    case 5: //mole percentage specified
        if (m_Concentration >= 100.0) {
            error(PERCENTAGE);
        }
        molesSolute = m_Concentration / (100.0 - m_Concentration) * molesSolvent();
        mode = 1;
        break;
    default:
        break;
    }

    // We have the amount of solvent in some form ( moles, equivalents, mass, volume etc )
    // Now we have to present it in the UI
    switch (type2) {
    case 0: // amount should be specified interms of mass
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
        m_AmtSolute = (Converter::self() -> convert(m_AmtSolute , \
                       m_amountSoluteUnit->nativeWidget()->currentText()));
        m_amountSolute -> setValue(m_AmtSolute.number());
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
        m_AmtSolute = (Converter::self() -> convert(m_AmtSolute , \
                       m_amountSoluteUnit2->nativeWidget()->currentText()));
        m_amountSolute -> setValue(m_AmtSolute.number());
        break;

    case 2: // amount should be specified in terms of moles
        switch (mode) {
        case 1: // we already know the moles of solute
            break;
        case 2: // we should obtain moles from equivalents ( not possible )
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
        m_amountSolute -> setValue(molesSolute);
        break;
    }
    return;
}

// Calculates the molar mass
void concentrationCalculator::calculateMolarMass(void)
{
    // molarity / molality / mole fraction required
    int type = m_concentrationUnit->nativeWidget() ->currentIndex();
    int type2 = m_amountSolventType->nativeWidget() ->currentIndex();
    double numMoles;
    switch (type) {
    case 0:     //molarity specified
        // number of moles = volume * concentration
        numMoles = volumeSolvent() * m_Concentration;
        break;
    case 1:     // cannot be calculated ( insufficient data )
        error(INSUFFICIENT_DATA_MOLE);
        return;
        break;
    case 2:     // molality specified
        numMoles = massSolvent() / 1000.0 * m_Concentration;
        break;
    case 3:     // cannot be calculated ( insufficient data )
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
void concentrationCalculator::calculateEqtMass(void)
{
    // Normality required
    int type = m_concentrationUnit->nativeWidget() ->currentIndex();
    int type2 = m_amountSoluteType->nativeWidget() ->currentIndex();

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
void concentrationCalculator::calculateMolarMassSolvent(void)
{
    // molarity / molality / mole fraction required
    int type = m_concentrationUnit->nativeWidget() ->currentIndex();
    int type2 = m_amountSolventType->nativeWidget() ->currentIndex();
    double numMoles;
    switch (type) {
    case 0:         // molarity specified
    case 1:         // normality specified
    case 2:         // molality specified
    case 3:         // mass fraction specified
    case 4:         // volume fraction specified
        error(INSUFFICIENT_DATA_SOLVENT);
        return;
        break;      // cannot be calculated ( insufficient data )
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
void concentrationCalculator::calculateAmountSolvent(void)
{
    int type1 = m_concentrationUnit->nativeWidget()-> currentIndex();
    int type2 = m_amountSolventType->nativeWidget()-> currentIndex();

    double moleSolvent , massSolvent, volSolvent;

    int mode = 0;               // Indicates the mode in which we have calculated the amount of solvent
    /*
     * mode = 1 ( molessolvent) mode = 2 eqtssolvent, mode = 3 mass, 4 volume
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

    // We have the amount of solvent in some form ( moles, equivalents, mass, volume etc )
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
        m_AmtSolvent = (Converter::self()->convert(m_AmtSolvent , \
                        m_amountSolventUnit2->nativeWidget()->currentText()));
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
        m_AmtSolvent = (Converter::self()-> convert(m_AmtSolvent , \
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
void concentrationCalculator::calculateConcentration(void)
{
    int type = m_concentrationUnit->nativeWidget()-> currentIndex();

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
    int type = m_amountSolventType->nativeWidget()-> currentIndex();
    double volume;
    switch (type) {
    case 0:
        volume = (Converter::self() -> convert(m_AmtSolvent , "liter")).number();
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
    int type = m_amountSolventType->nativeWidget()-> currentIndex();

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
    int type = m_amountSolventType->nativeWidget()-> currentIndex();
    double mass;
    switch (type) {
    case 0:
        mass = volumeSolvent() * densitySolvent();
        break;
    case 1:
        mass = (Converter::self() -> convert(m_AmtSolvent, "gram")) .number();
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
    return ((Converter::self() -> convert(m_DensitySolvent, "grams per liter")).number());
}

double concentrationCalculator::volumeSolute()
{
    int type = m_amountSoluteType -> nativeWidget()->currentIndex();
    double volume;
    switch (type) {
    case 0:
        volume = massSolute() / densitySolute();
        break;
    case 1:
        volume = (Converter::self() -> convert(m_AmtSolute , "liter")).number();
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
    int type = m_amountSoluteType -> nativeWidget()->currentIndex();

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
    int type = m_amountSoluteType -> nativeWidget()->currentIndex();
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
    int type = m_amountSoluteType -> nativeWidget()->currentIndex();
    double mass;
    switch (type) {
    case 0:
        mass = (Converter::self() -> convert(m_AmtSolute, "gram")) .number();
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
    return ((Converter::self() -> convert(m_DensitySolute, "grams per liter")).number());
}


// occurs when the amount of solute is changed
void concentrationCalculator::amountSoluteChanged(void)
{
    int type = m_amountSoluteType -> nativeWidget()->currentIndex();
    if (type == 0) {         // amount of solute specified in terms of mass
        m_amountSoluteUnit -> show();
        m_amountSoluteUnit2-> hide();
        m_AmtSolute = Value(m_amountSolute -> value(), m_amountSoluteUnit -> nativeWidget()->currentText());
    } else if (type == 1) { // amount of solute is specified in terms of volume
        m_amountSoluteUnit -> hide();
        m_amountSoluteUnit2-> show();
        m_AmtSolute = Value(m_amountSolute -> value(), m_amountSoluteUnit2 -> nativeWidget()->currentText());
    } else {                 // amount of solute is specified in terms of moles
        m_MolesSolute = m_amountSolute -> value();
        m_amountSoluteUnit -> hide();
        m_amountSoluteUnit2-> hide();
    }
    calculate();
}

// occurs when the amount of solvent is changed
void concentrationCalculator::amountSolventChanged(void)
{
    int type = m_amountSolventType -> nativeWidget()->currentIndex();
    if (type == 0) {     // amount of solvent specified in terms of volume
        m_amountSolventUnit -> hide();
        m_amountSolventUnit2-> show();
        m_AmtSolvent = Value(m_amountSolvent -> value(), m_amountSolventUnit2 -> nativeWidget()->currentText());
    } else if (type == 1) { // amount of solvent is specified in terms of mass
        m_amountSolventUnit -> show();
        m_amountSolventUnit2-> hide();
        m_AmtSolvent = Value(m_amountSolvent -> value(), m_amountSolventUnit -> nativeWidget()->currentText());
    } else {
        m_amountSolventUnit -> hide();
        m_amountSolventUnit2-> hide();
        m_MolesSolvent = m_amountSolvent -> value();
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
void concentrationCalculator::densitySoluteChanged(void)
{
    m_DensitySolute = Value(m_densitySolute -> value(), m_densitySoluteUnit -> nativeWidget()->currentText());
    calculate();
}

// occurs when the density of solvent is changed
void concentrationCalculator::densitySolventChanged(void)
{
    m_DensitySolvent = Value(m_densitySolvent -> value(), m_densitySolventUnit -> nativeWidget()->currentText());
    calculate();
}

// occurs when the concentration is changed
void concentrationCalculator::concentrationChanged(int value)
{
    m_Concentration = value;
    calculate();
}
// occurs when any quantity is changed
void concentrationCalculator::calculate(void)
{
    // Calculate the amount of solute
    if (m_r1 -> isChecked()) {
        if (m_concentrationUnit -> nativeWidget()->currentIndex() > 2 && m_concentration -> value() > 100)
            return;
        calculateAmountSolute();
    }
    // Calculate the molar mass of solute
    else if (m_r2 -> isChecked()) {
        calculateMolarMass();
    }
    // Calculate the equivalent mass of solute
    else if (m_r3 -> isChecked()) {
        calculateEqtMass();
    }
    // Calculate the amount of solvent
    else if (m_r5 -> isChecked()) {
        calculateAmountSolvent();
    }
    // Calculate the molar mass of solvent
    else if (m_r6 -> isChecked()) {
        calculateMolarMassSolvent();
    }
    // Calculate the concentration of the solution
    else if (m_r8 -> isChecked()) {
        calculateConcentration();
    }
    return;
}

void concentrationCalculator::debug(void)
{

}
void concentrationCalculator::error(int mode)
{

    switch (mode) {
    case PERCENTAGE:
        m_error->setText("Percentage should be less than 100.0, please enter valid data!");
        break;
    case DENSITY_ZERO:
        m_error->setText("Density cannot be zero. Invalid! Please enter a valid value!");
        break;
    case MASS_ZERO:
        m_error->setText("mass cannot be zero! Please enter valid data to correct it!");
        break;
    case VOLUME_ZERO:
        m_error->setText("volume cannot be zero! Please enter valid data to correct it!");
        break;
    case MOLES_ZERO:
        m_error->setText("Number of moles cannot be zero! Please enter valid data to correct the error!");
        break;
    case MOLAR_SOLVENT_ZERO:
        m_error->setText("Molar mass of solvent is zero! Please correct it!");
        break;
    case EQTS_ZERO:
        m_error->setText("Number of equivalents is zero. Cannot calculate equivalent mass!");
        break;
    case CONC_ZERO:
        m_error->setText("Concentration is zero! Please correct it!");
        break;
    case INSUFFICIENT_DATA_EQT:
        m_error->setText("Insufficient data! to calculate the required, please specify normality!");
        break;

    case INSUFFICIENT_DATA_MOLE:
        m_error->setText("Insufficient data, specify molarity / molefraction / molality to calculate!");
        break;
    case INSUFFICIENT_DATA_MOLES:
        m_error->setText("amount specified in moles, cannot calculate molar/equivalent masses! Specify mass/volume");
        break;
    case INSUFFICIENT_DATA_SOLVENT:
        m_error->setText("You can calculate the molar mass of solvent only if mole fraction is specified!");
        break;
    case MOLAR_MASS_ZERO:
        m_error->setText("molar mass cannot be zero, please correct it!");
        break;
    case EQT_MASS_ZERO:
        m_error->setText("Equivalent mass cannot be zero, please correct it!");
        break;
    default:
        break;
    }
}
#include "concentrationCalculator.moc"
