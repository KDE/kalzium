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
#include "didyouknow.h"

#include <QDebug>
#include <QPainter>

KalziumDidyouknow::KalziumDidyouknow(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_theme("widgets/chalkboard", this)
{
    m_label1 = 0;
    setHasConfigurationInterface(false);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
    setDrawStandardBackground(false);

//    resize(256,160);
}

void KalziumDidyouknow::init()
{
    qDebug() << "initializing DidYouKnow-Applet";

    KConfigGroup cg = config();
    Plasma::DataEngine* kalziumEngine = dataEngine("kalzium");
    kalziumEngine->connectSource("BlueObelisk", this, 1000);

    m_theme.setContentType(Plasma::Svg::SingleImage);

    m_label1 = new Plasma::Label(this);
    m_label1->setPos( m_theme.elementRect( "canvas" ).topLeft() );
}

void KalziumDidyouknow::constraintsUpdated(Plasma::Constraints constraints)
{
    qDebug() << "constraintsUpdated()";
    setDrawStandardBackground(false);
//X     prepareGeometryChange();
//X     if (constraints & Plasma::SizeConstraint) {
//X         m_theme.resize(contentSize().toSize());
//X     }
}

KalziumDidyouknow::~KalziumDidyouknow()
{
}

void KalziumDidyouknow::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
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
}

void KalziumDidyouknow::paintInterface(QPainter *p,
                       const QStyleOptionGraphicsItem *option,
                       const QRect &contentsRect)
{
    Q_UNUSED(option);

    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);

    // Now we draw the applet, starting with our svg
    m_theme.resize((int)contentsRect.width(), (int)contentsRect.height());
    m_theme.paint(p, (int)contentsRect.left(), (int)contentsRect.top());
}

#include "didyouknow.moc"
