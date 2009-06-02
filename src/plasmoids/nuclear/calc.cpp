#include "calc.h"
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
#include <QGraphicsGridLayout>
 
#include <plasma/svg.h>
#include <plasma/theme.h>

calc::calc(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
	m_widget(0)
{
    KGlobal::locale()->insertCatalog("libconversion");
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setHasConfigurationInterface(true);
    resize(400, 300);
}

calc::~calc()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }
}
 
void calc::init()
{

} 
 
 
QGraphicsWidget *calc::graphicsWidget()
{
    if (!m_widget)
    {
        m_widget = new QGraphicsWidget(this);
        Plasma::Frame *pHeader = new Plasma::Frame(this);
        pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        pHeader->setText(i18n("Nuclear Calculator"));

        Plasma::Label *eleLabel = new Plasma::Label(this);
        eleLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        eleLabel->setText(i18n("Element name:"));
        m_element = new Plasma::ComboBox(this);
        
        QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(m_widget);
        pGridLayout->addItem(pHeader, 0, 0, 1, 2);
        pGridLayout->addItem(eleLabel, 1, 0);
        pGridLayout->addItem(m_element, 1, 1);
    }
    return m_widget;
}

#include "calc.moc"
