// Here we avoid loading the header multiple times
#ifndef gasCalculator_HEADER
#define gasCalculator_HEADER
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
class gasCalculator : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        gasCalculator(QObject *parent, const QVariantList &args);
        ~gasCalculator();
        
        virtual void init();
        virtual QGraphicsWidget *graphicsWidget();
 
    private slots:
    // will be added soon
     
    private:
        QGraphicsWidget  *m_widget;
        Plasma::ComboBox *m_massUnit;
        Plasma::ComboBox *m_pressureUnit;
        Plasma::ComboBox *m_temperatureUnit;
        Plasma::ComboBox *m_volumeUnit;
        Plasma::ComboBox *m_aUnit;
        Plasma::ComboBox *m_bUnit;
        
        Plasma::SpinBox *m_molarMass;
        Plasma::SpinBox *m_moles;
        Plasma::SpinBox *m_mass;
        Plasma::SpinBox *m_pressure;
        Plasma::SpinBox *m_temperature;
        Plasma::SpinBox *m_volume;
        Plasma::SpinBox *m_Vand_a;
        Plasma::SpinBox *m_Vand_b; 
        
        Plasma::RadioButton *m_r2,*m_r4,*m_r5,*m_r6;
      
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(gasCalculator, gasCalculator)
#endif //gasCalculator_HEADER
