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

#ifndef KALZIUMGLWIDGET_H
#define KALZIUMGLWIDGET_H

#include <avogadro/glwidget.h>

class KalziumGLWidget : public Avogadro::GLWidget
{
    Q_OBJECT
public:
    KalziumGLWidget(QWidget *parent = 0);
    virtual ~KalziumGLWidget();

public slots:
    void setStyle(int style);
    void setStyle2(int style);
    void setLabels(int style);
    void setQuality(int quality);
    bool openFile(const QString &file);

protected:
    QByteArray m_lc_numeric;
    int m_lastEngine1, m_lastEngine2;
};

#endif // KALZIUMGLWIDGET_H
