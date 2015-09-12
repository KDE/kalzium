/***************************************************************************
 *  Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
 *  Copyright (C) 2007-2008 by Marcus D. Hanwell <marcus@cryos.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kalziumglpart.h"

#include "kalziumglwidget.h"

#include <kparts/genericfactory.h>

KAboutData kalziumGLPartAboutData()
{
    KAboutData aboutData(
        "kalzium", QByteArray(), ki18n("Kalzium OpenGL Part"), "1.1.1",
        ki18n("A cool thing"),
        KAboutLicense::GPL, ki18n("(c) 2006, Carsten Niehaus"),
        KLocalizedString(),
        "http://edu.kde.org/kalzium/index.php"
    );
    aboutData.addAuthor(ki18n("Carsten Niehaus"), KLocalizedString(), "cniehaus@kde.org");
    aboutData.addAuthor(ki18n("Marcus D. Hanwell"), KLocalizedString(), "marcus@cryos.org");

    return aboutData;
}

K_PLUGIN_FACTORY(KalziumGLPartFactory, registerPlugin<KalziumGLPart>();)
K_EXPORT_PLUGIN(KalziumGLPartFactory(kalziumGLPartAboutData()))

KalziumGLPart::KalziumGLPart(QWidget* parentWidget, QObject* parent, const QVariantList& args)
{
    Q_UNUSED(parent);
    Q_UNUSED(parentWidget);
    Q_UNUSED(args);
    qDebug() << "KalziumGLPart::KalziumGLPart()";

    m_widget = new KalziumGLWidget();
    m_widget->setObjectName("KalziumGLWidget-KPart");
}

KalziumGLPart::~KalziumGLPart()
{
    delete m_widget;
    qDebug() << "KalziumGLPart::~KalziumGLPart()";
}

bool KalziumGLPart::openFile()
{
    return m_widget->openFile(url().toLocalFile());
}

#include "kalziumglpart.moc"
