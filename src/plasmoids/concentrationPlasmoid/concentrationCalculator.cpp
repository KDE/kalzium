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
#include <Plasma/RadioButton>
#include <Plasma/SpinBox>
#include <Plasma/Slider>
#include <QGraphicsGridLayout>
 
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
	if (!m_widget) {	
		m_widget = new QGraphicsWidget(this);
		
		// setup the label
	    Plasma::Frame *pHeader = new Plasma::Frame(this);
	    pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	    pHeader->setText(i18n("concentration Calculator"));
	    
	    //setup the grid layout
		QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(m_widget);
		pGridLayout->addItem(pHeader, 0, 0, 1, 5);
		
		// Set up the user interface
		// 1 amount solute
		m_r1 = new Plasma::RadioButton(this);
		
		Plasma::Label *amtSltLabel = new Plasma::Label(this);
	    amtSltLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    amtSltLabel->setText(i18n("Amount of solute:"));
	    
	    m_amountSolute = new Plasma::SpinBox(this);
	    
	    m_amountSoluteType = new Plasma::ComboBox(this);
	    m_amountSoluteType->setZValue(2);
	    
   	    m_amountSoluteUnit1 = new Plasma::ComboBox(this);
	    m_amountSoluteUnit1->setZValue(2);
	    
   	    m_amountSoluteUnit2 = new Plasma::ComboBox(this);
	    m_amountSoluteUnit2->setZValue(2);
	    
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
	    
  	    m_densitySoluteUnit = new Plasma::ComboBox(this);
	    m_densitySoluteUnit->setZValue(2);
	    
	    pGridLayout->addItem(densitySoluteLabel, 4, 1);
	    pGridLayout->addItem(m_densitySolute, 4, 2);
	    pGridLayout->addItem(m_densitySoluteUnit, 4, 4);
	    
		// 5 amount solvent
		
		m_r5 = new Plasma::RadioButton(this);
		
		Plasma::Label *amtSlvtLabel = new Plasma::Label(this);
	    amtSlvtLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    amtSlvtLabel->setText(i18n("Amount of solvent:"));
	    
	    m_amountSolvent = new Plasma::SpinBox(this);
	    
	    m_amountSolventType = new Plasma::ComboBox(this);
	    m_amountSolventType->setZValue(2);
	    
   	    m_amountSolventUnit1 = new Plasma::ComboBox(this);
	    m_amountSolventUnit1->setZValue(2);
	    
   	    m_amountSolventUnit2 = new Plasma::ComboBox(this);
	    m_amountSolventUnit2->setZValue(2);
	    
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
	    
  	    m_densitySolventUnit = new Plasma::ComboBox(this);
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
		
		m_concentrationUnit = new Plasma::ComboBox(this);
	    m_concentrationUnit->setZValue(2);
	    
	    pGridLayout->addItem(m_r8, 8, 0);
	    pGridLayout->addItem(concentrationLabel, 8, 1);
	    pGridLayout->addItem(m_concentration, 8, 2);
	    pGridLayout->addItem(m_concentrationUnit, 8, 4);
	}    
    return m_widget;
}

#include "concentrationCalculator.moc"
