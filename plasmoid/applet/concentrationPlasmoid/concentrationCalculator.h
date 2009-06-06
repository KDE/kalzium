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
// Here we avoid loading the header multiple times
#ifndef concentrationCalculator_HEADER
#define concentrationCalculator_HEADER
// We need the Plasma Applet headers
#include <KIcon>
 
#include <Plasma/PopupApplet>
#include <Plasma/Svg>

namespace Plasma{
	class QSizeF;
	class ComboBox;
	class SpinBox;
	class TextEdit;
	class Label;
	class Slider;
	class SpinBox;
	class RadioButton;
};

// Define our plasma Applet
class concentrationCalculator : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        concentrationCalculator(QObject *parent, const QVariantList &args);
        ~concentrationCalculator();
        
        virtual void init();
        virtual QGraphicsWidget *graphicsWidget();
 
    private slots:
    // will be added soon
     
    private:
        QGraphicsWidget  *m_widget;
        Plasma::ComboBox *m_amountSoluteType;
        Plasma::ComboBox *m_amountSoluteUnit1;
        Plasma::ComboBox *m_amountSoluteUnit2;        
        Plasma::ComboBox *m_amountSolventType;
        Plasma::ComboBox *m_amountSolventUnit1;
        Plasma::ComboBox *m_amountSolventUnit2;
        Plasma::ComboBox *m_densitySoluteUnit;
        Plasma::ComboBox *m_densitySolventUnit;
        Plasma::ComboBox *m_concentrationUnit;
        
        Plasma::SpinBox *m_amountSolute;
        Plasma::SpinBox *m_molarMass;
        Plasma::SpinBox *m_eqtMass;
        Plasma::SpinBox *m_densitySolute;
        Plasma::SpinBox *m_amountSolvent;
        Plasma::SpinBox *m_molarMassSolvent;
        Plasma::SpinBox *m_densitySolvent;
        Plasma::SpinBox *m_concentration;   
        
        Plasma::RadioButton *m_r1,*m_r2,*m_r3,*m_r5,*m_r6,*m_r8;
      
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(concentrationCalculator, concentrationCalculator)
#endif //concentrationCalculator_HEADER
