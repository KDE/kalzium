/*
    SPDX-FileCopyrightText: 2008 Carsten Niehaus <cniehaus@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef DIDYOUKNOW_H
#define DIDYOUKNOW_H

#include <Plasma/Applet>
#include <Plasma/Svg>

#include <plasma/dataengine.h>

#include <QGraphicsTextItem>

class QSizeF;

class KalziumDidyouknow : public Plasma::Applet
{
    Q_OBJECT

    public:
        KalziumDidyouknow(QObject *parent, const QVariantList &args);
        ~KalziumDidyouknow();
        void init();

        void paintInterface(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            const QRect& contentsRect);
        void constraintsUpdated(Plasma::Constraints constraints);

    public slots:
        void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);

    private:
        Plasma::Svg m_theme;
        QGraphicsTextItem *m_label1;
        Plasma::DataEngine* m_engine;

        QSizeF m_size;
};

K_EXPORT_PLASMA_APPLET(didyouknow_kalzium, KalziumDidyouknow)

#endif // DIDYOUKNOW_H
