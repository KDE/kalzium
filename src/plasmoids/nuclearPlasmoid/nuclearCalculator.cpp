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

nuclearCalculator::nuclearCalculator(QObject *parent, const QVariantList &args)
: Plasma::PopupApplet(parent, args)
, m_widget(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPopupIcon("accessories-calculator");
    setHasConfigurationInterface(true);
    resize(800, 600);
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
} 
 
 
QGraphicsWidget *nuclearCalculator::graphicsWidget()
{
	if (!m_widget) {	
		m_widget = new QGraphicsWidget(this);
	    Plasma::Frame *pHeader = new Plasma::Frame(this);
	    pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	    pHeader->setText(i18n("Nuclear Calculator"));

		// here comes the element - isotope and halfLife info part
	    Plasma::Label *eleLabel = new Plasma::Label(this);
	    eleLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    eleLabel->setText(i18n("Element name:"));
	    Plasma::Label *isoLabel = new Plasma::Label(this);
	    isoLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    isoLabel->setText(i18n("Isotope mass:"));
	    Plasma::Label *hLifeLabel = new Plasma::Label(this);
	    hLifeLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    hLifeLabel->setText(i18n("Half-Life"));
	    
	    m_element = new Plasma::ComboBox(this);
	    m_element->setZValue(2);
	    m_isotope = new Plasma::ComboBox(this);
	    m_element->setZValue(2);
	    m_halfLife = new Plasma::SpinBox(this);

		m_halfLifeUnit = new Plasma::ComboBox(this);
		m_halfLifeUnit->setZValue(2);
		
	    QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(m_widget);
	    pGridLayout->addItem(pHeader, 0, 0, 1, 5);
	    pGridLayout->addItem(eleLabel, 1, 1);
	    pGridLayout->addItem(m_element, 1, 2);
	    pGridLayout->addItem(isoLabel, 2, 1);
	    pGridLayout->addItem(m_isotope, 2, 2);
	    pGridLayout->addItem(hLifeLabel, 3, 1);
	    pGridLayout->addItem(m_halfLifeUnit, 3, 3);
	    pGridLayout->addItem(m_halfLife, 3, 2);
	    
	    // Here comes the amount and time part
	    m_r1 = new Plasma::RadioButton(this);
		m_r2 = new Plasma::RadioButton(this);
		m_r3 = new Plasma::RadioButton(this);
				
	    Plasma::Label *initLabel = new Plasma::Label(this);
	    initLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    initLabel->setText(i18n("Initial amount:"));
	    Plasma::Label *finalLabel = new Plasma::Label(this);
	    finalLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    finalLabel->setText(i18n("Final amount:"));
	    Plasma::Label *timeLabel = new Plasma::Label(this);
	    timeLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    timeLabel->setText(i18n("Time"));
	    Plasma::Label *time2Label = new Plasma::Label(this);
	    time2Label->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    time2Label->setText(i18n("Time in Half-Lives"));
	    
	    m_timeInHalfLives = new Plasma::Label(this);
	    m_timeInHalfLives->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    m_timeInHalfLives->setText(i18n("0 seconds"));	    
	    m_initAmt = new Plasma::SpinBox(this);
	    m_finalAmt = new Plasma::SpinBox(this);
	    m_time = new Plasma::SpinBox(this);
	    	    
		m_initUnit = new Plasma::ComboBox(this);
	    m_initUnit->setZValue(2);
	    m_finalUnit = new Plasma::ComboBox(this);
	    m_finalUnit->setZValue(2);
	    m_timeUnit = new Plasma::ComboBox(this);
	    m_timeUnit->setZValue(2);
	    m_initType = new Plasma::ComboBox(this);
	    m_initType->setZValue(2);
	    m_finalType = new Plasma::ComboBox(this);
	    m_finalType->setZValue(2);

		m_slider = new Plasma::Slider(this);
		m_slider->setRange(0, 100);
		m_slider->setOrientation(Qt::Horizontal);
		
  	    pGridLayout->addItem(m_r1, 5, 0);
   	    pGridLayout->addItem(m_r2, 6, 0);
  	    pGridLayout->addItem(m_r3, 7, 0);
  	    
  	    pGridLayout->addItem(initLabel, 5, 1);
   	    pGridLayout->addItem(finalLabel, 6, 1);
   	    pGridLayout->addItem(timeLabel, 7, 1);
   	    pGridLayout->addItem(time2Label, 8, 1);
   	    
  	    pGridLayout->addItem(m_initAmt, 5, 2);
   	    pGridLayout->addItem(m_finalAmt, 6, 2);
   	    pGridLayout->addItem(m_time, 7, 2);
   	    pGridLayout->addItem(m_slider , 8, 2);
   	    
   	    pGridLayout->addItem(m_initType, 5, 3);
   	    pGridLayout->addItem(m_finalType, 6, 3);
   	    pGridLayout->addItem(m_timeInHalfLives, 8, 3);
   	    
   	    pGridLayout->addItem(m_initUnit, 5, 4);
   	    pGridLayout->addItem(m_finalUnit, 6, 4);
   	    pGridLayout->addItem(m_timeUnit, 7, 4);
	    	    	    	    	    
	    //pGridLayout->addItem(
	}    
    return m_widget;
}

#include "nuclearCalculator.moc"
