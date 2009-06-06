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

gasCalculator::gasCalculator(QObject *parent, const QVariantList &args)
: Plasma::PopupApplet(parent, args)
, m_widget(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPopupIcon("accessories-calculator");
    setHasConfigurationInterface(true);
    resize(800, 600);
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
} 
 
 
QGraphicsWidget *gasCalculator::graphicsWidget()
{
//FIXME:
// 1.> Currently the spin boxes are integer, please convert them into double
// and uncomment certain lines of code which say 'setDecimals(4)'
// 2.> The radio buttons allow multiple selection which should not happen, they should be 
// grouped somehow.

	if (!m_widget) {	
		m_widget = new QGraphicsWidget(this);
		
		// setup the label
	    Plasma::Frame *pHeader = new Plasma::Frame(this);
	    pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	    pHeader->setText(i18n("gas Calculator"));
	    
	    //setup the grid layout
		QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(m_widget);
		pGridLayout->addItem(pHeader, 0, 0, 1, 5);
		
		// Set up the user interface
		// molar mass
		
		Plasma::Label *molarMassLabel = new Plasma::Label(this);
	    molarMassLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassLabel->setText(i18n("Molar Mass of Gas:"));
	    
	    m_molarMass = new Plasma::SpinBox(this);
		//m_molarMass->setDecimals(4);
        m_molarMass->setMaximum(1e+09);
        
		Plasma::Label *molarMassUnitLabel = new Plasma::Label(this);
	    molarMassUnitLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassUnitLabel->setText(i18n("u (mass)"));
	    
	    pGridLayout->addItem(molarMassLabel, 1, 1);
	    pGridLayout->addItem(m_molarMass, 1, 2);
	    pGridLayout->addItem(molarMassUnitLabel, 1, 3);

	    // moles
	    
	    m_r2 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *molesLabel = new Plasma::Label(this);
	    molesLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molesLabel->setText(i18n("Number of moles:"));
	    
	    m_moles = new Plasma::SpinBox(this);
        //m_moles->setDecimals(4);
        m_moles->setMaximum(1e+09);
        	    
	    pGridLayout->addItem(m_r2, 2, 0);
	    pGridLayout->addItem(molesLabel, 2, 1);
	    pGridLayout->addItem(m_moles, 2, 2);
	    
	    // mass
	    	    
	    Plasma::Label *massLabel = new Plasma::Label(this);
	    massLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    massLabel->setText(i18n("Mass of the gas:"));
	    
	    m_mass = new Plasma::SpinBox(this);
	    //m_mass->setDecimals(4);
	    m_mass->setMaximum(1e+09);
	    
  	    m_massUnit = new Plasma::ComboBox(this);
	    m_massUnit->setZValue(2);
		m_massUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams", 0)
         << tr2i18n("tons", 0)
         << tr2i18n("carats", 0)
         << tr2i18n("pounds", 0)
         << tr2i18n("ounces", 0)
         << tr2i18n("troy ounces", 0)
        );
        m_massUnit->setZValue(6);
        
		pGridLayout->addItem(massLabel, 3, 1);
	    pGridLayout->addItem(m_mass, 3, 2);
	    pGridLayout->addItem(m_massUnit, 3, 3);

		// pressure
		
		m_r4 = new Plasma::RadioButton(this);
				
		Plasma::Label *pressureLabel = new Plasma::Label(this);
	    pressureLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    pressureLabel->setText(i18n("Pressure of the Gas:"));
	    
	    m_pressure = new Plasma::SpinBox(this);
        //m_pressure->setDecimals(4);
        m_pressure->setMaximum(1e+09);
        
  	    m_pressureUnit = new Plasma::ComboBox(this);
	    m_pressureUnit->setZValue(2);
		m_pressureUnit->nativeWidget()->insertItems(0, QStringList()
		 << tr2i18n("atmospheres", 0)
         << tr2i18n("pascal", 0)
         << tr2i18n("bars", 0)
         << tr2i18n("millibars", 0)
         << tr2i18n("decibars", 0)
         << tr2i18n("torrs", 0)
         << tr2i18n("inches of mercury", 0)
        );
	    m_pressureUnit->setZValue(5);
	    
	    pGridLayout->addItem(m_r4, 4, 0);
	    pGridLayout->addItem(pressureLabel, 4, 1);
	    pGridLayout->addItem(m_pressure, 4, 2);
	    pGridLayout->addItem(m_pressureUnit, 4, 3);
	    
		// temperature
		
		m_r5 = new Plasma::RadioButton(this);
		
		Plasma::Label *temperatureLabel = new Plasma::Label(this);
	    temperatureLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    temperatureLabel->setText(i18n("Temperature:"));
	    
	    m_temperature = new Plasma::SpinBox(this);
	    //m_temp->setDecimals(4);
        m_temperature->setMaximum(1e+09);
        
	    m_temperatureUnit = new Plasma::ComboBox(this);
	    m_temperatureUnit->setZValue(2);
	    m_temperatureUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("kelvins", 0)
         << tr2i18n("celsius", 0)
         << tr2i18n("fahrenheit", 0)
         << tr2i18n("delisles", 0)
         << tr2i18n("r\303\251aumurs", 0)
        );
        m_temperatureUnit->setZValue(4);
        
   	    pGridLayout->addItem(m_r5, 5, 0);
	    pGridLayout->addItem(temperatureLabel, 5, 1);
	    pGridLayout->addItem(m_temperature, 5, 2);
	    pGridLayout->addItem(m_temperatureUnit, 5, 3);
	    
	    // volume
	    
	    m_r6 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *volumeLabel = new Plasma::Label(this);
	    volumeLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    volumeLabel->setText(i18n("Volume of the gas:"));
	    
	    m_volume = new Plasma::SpinBox(this);
        //m_volume->setDecimals(4);
        m_volume->setMaximum(1e+09);
        	    
	    m_volumeUnit = new Plasma::ComboBox(this);
	    m_volumeUnit->setZValue(2);
	    m_volumeUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("liter", 0)
         << tr2i18n("cubic feet", 0)
         << tr2i18n("cubic inch", 0)
         << tr2i18n("fluid ounce", 0)
         << tr2i18n("cups", 0)
         << tr2i18n("gallons", 0)
         << tr2i18n("pints", 0)
        );
        m_volumeUnit->setZValue(3);
        
	    pGridLayout->addItem(m_r6, 6, 0);
	    pGridLayout->addItem(volumeLabel, 6, 1);
	    pGridLayout->addItem(m_volume, 6, 2);
	    pGridLayout->addItem(m_volumeUnit, 6, 3);
	    
	    // vander waal's constant a
	    
	    Plasma::Label *vand_aLabel = new Plasma::Label(this);
	    vand_aLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    vand_aLabel->setText(i18n("Van der Waals constant 'a':"));
	    
	    m_Vand_a = new Plasma::SpinBox(this);
        //m_Vand_a->setDecimals(4);
        m_Vand_a->setMaximum(1e+09);
        
  	    m_aUnit = new Plasma::ComboBox(this);
	    m_aUnit->setZValue(2);
	    m_aUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("liter^2 atmosphere/mol^2", 0)
        );
        m_aUnit->setZValue(2);
        
   	    pGridLayout->addItem(vand_aLabel, 7, 1);
	    pGridLayout->addItem(m_Vand_a, 7, 2);
	    pGridLayout->addItem(m_aUnit, 7, 3);
	    
	    // vander waal's constant b
	    
	    Plasma::Label *vand_bLabel = new Plasma::Label(this);
	    vand_bLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    vand_bLabel->setText(i18n("Van der Waals constant 'b':"));
	    
	    m_Vand_b = new Plasma::SpinBox(this);
        //m_Vand_b->setDecimals(4);
        m_Vand_b->setMaximum(1e+09);
        
  	    m_bUnit = new Plasma::ComboBox(this);
	    m_bUnit->setZValue(2);
	    m_bUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("liters", 0)
         << tr2i18n("cubic meters", 0)
         << tr2i18n("cubic feet", 0)
         << tr2i18n("cubic inches", 0)
         << tr2i18n("gallons", 0)
         << tr2i18n("cups", 0)
        );
        m_bUnit->setZValue(1);
        
   	    pGridLayout->addItem(vand_bLabel, 8, 1);
	    pGridLayout->addItem(m_Vand_b, 8, 2);
	    pGridLayout->addItem(m_bUnit, 8, 3);
	}    
    return m_widget;
}

#include "gasCalculator.moc"
