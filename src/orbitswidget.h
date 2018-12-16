/***************************************************************************
                           orbitswidget.h  -  description
                             -------------------
    begin                : June 2003
    copyright            : (C) 2003, 2004, 2005, 2006, 2007 by Carsten Niehaus
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

#ifndef ORBITSWIDGET_H
#define ORBITSWIDGET_H

#include <QLabel>
#include <QWidget>

/**
 * @brief the widget which displays the Bohr-orbit of the element
 * @author Carsten Niehaus
 */
class OrbitsWidget : public QWidget
{
    Q_OBJECT

public:
    OrbitsWidget(QWidget *parent = nullptr);

    void setElementNumber(int num);

private:
    /**
     * the elementnumber we are looking at
     */
    int Elemno;

    /// Label that shows the electronic configuration
    QLabel *m_electronConf;

    QList<int> numOfElectrons;

protected slots:
    virtual void paintEvent(QPaintEvent*);
};

#endif // ORBITSWIDGET_H
