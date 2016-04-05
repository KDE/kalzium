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

#include <avogadro/qtopengl/glwidget.h>

class Q_DECL_EXPORT KalziumGLWidget : public Avogadro::QtOpenGL::GLWidget
{
    Q_OBJECT
public:
    KalziumGLWidget(QWidget *parent = nullptr);
    virtual ~KalziumGLWidget();

public slots:
    void setLabels(int style);
    bool openFile(const QString &file);

protected:
    QByteArray m_lc_numeric;
    int m_lastEngine1, m_lastEngine2;
};

#endif // KALZIUMGLWIDGET_H
