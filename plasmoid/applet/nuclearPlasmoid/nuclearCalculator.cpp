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
#include <Plasma/GroupBox>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <plasma/svg.h>
#include <plasma/theme.h>

using namespace Conversion;

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
//FIXME:
// setZvalue function has a problem please fix it (look at the comboBox in the plasmoid
// and try to click on one of them to notice the exact problem)
// Also currently the spin boxes are integer, please convert them into double
// and uncomment certain lines of code which say 'setDecimals(4)'
// 3.> The radio buttons allow multiple selection which should not happen, they should be 
// grouped somehow.

	if (!m_widget) {	
		// Position all UI elements
		m_widget = new QGraphicsWidget(this);
	    Plasma::Frame *pHeader = new Plasma::Frame(this);
	    pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	    pHeader->setText(i18n("Nuclear Calculator"));
		
		Plasma::GroupBox *pGroupBox1 = new Plasma::GroupBox(this);
		Plasma::GroupBox *pGroupBox2 = new Plasma::GroupBox(this);
		
		QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(pGroupBox1);
	    QGraphicsGridLayout *pGridLayout2 = new QGraphicsGridLayout(pGroupBox2);
	    QGraphicsLinearLayout *pVLayout = new QGraphicsLinearLayout(Qt::Vertical,m_widget);
	    pVLayout->addItem(pGroupBox1);
	    pVLayout->addItem(pGroupBox2);
	    		
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
	    m_isotope->setZValue(1);
	    
	    m_halfLife = new Plasma::SpinBox(this);
	    //m_halfLife->setDecimals(4);
        m_halfLife->setMaximum(1e+09);

		m_halfLifeUnit = new Plasma::ComboBox(this);
		m_halfLifeUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("year", 0)
         << tr2i18n("seconds", 0)
         << tr2i18n("minutes", 0)
         << tr2i18n("hours", 0)
         << tr2i18n("days", 0)
         << tr2i18n("weeks", 0));
	    m_halfLifeUnit->setZValue(6);
	    
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
	    //m_initAmt->setDecimals(4);
        m_initAmt->setMaximum(1e+09);
	    m_finalAmt = new Plasma::SpinBox(this);
	    //m_finalAmt->setDecimals(4);
        m_finalAmt->setMaximum(1e+09);
	    m_time = new Plasma::SpinBox(this);
	    //m_time->setDecimals(4);
        m_time->setMaximum(1e+09);
	    	    
		m_initUnit = new Plasma::ComboBox(this);
		m_initUnit->setZValue(2);
	    m_initUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams", 0)
         << tr2i18n("tons", 0)
         << tr2i18n("carats", 0)
         << tr2i18n("pounds", 0)
         << tr2i18n("ounces", 0)
         << tr2i18n("troy ounces", 0));
        m_initUnit->setZValue(3);
         
	    m_finalUnit = new Plasma::ComboBox(this);
	    m_finalUnit->setZValue(2);
	    m_finalUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams", 0)
         << tr2i18n("tons", 0)
         << tr2i18n("carats", 0)
         << tr2i18n("pounds", 0)
         << tr2i18n("ounces", 0)
         << tr2i18n("troy ounces", 0));
        m_finalUnit->setZValue(2);
         
	    m_timeUnit = new Plasma::ComboBox(this);
	    m_timeUnit->setZValue(2);
	    m_timeUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("year", 0)
         << tr2i18n("seconds", 0)
         << tr2i18n("minutes", 0)
         << tr2i18n("hours", 0)
         << tr2i18n("days", 0)
         << tr2i18n("weeks", 0));
        m_timeUnit->setZValue(1);
        
	    m_initType = new Plasma::ComboBox(this);
	    m_initType->setZValue(2);
	    m_initType->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("Mass", 0)
         << tr2i18n("moles", 0));
        m_initType->setZValue(2);
         	
	    m_finalType = new Plasma::ComboBox(this);
	    m_finalType->setZValue(2);
	    m_finalType->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("Mass", 0)
         << tr2i18n("moles", 0));
		m_finalType->setZValue(1);
		
		m_slider = new Plasma::Slider(this);
		m_slider->setRange(0, 100);
		m_slider->setOrientation(Qt::Horizontal);
		m_slider->setMaximum(100);
		
  	    pGridLayout2->addItem(m_r1, 5, 0);
   	    pGridLayout2->addItem(m_r2, 6, 0);
  	    pGridLayout2->addItem(m_r3, 7, 0);
  	    
  	    pGridLayout2->addItem(initLabel, 5, 1);
   	    pGridLayout2->addItem(finalLabel, 6, 1);
   	    pGridLayout2->addItem(timeLabel, 7, 1);
   	    pGridLayout2->addItem(time2Label, 8, 1);
   	    
  	    pGridLayout2->addItem(m_initAmt, 5, 2);
   	    pGridLayout2->addItem(m_finalAmt, 6, 2);
   	    pGridLayout2->addItem(m_time, 7, 2);
   	    pGridLayout2->addItem(m_slider , 8, 2);
   	    
   	    pGridLayout2->addItem(m_initType, 5, 3);
   	    pGridLayout2->addItem(m_finalType, 6, 3);
   	    pGridLayout2->addItem(m_timeInHalfLives, 8, 3);
   	    
   	    pGridLayout2->addItem(m_initUnit, 5, 4);
   	    pGridLayout2->addItem(m_finalUnit, 6, 4);
   	    pGridLayout2->addItem(m_timeUnit, 7, 4);
	    
	    // Positioning of UI elements done	    	    	    	    
		// Now add required properties to the UI widgets
		
	    /**************************************************************************/
	    //                       Nuclear Calculator set up                        //
	    /**************************************************************************/
	    KalziumDataObject *kdo = KalziumDataObject::instance();

	    // add all element names to the comboBox in the user interface
	    foreach(Element * e, kdo -> ElementList) {
	        m_element->nativeWidget()->addItem(e -> dataAsString(ChemicalDataObject::name));
	    }
		///FIXME
	    /* The last three elemenents will be removed because information is not available
	       and causes the program to crash when selected. */
	    int count = m_element->nativeWidget()->count();
	    m_element->nativeWidget()->removeItem(count - 1);
	    m_element->nativeWidget()->removeItem(count - 2);
	    m_element->nativeWidget()->removeItem(count - 3);
	    // Add all isotope names of Uranium ( by default )to the isotope comboBox
	    QList<Isotope*> list = KalziumDataObject::instance()->isotopes(92);
	    QString isotope;
	    foreach(Isotope * i , list) {
	        isotope.setNum(i -> mass());
	        m_isotope->nativeWidget()->addItem(isotope);
	    }

	    // initialise the data, initially selected values ( Uranium, 92, 238)
	    m_element->nativeWidget()-> setCurrentIndex(91);
	    m_isotope->nativeWidget()-> setCurrentIndex(18);
	    m_halfLife-> setValue(list.at(18)->halflife());
	    // Setup of the UI done
	    // Initialise values of the objects of the class
	    
	    m_InitAmount=Value(0.0, "g") ;
	    m_FinalAmount=Value(0.0, "g");
	    m_Mass = list. at(18) -> mass();
	    m_Time = Value(0.0, "y");
	    m_HalfLife = Value(list . at(18) -> halflife(), "y");

	    m_Element = * KalziumDataObject::instance() -> element(92);
	    m_Isotope = * list . at(18);
	}    
    return m_widget;
}

#include "nuclearCalculator.moc"
