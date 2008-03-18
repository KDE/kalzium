/***************************************************************************
    copyright            : (C) 2008 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "kalzium_plasma.h"

#include <KDialog>
#include <KConfigGroup>
#include <KFontDialog>
#include <KColorDialog>

#include <QDebug>

KalziumPlasma::KalziumPlasma(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_theme("widgets/kalzium_plasma_card", this),
    m_dialog(0),
    m_font(QFont()),
    m_size(256,160)

{
    m_dialog = 0;
    m_label1 = 0;
    m_label2 = 0;
    setHasConfigurationInterface(true);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
    setDrawStandardBackground(false);

    m_theme.resize();
}

void KalziumPlasma::init()
{
    qDebug() << "initializing Kalzium";

    KConfigGroup cg = config();
    m_updateInterval = cg.readEntry("updateInterval", 1000);
    Plasma::DataEngine* kalziumEngine = dataEngine("kalzium");
    kalziumEngine->connectSource("BlueObelisk", this, m_updateInterval);

    m_theme.setContentType(Plasma::Svg::SingleImage);
    m_theme.size().height();

    m_label1 = new Plasma::Label(this);
    m_label2 = new Plasma::Label(this);

    m_label1->setText("label1 debug");
    m_label2->setText("label2 debug");
    
    m_label1->setPos( m_theme.elementRect( "translation1" ).topLeft() );
    m_label2->setPos( m_theme.elementRect( "translation2" ).topLeft() );
    
    m_label1->setFont(cg.readEntry("font",m_font));
    m_label2->setFont(cg.readEntry("font",m_font));
}

void KalziumPlasma::constraintsUpdated(Plasma::Constraints constraints)
{
    qDebug() << "constraintsUpdated()";
    setDrawStandardBackground(false);
    prepareGeometryChange();
    if (constraints & Plasma::SizeConstraint) {
        m_theme.resize(contentSize().toSize());
    }
    m_label1->setPos( m_theme.elementRect( "translation1" ).topLeft() );
    m_label1->setFont( m_font );
    double scale = qMin(m_theme.elementRect( "translation1" ).width()/m_label1->boundingRect().width(), m_theme.elementRect( "translation1" ).height()/m_label1->boundingRect().height());
    m_label1->setTransform(QTransform().scale(scale, scale));

    m_label2->setPos( m_theme.elementRect( "translation2" ).topLeft() );
    m_label2->setFont( m_font );
    scale = qMin(m_theme.elementRect( "translation2" ).width()/m_label2->boundingRect().width(), m_theme.elementRect( "translation2" ).height()/m_label2->boundingRect().height());
    m_label2->setTransform(QTransform().scale(scale, scale));
}

KalziumPlasma::~KalziumPlasma()
{
    delete m_dialog;
}

void KalziumPlasma::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(source);
    qDebug() << "dataUpdated called =========== source is: " << source << " =============================";
    
    if ( m_label1) {
        QString text = (data["BlueObelisk"]).toString();
        m_label1->setText(text);
        double scale = qMin(m_theme.elementRect( "translation1" ).width()/m_label1->boundingRect().width(), m_theme.elementRect( "translation1" ).height()/m_label1->boundingRect().height());
        m_label1->setTransform(QTransform().scale(scale, scale));
        m_label1->setPos(m_theme.elementRect( "translation1" ).topLeft()
                + QPointF(
                    (m_theme.elementRect("translation1").width()-m_label1->boundingRect().width()*scale)/2.0,
                    (m_theme.elementRect("translation1").height()-m_label1->boundingRect().height()*scale)/2.0));

    }

}

void KalziumPlasma::setContentSize(const QSizeF& size)
{
m_size = size;
}

QSizeF KalziumPlasma::contentSizeHint() const
{
return m_size;
}

void KalziumPlasma::paintInterface(QPainter *p,
                       const QStyleOptionGraphicsItem *option,
                       const QRect &contentsRect)
{
Q_UNUSED(option);

m_theme.resize((int)contentsRect.width(),
                     (int)contentsRect.height());
m_theme.paint(p,
                   (int)contentsRect.left(),
                   (int)contentsRect.top());
}

void KalziumPlasma::showConfigurationInterface()
{
if (m_dialog == 0) {
    m_dialog = new KDialog;
    m_dialog->setWindowIcon(KIcon("kalzium"));
        m_dialog->setCaption( i18n("KalziumPlasma Configuration") );
        ui.setupUi(m_dialog->mainWidget());
        m_dialog->mainWidget()->layout()->setMargin(0);
        ui.updateIntervalSpinBox->setValue(m_updateInterval/1000);
        m_dialog->setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Apply );
        connect( m_dialog, SIGNAL(applyClicked()), this, SLOT(configAccepted()) );
        connect( m_dialog, SIGNAL(okClicked()), this, SLOT(configAccepted()) );
        connect( ui.fontSelectButton, SIGNAL(clicked()), this, SLOT(showFontSelectDlg()) );
    }

    m_dialog->show();
}

void KalziumPlasma::showFontSelectDlg()
{
    KFontDialog::getFont(m_font);
}

void KalziumPlasma::configAccepted()
{
    prepareGeometryChange();

    KConfigGroup cg = config();
    cg.writeEntry("font", m_font);
    m_label1->setFont(m_font);
    m_label2->setFont(m_font);
    m_updateInterval = ui.updateIntervalSpinBox->value()*1000;
    cg.writeEntry("updateInterval", m_updateInterval);
    Plasma::DataEngine* kalziumEngine = dataEngine("kalzium");
    kalziumEngine->connectSource("lang:0", this, m_updateInterval);
    kalziumEngine->connectSource("lang:1", this, m_updateInterval);
    emit configNeedsSaving();
}

void KalziumPlasma::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    Plasma::Applet::hoverEnterEvent(event);
    m_label2->show();
}

void KalziumPlasma::hoverLeaveEvent(QGraphicsSceneHoverEvent  * event)
{
    Plasma::Applet::hoverLeaveEvent(event);
    m_label2->hide();
}

#include "kalzium_plasma.moc"
