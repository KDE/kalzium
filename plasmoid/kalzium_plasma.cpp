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
    m_font(QFont())
{
    m_dialog = 0;
    m_label1 = 0;
    /*m_label2 = 0;
    m_label3 = 0;
    m_label4 = 0;
    m_label5 = 0;*/
    setHasConfigurationInterface(true);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
    setDrawStandardBackground(false);

    resize(256,160);
}

void KalziumPlasma::init()
{
    qDebug() << "initializing Kalzium";

    KConfigGroup cg = config();
    m_updateInterval = cg.readEntry("updateInterval", 1000);
    Plasma::DataEngine* kalziumEngine = dataEngine("kalzium");
    kalziumEngine->connectSource("BlueObelisk", this, m_updateInterval);

    m_theme.setContentType(Plasma::Svg::SingleImage);

    m_label1 = new Plasma::Label(this);
    /*m_label2 = new Plasma::Label(this);
    m_label3 = new Plasma::Label(this);
    m_label4 = new Plasma::Label(this);
    m_label5 = new Plasma::Label(this);*/

    m_label1->setPos( m_theme.elementRect( "name" ).topLeft() );
    /*m_label2->setPos( m_theme.elementRect( "symbol" ).topLeft() );
    m_label3->setPos( m_theme.elementRect( "bp" ).topLeft() );
    m_label4->setPos( m_theme.elementRect( "mp" ).topLeft() );
    m_label5->setPos( m_theme.elementRect( "mass" ).topLeft() );*/
    
    m_label1->setFont(cg.readEntry("font",m_font));
    /*m_label2->setFont(cg.readEntry("font",m_font));
    m_label3->setFont(cg.readEntry("font",m_font));
    m_label4->setFont(cg.readEntry("font",m_font));
    m_label5->setFont(cg.readEntry("font",m_font));*/
}

void KalziumPlasma::constraintsUpdated(Plasma::Constraints constraints)
{
    qDebug() << "constraintsUpdated()";
    setDrawStandardBackground(false);
    prepareGeometryChange();
    if (constraints & Plasma::SizeConstraint) {
        m_theme.resize(contentSize().toSize());
    }
//X     m_label1->setPos( m_theme.elementRect( "name" ).topLeft() );
//X     m_label1->setFont( m_font );
//X     double scale = qMin(m_theme.elementRect( "name" ).width()/m_label1->boundingRect().width(),
//X             m_theme.elementRect( "name" ).height()/m_label1->boundingRect().height());
//X     m_label1->setTransform(QTransform().scale(scale, scale));
    
//X     m_label2->setPos( m_theme.elementRect( "symbol" ).topLeft() );
//X     m_label2->setFont( m_font );
//X     scale = qMin(m_theme.elementRect( "symbol" ).width()/m_label2->boundingRect().width(),
//X             m_theme.elementRect( "symbol" ).height()/m_label2->boundingRect().height());
//X     m_label2->setTransform(QTransform().scale(scale, scale));
//X     
//X     m_label3->setPos( m_theme.elementRect( "bp" ).topLeft() );
//X     m_label3->setFont( m_font );
//X     scale = qMin(m_theme.elementRect( "bp" ).width()/m_label3->boundingRect().width(),
//X             m_theme.elementRect( "bp" ).height()/m_label3->boundingRect().height());
//X     m_label3->setTransform(QTransform().scale(scale, scale));
//X 
//X     m_label4->setPos( m_theme.elementRect( "mp" ).topLeft() );
//X     m_label4->setFont( m_font );
//X     scale = qMin(m_theme.elementRect( "mp" ).width()/m_label4->boundingRect().width(),
//X             m_theme.elementRect( "mp" ).height()/m_label4->boundingRect().height());
//X     m_label4->setTransform(QTransform().scale(scale, scale));
//X 
//X     m_label5->setPos( m_theme.elementRect( "symbol" ).topLeft() );
//X     m_label5->setFont( m_font );
//X     scale = qMin(m_theme.elementRect( "symbol" ).width()/m_label5->boundingRect().width(),
//X             m_theme.elementRect( "symbol" ).height()/m_label5->boundingRect().height());
//X     m_label5->setTransform(QTransform().scale(scale, scale));
}

KalziumPlasma::~KalziumPlasma()
{
    delete m_dialog;
}

void KalziumPlasma::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(source);

    QString bp = data["bp"].toString();
    QString mp = data["mp"].toString();
    QString mass = data["mass"].toString();
    QString symbol = data["symbol"].toString();
    QString name = data["name"].toString();
    QString text;
    text = QString(i18n( "\nName: %1", name ));
    text.append(QString(i18n( "\nSymbol: %1", symbol)));
    text.append(QString(i18n( "\nBoilingpoint: %1", bp)));
    text.append(QString(i18n( "\nMeltingpoint: %1", mp)));
    text.append(QString(i18n( "\nMass: %1", mass)));
    if (m_label1)  {
	m_label1->setAlignment(Qt::AlignLeft);
        m_label1->setText(text);
    }
    /*if (m_label2)
        m_label2->setText(i18n("\nSymbol: %1", symbol));
    if (m_label3)
        m_label3->setText(i18n("\nBoilingpoint: %1", bp));
    if (m_label4)
        m_label4->setText(i18n("\nMeltingpoint: %1", mp));
    if (m_label5)
        m_label5->setText(i18n("\nMass: %1", mass));*/
}

void KalziumPlasma::paintInterface(QPainter *p,
                       const QStyleOptionGraphicsItem *option,
                       const QRect &contentsRect)
{
    Q_UNUSED(option);

    m_theme.resize((int)contentsRect.width(),
            (int)contentsRect.height());
    m_theme.paint(p,
            (int)contentsRect.left() - 20,
            (int)contentsRect.top() -10 );
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
}

void KalziumPlasma::hoverLeaveEvent(QGraphicsSceneHoverEvent  * event)
{
    Plasma::Applet::hoverLeaveEvent(event);
}

#include "kalzium_plasma.moc"
