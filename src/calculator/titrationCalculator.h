/***************************************************************************
 *   Copyright (C) 2010 by Luca Tringali, TRINGALINVENT@libero.it          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef TITRATIONCALCULATOR_H
#define TITRATIONCALCULATOR_H

#include <QDebug>
#include <QString>
#include <QStringList>

#include <KPlotObject>
#include <KPlotPoint>
#include <KPlotWidget>

#include <element.h>
#include <isotope.h>
#include <prefs.h>

#include "ui_titrationCalculator.h"

/*
 * This class implements the titration calculator: it can solve a system of chemical equilibria equations
 * and find out the equivalence point of an experimental titration.
 *
 * @author Luca Tringali
 */
class titrationCalculator : public QWidget
{
    Q_OBJECT

public:
    titrationCalculator(QWidget * parent = nullptr);
    ~titrationCalculator();

public slots:
    void on_pushButton_clicked();
    void on_xmin_valueChanged(double val);
    void on_xmax_valueChanged(double val);
    void on_ymin_valueChanged(double val);
    void on_ymax_valueChanged(double val);

    void on_actionSave_image_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_actionRapid_Help_triggered();

private:
    void resize();
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    int width;
    int end;
    int lettere;
    int temponu;
    double a;
    void plot();
    QString solve(char *yvalue);
    QString solvex(char *yvalue, const QString &dnum);
    QImage tempi;
    double setttanh(double x);
    QString redplot;
    QString greenplot;
    QString blueplot;

    Ui::titrationCalculator *ui;               // The user interface pointer
    Ui::titrationCalculator uid;               // The user interface
};

#endif // TITRATIONCALCULATOR_H
