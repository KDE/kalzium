/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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
#ifndef KALZIUMGLPART_H
#define KALZIUMGLPART_H

#include <kparts/factory.h>
#include <kparts/genericfactory.h>

#include <avogadro/glwidget.h>

class KalziumGLPart : public KParts::ReadOnlyPart
{
    Q_OBJECT
    public:
        KalziumGLPart(QWidget*, QObject*, const QStringList&);
        virtual ~KalziumGLPart();

        static KAboutData* createAboutData();

    protected:
        bool openFile();

        Avogadro::GLWidget* m_widget;
};

#endif // KALZIUMGLPART_H
