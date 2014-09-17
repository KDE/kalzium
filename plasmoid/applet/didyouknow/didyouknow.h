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

#ifndef DIDYOUKNOW_H
#define DIDYOUKNOW_H

#include <Plasma/Applet>
#include <Plasma/Svg>

#include <plasma/dataengine.h>

#include <QGraphicsTextItem>

#include <KRandomSequence>

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

        KRandomSequence * m_random;

        QSizeF m_size;
};

K_EXPORT_PLASMA_APPLET(didyouknow_kalzium, KalziumDidyouknow)

#endif // DIDYOUKNOW_H
