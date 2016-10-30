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

#ifndef KALZIUMGLPART_H
#define KALZIUMGLPART_H

#include <readonlypart.h>

class KalziumGLWidget;

class KalziumGLPart : public KParts::ReadOnlyPart
{
    Q_OBJECT
public:
    KalziumGLPart(QWidget*, QObject*, const QVariantList&);
    virtual ~KalziumGLPart();

protected:
    bool openFile();

    KalziumGLWidget* m_widget;
};

#endif // KALZIUMGLPART_H
