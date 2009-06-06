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
        
   	    m_amountSoluteUnit1 = new Plasma::ComboBox(this);
	    m_amountSoluteUnit1->setZValue(2);
	    m_amountSoluteUnit1->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams", 0)
         << tr2i18n("tons", 0)
         << tr2i18n("carats", 0)
         << tr2i18n("pounds", 0)
         << tr2i18n("ounces", 0)
         << tr2i18n("troy ounces", 0)
        );
        m_amountSoluteUnit1->setZValue(5);
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
	    pGridLayout->addItem(m_amountSoluteUnit1, 1, 4);
	    pGridLayout->addItem(m_amountSoluteUnit2, 1, 5);
	    // 2 molar mass solute
	    
	    m_r2 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *molarMassLabel = new Plasma::Label(this);
	    molarMassLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassLabel->setText(i18n("Molar mass of solute:"));
	    
	    m_molarMass = new Plasma::SpinBox(this);
        //m_molarMass->setDecimals(4);
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
        //m_amtSolvent->setDecimals(4);
        m_amountSolvent->setMaximum(1e+09);
        	    
	    m_amountSolventType = new Plasma::ComboBox(this);
	    m_amountSolventType->setZValue(2);
	    m_amountSolventType->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("volume", 0)
         << tr2i18n("Mass", 0)
         << tr2i18n("moles", 0)
        );
        m_amountSolventType->setZValue(3);
        
   	    m_amountSolventUnit1 = new Plasma::ComboBox(this);
	    m_amountSolventUnit1->setZValue(2);
	    m_amountSolventUnit1->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams", 0)
         << tr2i18n("tons", 0)
         << tr2i18n("carats", 0)
         << tr2i18n("pounds", 0)
         << tr2i18n("ounces", 0)
         << tr2i18n("troy ounces", 0)
        );
        m_amountSolventUnit1->setZValue(3);
        
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
	    pGridLayout->addItem(m_amountSolventUnit1, 5, 4);
	    pGridLayout->addItem(m_amountSolventUnit2, 5, 5);
	    
	    // 6 molar mass solvent
	    
	    m_r6 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *molarMassSolvtLabel = new Plasma::Label(this);
	    molarMassSolvtLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassSolvtLabel->setText(i18n("Molar mass of solvent:"));
	    
	    m_molarMassSolvent = new Plasma::SpinBox(this);
        //m_molarMassSolvent->setDecimals(4);
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
	}    
    return m_widget;
}

#include "concentrationCalculator.moc"
