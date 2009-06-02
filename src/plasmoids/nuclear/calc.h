// Here we avoid loading the header multiple times
#ifndef calc_HEADER
#define calc_HEADER
// We need the Plasma Applet headers
#include <KIcon>
 
#include <Plasma/Applet>
#include <Plasma/Svg>

namespace Plasma{
	class QSizeF;
	class ComboBox;
	class SpinBox;
	class TextEdit;
	class Label;
	class Slider;
}

// Define our plasma Applet
class calc : public Plasma::Applet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        calc(QObject *parent, const QVariantList &args);
        ~calc();
        
        virtual void init();
        virtual QGraphicsWidget *graphicsWidget();
 
    private slots:
    // will be added soon
     
    private:
        QGraphicsWidget  *m_widget;
        Plasma::ComboBox *m_element;
        Plasma::ComboBox *m_isotope;
        Plasma::ComboBox *m_yearUnit;
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
        
        Plasma::Label *m_atomicMass;
        Plasma::Label *m_timeInHalfLives;      
        
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(calc, calc)
#endif //calc_HEADER
