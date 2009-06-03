// Here we avoid loading the header multiple times
#ifndef nuclearCalculator_HEADER
#define nuclearCalculator_HEADER
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
class nuclearCalculator : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        nuclearCalculator(QObject *parent, const QVariantList &args);
        ~nuclearCalculator();
        
        virtual void init();
        virtual QGraphicsWidget *graphicsWidget();
 
    private slots:
    // will be added soon
     
    private:
        QGraphicsWidget  *m_widget;
        Plasma::ComboBox *m_element;
        Plasma::ComboBox *m_isotope;
        Plasma::ComboBox *m_halfLifeUnit;
        Plasma::ComboBox *m_initType;
        Plasma::ComboBox *m_finalType;
        Plasma::ComboBox *m_initUnit;
        Plasma::ComboBox *m_finalUnit;
        Plasma::ComboBox *m_timeUnit;
        
        Plasma::SpinBox *m_halfLife;
        Plasma::SpinBox *m_initAmt;
        Plasma::SpinBox *m_finalAmt;
        Plasma::SpinBox *m_time;

		Plasma::Slider *m_slider;
        
        Plasma::Label *m_timeInHalfLives;      
        
        Plasma::RadioButton *m_r1;
        Plasma::RadioButton *m_r2;
        Plasma::RadioButton *m_r3;
        
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(nuclearCalculator, nuclearCalculator)
#endif //nuclearCalculator_HEADER
