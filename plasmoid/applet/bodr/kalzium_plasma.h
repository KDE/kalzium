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

#ifndef KALZIUM_PLASMA_H
#define KALZIUM_PLASMA_H

#include <Plasma/Applet>
#include <Plasma/Svg>
#include <plasma/widgets/lineedit.h>
#include <plasma/widgets/label.h>

#include <QGraphicsTextItem>

#include "ui_config.h"

class QDialog;
class QFont;

class KalziumPlasma : public Plasma::Applet
{
    Q_OBJECT

    public:
        KalziumPlasma(QObject *parent, const QVariantList &args);
        ~KalziumPlasma();
        void init();

        void paintInterface(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            const QRect& contentsRect);
        void constraintsUpdated(Plasma::Constraints constraints);

    public slots:
        void showConfigurationInterface();
        void configAccepted();
        void showFontSelectDlg();
        void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);

        void textChanged();
        void configChanged();

    private:
        Plasma::Svg m_theme;
        QGraphicsTextItem *m_label1;
        Plasma::LineEdit *m_lineedit;
        Plasma::DataEngine* m_engine;

        QString m_currentSource;

        Ui::config ui;
        QDialog *m_dialog;

        QFont m_font;
};

K_EXPORT_PLASMA_APPLET(data_kalzium, KalziumPlasma)

#endif // KALZIUM_PLASMA_H
