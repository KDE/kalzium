/*
    SPDX-FileCopyrightText: 2008 Carsten Niehaus <cniehaus@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "didyouknow.h"

#include <QDebug>
#include <QPainter>
#include <QPen>

KalziumDidyouknow::KalziumDidyouknow(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    m_theme.setImagePath("widgets/chalkboard");

    m_engine = dataEngine("kalzium");
    m_label1 = 0;
    setHasConfigurationInterface(false);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
//     setDrawStandardBackground(false);
    setAssociatedApplication("kalzium");

    resize(512,256);
}

void KalziumDidyouknow::init()
{
    qDebug() << "initializing DidYouKnow-Applet";

    m_engine->connectSource("Fact", this, 1000);

    m_theme.setContainsMultipleImages(false);

    m_label1 = new QGraphicsTextItem(this);
    m_label1->setPos(m_theme.elementRect("canvas").topLeft());
    m_label1->setDefaultTextColor(Qt::white);
}

void KalziumDidyouknow::constraintsUpdated(Plasma::Constraints constraints)
{
//     setDrawStandardBackground(false);
    prepareGeometryChange();
    if (constraints & Plasma::SizeConstraint) {
         m_theme.resize(size());
    }

    m_label1->setPos(m_theme.elementRect("canvas").topLeft());
}

KalziumDidyouknow::~KalziumDidyouknow()
{
}

void KalziumDidyouknow::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
    qDebug() << "entering dataUpdated()";
    Q_UNUSED(source)

    if (m_label1)  {
//      m_label1->setAlignment(Qt::AlignLeft);
        m_label1->setPlainText(data["fact"].toString());
    }
}


void KalziumDidyouknow::paintInterface(QPainter *p,
                       const QStyleOptionGraphicsItem *option,
                       const QRect &contentsRect)
{
    Q_UNUSED(option)
    Q_UNUSED(contentsRect)
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);

    // Now we draw the applet, starting with our svg
    m_theme.resize(size());
    m_theme.paint(p, 0, 0);
}

#include "didyouknow.moc"
