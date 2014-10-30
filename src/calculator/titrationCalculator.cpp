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

#include "titrationCalculator.h"

#include <math.h>
#include "prefs.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <QFileDialog>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptClass>
#include <QVarLengthArray>
#include <qscriptengine.h>
#include <stdlib.h>
#include <float.h>
#include <QMessageBox>

using namespace std;

#ifdef _MSC_VER
#define and &&
#define or ||
#endif

titrationCalculator::titrationCalculator(QWidget * parent) : QWidget(parent)
{
    xmin = 0;
    xmax = 50;
    ymin = 0;
    ymax = 50;
    width = int(xmax - xmin);

    uid.setupUi(this);
    uid.tabWidget->setTabText(0, i18n("Experimental values"));
    uid.tabWidget->setTabText(1, i18n("Theoretical equations"));
    uid.tab->setFocus();
    plot();

    connect(uid.pushButton, SIGNAL(clicked()),this, SLOT(on_pushButton_clicked()));
    connect(uid.xmin, SIGNAL(valueChanged(double)),this, SLOT(on_xmin_valueChanged(double)));
    connect(uid.xmax, SIGNAL(valueChanged(double)),this, SLOT(on_xmax_valueChanged(double)));
    connect(uid.ymin, SIGNAL(valueChanged(double)),this, SLOT(on_ymin_valueChanged(double)));
    connect(uid.ymax, SIGNAL(valueChanged(double)),this, SLOT(on_ymax_valueChanged(double)));


    connect(uid.saveimage, SIGNAL(clicked()),this, SLOT(on_actionSave_image_triggered()));
    connect(uid.open, SIGNAL(clicked()),this, SLOT(on_actionOpen_triggered()));
    connect(uid.save, SIGNAL(clicked()),this, SLOT(on_actionSave_triggered()));
    connect(uid.newfile, SIGNAL(clicked()),this, SLOT(on_actionNew_triggered()));
    connect(uid.rapidhelp, SIGNAL(clicked()),this, SLOT(on_actionRapid_Help_triggered()));
}

titrationCalculator:: ~titrationCalculator()
{
}

void titrationCalculator::plot()
{
    width = int(xmax - xmin);
    //now I'm preparing the kplot widget
    uid.kplotwidget->removeAllPlotObjects();
    uid.kplotwidget->setLimits(xmin, xmax, ymin, ymax); //now I need to set the limits of the plot

    KPlotObject *kpor = new KPlotObject(Qt::red, KPlotObject::Lines);
    KPlotObject *kpog = new KPlotObject(Qt::green, KPlotObject::Lines);
    KPlotObject *kpob = new KPlotObject(Qt::blue, KPlotObject::Lines);
    redplot = "<polyline points=\"";
    greenplot = "<polyline points=\"";
    blueplot = "<polyline points=\"";

    if (!uid.tableWidget->item(0,0) || uid.tableWidget->item(0,0)->text().isEmpty()) {
        //go on
    } else {
        char yvalue[80];
        int tmpy = 0;
        for (int i = 0; i < uid.tableWidget->rowCount(); ++i) {
            if (!uid.tableWidget->item(i,0) || uid.tableWidget->item(i,0)->text().isEmpty()) {
                break;
            } else {
                if (uid.tableWidget->item(i,0)->data(Qt::DisplayRole).toString() == uid.yaxis->text()) {
                    QString yvalueq = uid.tableWidget->item(i,1)->data(Qt::DisplayRole).toString();
                    QByteArray ba = yvalueq.toLatin1();
                    char *yvaluen = ba.data();
                    strcpy(yvalue,yvaluen);
                    tmpy = 1;
                }
            }
        }
        QString mreporto;
        int iter = 0;
        if (uid.xaxis->text() == "" or uid.xaxis->text() == " ") {
            uid.xaxis->setText(i18n("nothing"));
        }
        if (tmpy == 0) {
            QMessageBox::critical(this,i18n("Error"),i18n("Unable to find an equation for Y-axis variable."));
        } else {
            //now we have to solve the system of equations NOTE:yvalue contains the equation of Y-axis variable
            //we iterates the process until you have an equation in one only unknown variable or a numeric expression
            mreporto = solve(yvalue);
            while (end == 0 or lettere == 1) {
                QByteArray ba = mreporto.toLatin1();
                char *tmreport = ba.data();
                ++iter;
                if (end == 1 or lettere == 0) {
                    break;
                }
                if (iter > 100) {
                    break; //preventing from an endless iteration
                }
                mreporto = solve(tmreport);
            }
        }
        //if (mreporto!="") uid.note->setText("Theoretical Curve: "+mreporto);
        if (!mreporto.isEmpty()) {
            uid.note->setText(i18n("Theoretical curve") + ": " + mreporto);
            for (int i = int(xmin); i < int(xmax); ++i) {
                double id = i;
                QScriptEngine myEngine;
                QByteArray ban = mreporto.toLatin1();
                char *tmreporto = ban.data();

                QString istr;
                istr.append(QString("%1").arg((id)));
                //now i'm using QScript language to solve the expression
                //in a future we can consider to change it supporting some backends, but it's really complex
                QString myscript = solvex(tmreporto,istr);
                QScriptValue three = myEngine.evaluate(myscript);

                double tvalue = three.toNumber();
                kpor->addPoint(id, tvalue);
                redplot = redplot
                          + ' '
                          + QString::number((id * 10) + 5).replace(QChar(','), QChar('.'))
                          + ','
                          + QString::number((ymax - tvalue) * 10).replace(QChar(','), QChar('.'));
            }
        }
        temponu = 0;
    } //here ends the equations mode

    //uid.tableWidget_2->sortItems(1, Qt::AscendingOrder); //seems that the sorting doesn't work correctly
    if (!uid.tableWidget_2->item(0, 0) || uid.tableWidget_2->item(0, 0)->text().isEmpty()) {
        //go on
    } else {
        //now we can plot the values
        double a, b, c, d, xval;
        QVarLengthArray<double, 64> px(uid.tableWidget_2->rowCount());
        QVarLengthArray<double, 64> py(uid.tableWidget_2->rowCount());
        int totaldata = 0;
        for (int i = 0; i < uid.tableWidget_2->rowCount(); ++i) {
            if (!uid.tableWidget_2->item(i,0) || uid.tableWidget_2->item(i, 0)->text().isEmpty()) {
                break;
            } else {
                ++totaldata;
                kpob->addPoint(uid.tableWidget_2->item(i,1)->data(Qt::DisplayRole).toDouble(), uid.tableWidget_2->item(i,0)->data(Qt::DisplayRole).toDouble());
                py[i] = uid.tableWidget_2->item(i,0)->data(Qt::DisplayRole).toDouble();
                px[i] = uid.tableWidget_2->item(i,1)->data(Qt::DisplayRole).toDouble();
                blueplot = blueplot
                           + ' '
                           + QString::number((uid.tableWidget_2->item(i,1)->data(Qt::DisplayRole).toDouble() * 10) + 5).replace(QChar(','), QChar('.'))
                           + ','
                           + QString::number((ymax-uid.tableWidget_2->item(i, 0)->data(Qt::DisplayRole).toDouble()) * 10).replace(QChar(','), QChar('.'));
            }
        }
        a = py[totaldata - 1] - py[0];
        b = 4 / (px[totaldata - 1] - px[0]);
        d = 0;
        if (a > 0) {
            d = py[0] + (a / 2);
        } else if (a < 0) {
            d = py[totaldata - 1] - (a / 2);
        }
        double cn;
        kDebug() << cn;
        int th = 0;
        for (int i = 1; i < (totaldata - 1); ++i) {
            //now i'm using the value of the points to fit the curve
            double ci = ((setttanh((py[i] - d) / a)) / b) - px[i];
            if ((ci * 0) == 0) {
                cn = cn + ci;
                ++th;
            }
        }
        //c = cn/(th); it doesn't wok, but i found out this little hack. The strange thing is that in the standalone application it works fine.
        c = cn / (th * 2);
        //THIS IS THE PLOT OF APPROXIMATED CURVE
        for (int i = int(xmin); i < (int(xmax)); ++i) {
            double id = i;
            xval = (a * tanh(b * (id + c))) + d;
            kpog->addPoint(id, xval);
            greenplot = greenplot
                        + ' '
                        + QString::number((id * 10) + 5).replace(QChar(','), QChar('.'))
                        + ','
                        + QString::number((ymax-xval) * 10).replace(QChar(','), QChar('.'));
        }
        //THIS IS THE EQUIVALENCE POINT (THE INFLECTION OF THE CURVE)
        QString es = QString::number(-c);
        QString as = QString::number(a);
        QString bs = QString::number(b);
        QString cs = QString::number(c);
        QString ds = QString::number(d);
        QString tempon = uid.note->toPlainText()+QChar('\n');
        if (temponu != 0) {
            tempon = "";
        }
        uid.note->setText(tempon
                          + '\n'
                          + i18n("Approximated curve")
                          + ": "
                          + as
                          + "*tanh("
                          + bs
                          + "*(x+"+cs+"))+"
                          + ds
                          +'\n'
                          + i18n("Equivalence point")
                          + ": "
                          + es);
    } //here ends the experimental values mode

    uid.kplotwidget->addPlotObject(kpor);
    uid.kplotwidget->addPlotObject(kpog);
    uid.kplotwidget->addPlotObject(kpob);

    redplot = redplot + "\" style=\"stroke:red;fill:none\"/> ";
    blueplot = blueplot + "\" style=\"stroke:blue;fill:none\"/> ";
    greenplot = greenplot + "\" style=\"stroke:green;fill:none\"/> ";

}

double titrationCalculator::setttanh(double x)
{
    double temp;
    temp = log((1 + x) / (1 - x)) / 2;
    return temp;
}


QString titrationCalculator::solve(char *yvalue)
{

    QString mreport;
    lettere = 0;
    //now we have to solve the system of equations
    //yvalue contains the equation of Y-axis variable
    QString tempy = "";
    end = 1;
    mreport = "";
    QString tempyval;
    QString ptem;
    for (int i = 0; strlen(yvalue) + 1; ++i) {
        if (!(yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='^' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',')) {
            break; //if current value is not a permitted value, this means that something is wrong
        }
        if (yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M' or yvalue[i]=='.' or yvalue[i]==',') {
            lettere = 1; //if lettere == 0 then the equation contains only mnumbers
        }
        if (yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='^' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',') {
            tempyval = tempyval + QString(yvalue[i]);
        } else {
            tempy = tempy + QString(yvalue[i]);
            for (int i = 0; i < uid.tableWidget->rowCount(); ++i) {
                QTableWidgetItem *titem = uid.tableWidget->item(i, 0);
                QTableWidgetItem *titemo = uid.tableWidget->item(i, 1);
                if (!titem || titem->text().isEmpty()) {
                    break;
                } else {
                    if (tempy == uid.xaxis->text()) {
                        tempyval = uid.xaxis->text();
                        tempy = "";
                    }
                    if (titem->data(Qt::DisplayRole).toString() == tempy) {
                        QString yvaluerq = titemo->data(Qt::DisplayRole).toString();
                        QByteArray ba = yvaluerq.toLatin1();
                        char *yvalure = ba.data();
                        tempyval = QChar('(') + QString(yvalure) + QChar(')');
                        tempy = "";
                        end = 1;
                    }
                    if (tempy!=uid.xaxis->text()) {
                        if (yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='^' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',') {
                            //actually nothing
                        } else {
                            end = 0;
                        }
                    }
                }
            }
        } // simbol end
        if (!tempyval.isEmpty()) {
            mreport = mreport + tempyval;
        }
        tempyval = "";
    }
    return mreport;

}

QString titrationCalculator::solvex(char *yvalue, QString dnum) {

    QString mreport = "";
    lettere = 0;
    //now we have to solve the system of equations
    //yvalue contains the equation of Y-axis variable
    //Remember that the function to elevate to power is Math.pow(b,e)
    QString tempy;
    QString tempyold;
    QString tempyolda = "";
    int olda = 0;
    end = 1;
    QString tempyval;
    tempy = "";
    for (int i = 0; strlen(yvalue) + 1; ++i) {
        if (!(yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='^' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',')) {
            break; //if current value is not a permitted value, this means that something is wrong
        }
        if (yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M' or yvalue[i]=='.' or yvalue[i]==',') {
            tempy = tempy + yvalue[i]; //if lettere == 0 then the equation contains only mnumbers
        }
        if (yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='^' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',') {
            if (!tempyolda.isEmpty()) {
                tempy = tempy + yvalue[i];
                if (tempyolda == uid.xaxis->text()) {
                    tempyolda = dnum;
                }
                tempyval = tempyval
                           + QString("Math.pow(")
                           + tempyolda + QChar(',')
                           + tempy + QChar(')');
                tempyolda = "";
                tempyold = "";
                olda = 1;
            }
            if (yvalue[i] == '^') {
                tempyolda = tempyold;
            } else {
                tempyold = "";
                if (((olda != 1) and (yvalue[i + 1] != '^')) or (yvalue[i] == '+' or yvalue[i] == '-' or yvalue[i] == '^' or yvalue[i] == '*' or yvalue[i] == '/' or yvalue[i] == '(' or yvalue[i] == ')')) {
                    tempyval = tempyval + QString(yvalue[i]);
                }
            }

        } else {
            if (!tempyolda.isEmpty()) {
                tempyval = tempyval
                           + QString("Math.pow(")
                           + tempyolda + QChar(',')
                           + tempy + QChar(')');
                tempyolda = "";
                tempyold = "";
                olda = 1;
            }
            if ((tempy==uid.xaxis->text()) and (!tempyolda.isEmpty())) {
                if (yvalue[i + 1] != '^') {
                    tempyval = tempyval + dnum;
                }
                tempyold = tempy;
                tempy = "";
            }
        } // simbol end
        if (!tempyval.isEmpty()) {
            mreport = mreport + tempyval;
        }
        tempyval = "";
    }
    //QMessageBox::information(this, "report", mreport);
    return mreport;
}

void titrationCalculator::on_xmin_valueChanged(double val)
{
    xmin = val;
    on_pushButton_clicked(); //please take note that calling directly the plot() function will give a wrong value for equivalence point
}

void titrationCalculator::on_xmax_valueChanged(double val)
{
    xmax = val;
    on_pushButton_clicked(); //please take note that calling directly the plot() function will give a wrong value for equivalence point
}

void titrationCalculator::on_ymin_valueChanged(double val)
{
    ymin = val;
    on_pushButton_clicked(); //please take note that calling directly the plot() function will give a wrong value for equivalence point
}

void titrationCalculator::on_ymax_valueChanged(double val)
{
    ymax = val;
    on_pushButton_clicked(); //please take note that calling directly the plot() function will give a wrong value for equivalence point
}

void titrationCalculator::on_pushButton_clicked()
{
    plot();
}



void titrationCalculator::on_actionRapid_Help_triggered()
{
    on_actionNew_triggered();
    //now I'm going to fill the tables with the example values

    //table1
    QTableWidgetItem *titemo = uid.tableWidget->item(0,0);
    titemo->setText("A");
    titemo = uid.tableWidget->item(0,1);
    titemo->setText("(C*D)/(B*K)");
    titemo = uid.tableWidget->item(1,0);
    titemo->setText("K");
    titemo = uid.tableWidget->item(1,1);
    titemo->setText("10^-3");
    titemo = uid.tableWidget->item(2,0);
    titemo->setText("C");
    titemo = uid.tableWidget->item(2,1);
    titemo->setText("OH");
    titemo = uid.tableWidget->item(3,0);
    titemo->setText("OH");
    titemo = uid.tableWidget->item(3,1);
    titemo->setText("(10^-14)/H");
    titemo = uid.tableWidget->item(4,0);
    titemo->setText("H");
    titemo = uid.tableWidget->item(4,1);
    titemo->setText("10^-4");
    titemo = uid.tableWidget->item(5,0);
    titemo->setText("B");
    titemo = uid.tableWidget->item(5,1);
    titemo->setText("6*(10^-2)");
    //xaxis
    uid.xaxis->setText("D");
    //yaxis
    uid.yaxis->setText("A");
    //table2
    titemo = uid.tableWidget_2->item(0,0);
    titemo->setText("7,19");
    titemo = uid.tableWidget_2->item(0,1);
    titemo->setText("30");
    titemo = uid.tableWidget_2->item(1,0);
    titemo->setText("7,64");
    titemo = uid.tableWidget_2->item(1,1);
    titemo->setText("30,5");
    titemo = uid.tableWidget_2->item(2,0);
    titemo->setText("10,02");
    titemo = uid.tableWidget_2->item(2,1);
    titemo->setText("31");
    titemo = uid.tableWidget_2->item(3,0);
    titemo->setText("10,45");
    titemo = uid.tableWidget_2->item(3,1);
    titemo->setText("31,5");

    // I think it's better if I don't give so much information here.
    //  This information could be included into kalzium help, but I don't know how to do
    //QMessageBox::information(this, "IceeQt Rapid Help", "There are two ways to use IceeQt:\n\nTheoretical Equations\n Here you can fill the table with the equations you have previously obtained for the chemical equilibria. FOR EXAMPLE if you have this reaction A + B -> C + D then you will have the equation K=(C*D)/(A*B) so you must write 'K' in the Parameter column and '(C*D)/(A*B)' in the Value column. If you want to assign a known value to a parameter you can simply write the numeric value in the Value field. FOR EXAMPLE you can use the system \nA=(C*D)/(B*K) \nK=10^-3 \nC=OH \nOH=(10^-14)/H \nH=10^-4 \nB=6*(10^-2) \nThen you have to write D as X axis and A as Y axis: so you will find out how the concentration of A change in function of D concentration.\nPlease don't use parenthesis for exponents: 10^-3 is correct, while 10^(-3) is wrong. \n\nExperimental Values\n You can use this program to draw the plot of your experimental data obtained during a titration and find out the volume of equivalence. It's strongly recommended to insert a even number of points, because of the best fit algorithm, sorted by volume (the X axis value).\n\nPlot\n The plot shows in red the curve that cames from theorical equations, in blue the experimental points, and in green the aproximated curve for experimental points.");
 }

/*
void titrationCalculator::on_actionAbout_triggered()
{
    QMessageBox::information(this, "IceeQt About", "I\nCompute\nEquilibria\nExactly\n\nIceeQt is a program for computing chemical equilibria in a easy way. The first version of Icee was written by Gabriele Balducci(University of Trieste, Italy) using matheval, gnuplot, and tk. This version, called IceeQt, was written by Luca Tringali using Qtopia. \n IceeQt is installable on every system supported by Qt: Windows, MacOS, GNU/Linux, FreeBSD, Solaris, Symbian, etc..\n This program is released under GPL3 licence.\n\nThe website is http://www.dsch.units.it/~balducci/lca1/");
}
*/
void titrationCalculator::on_actionNew_triggered()
{
    //set all the table cells as empty ("")
    for (int i = 0; i < uid.tableWidget->rowCount(); ++i) {
        QTableWidgetItem *titem = new QTableWidgetItem;
        titem->setText("");
        uid.tableWidget->setItem(i, 0, titem);
        QTableWidgetItem *titemo = new QTableWidgetItem;
        titemo->setText("");
        uid.tableWidget->setItem(i, 1, titemo);
    }
    uid.xaxis->setText("");
    uid.yaxis->setText("");
    for (int i = 0; i < uid.tableWidget_2->rowCount(); ++i) {
        QTableWidgetItem *titem = new QTableWidgetItem;
        titem->setText("");
        uid.tableWidget_2->setItem(i, 0, titem);
        QTableWidgetItem *titemo = new QTableWidgetItem;
        titemo->setText("");
        uid.tableWidget_2->setItem(i, 1, titemo);
    }
    uid.note->setText("");
}

void titrationCalculator::on_actionSave_triggered()
{
    //save all the cells values
    //  if we have for example:
    //  table1:
    //  |a|f|
    //  |d|h|
    //  table2:
    //  |w|q|
    //  |h|l|
    //  then the file would be:
    //  table1|
    //  a|
    //  f|
    //  d|
    //  h|
    //  table2|
    //  w|
    //  q|
    //  h|
    //  l|
    //  note|
    //   ewewewww|
    //   as you can see we don't save also the empty cells, this is obious.

    QString tempyval;
    tempyval = "table1|";
    for (int i = 0; i < uid.tableWidget->rowCount(); ++i) {
        QTableWidgetItem *titem = uid.tableWidget->item(i, 0);
        QTableWidgetItem *titemo = uid.tableWidget->item(i, 1);
        if (!titem || titem->text().isEmpty()) {
            break;
        } else {
            QString yvaluerq = titemo->data(Qt::DisplayRole).toString();
            QString valuerq = titem->data(Qt::DisplayRole).toString();
            tempyval = tempyval + QChar('\n') + valuerq + QString("|\n") + yvaluerq + QChar('|');
        }
    }
    tempyval =  tempyval + QString("\nxaxis|");
    tempyval = tempyval + QString("\n") + uid.xaxis->text() + QChar('|');
    tempyval =  tempyval + QString("\nyaxis|");
    tempyval = tempyval + QString("\n") + uid.yaxis->text() + QChar('|');
    tempyval =  tempyval + QString("\ntable2|");
    for (int i = 0; i < uid.tableWidget_2->rowCount(); ++i) {
        QTableWidgetItem *titem = uid.tableWidget_2->item(i, 0);
        QTableWidgetItem *titemo = uid.tableWidget_2->item(i, 1);
        if (!titem || titem->text().isEmpty()) {
            break;
        } else {
            QString yvaluerq = titemo->data(Qt::DisplayRole).toString();
            QString valuerq = titem->data(Qt::DisplayRole).toString();
            tempyval = tempyval + QChar('\n') + valuerq + QString("|\n") + yvaluerq + QChar('|');
        }
    }
    tempyval = tempyval + QString("\nnote|\n") + uid.note->toPlainText() + QChar('|');

    QString file = QFileDialog::getSaveFileName(this,i18n("Save work"),"",i18n("Icee File (*.icee)"));
    if (!file.isEmpty()) {
        QByteArray ba = tempyval.toLatin1();
        char *strsave = ba.data();
        QByteArray bac = file.toLatin1();
        char *filec = bac.data();

        ofstream out(filec);
        cout << "|";
        cout << filec;
        cout << "|";
        if (!out) {
            QMessageBox::critical(this,i18n("Error"),i18n("Unable to create %1", file));
        }
        out << strsave;
        out.close();
        //if (out) QMessageBox::information(this, "Information", "File " + file + " successfully saved.");
    }
}

void titrationCalculator::on_actionOpen_triggered()
{
    //loads all the cells text from a file prevoiusly saved
    QString file = QFileDialog::getOpenFileName(this,i18n("Open work"),"",i18n("Icee File (*.icee)"));
    if (!file.isEmpty()) {
        QByteArray bac = file.toLatin1();
        char *filec = bac.data();
        ifstream texto(filec);
        if (!texto) {
            QMessageBox::critical(this,i18n("Error"),i18n("Unable to open %1", file));
        }
        if (texto) {
            on_actionNew_triggered();
            QString tempyval;
            char tmpchr;
            int i = 0;
            int tablea = 0;
            int tableb = 0;
            int xax = 0;
            int yax = 0;
            int notea = 0;
            do {
                texto >> tmpchr;
                if (tmpchr != '|') {
                    tempyval = tempyval + tmpchr;
                } else {
                    if ((tablea == 1) and (tempyval != QString("table1")) and (tempyval != QString("table2")) and (tempyval != QString("xaxis")) and (tempyval != QString("yaxis")) and (tempyval != QString("note"))) {
                        if ((i % 2) != 0) {
                            QTableWidgetItem *titemo = uid.tableWidget->item((i - 1) / 2, 1);
                            if (titemo) {
                                titemo->setText(tempyval);
                            }
                        } else {
                            QTableWidgetItem *titem = uid.tableWidget->item((i / 2), 0);
                            if (titem) {
                                titem->setText(tempyval);
                            }
                        }
                        ++i;
                    }

                    if ((tableb == 1) and (tempyval != QString("table1")) and (tempyval != QString("table2")) and (tempyval != QString("xaxis")) and (tempyval != QString("yaxis")) and (tempyval != QString("note"))) {
                        if ((i % 2) != 0) {
                            QTableWidgetItem *titemo = uid.tableWidget_2->item((i - 1) / 2, 1);
                            if (titemo) {
                                titemo->setText(tempyval);
                            }
                        } else {
                            QTableWidgetItem *titem = uid.tableWidget_2->item((i / 2), 0);
                            if (titem) {
                                titem->setText(tempyval);
                            }
                            //cout << i;
                        }
                        ++i;
                    }
                    if ((xax == 1) and (tempyval != QString("table1")) and (tempyval != QString("table2")) and (tempyval != QString("xaxis")) and (tempyval != QString("yaxis")) and (tempyval != QString("note"))) {
                        uid.xaxis->setText(tempyval);
                    }
                    if ((yax == 1) and (tempyval != QString("table1")) and (tempyval != QString("table2")) and (tempyval != QString("xaxis")) and (tempyval != QString("yaxis")) and (tempyval != QString("note"))) {
                        uid.yaxis->setText(tempyval);
                    }
                    if ((notea == 1) and (tempyval != QString("table1")) and (tempyval != QString("table2")) and (tempyval != QString("xaxis")) and (tempyval != QString("yaxis")) and (tempyval != QString("note"))) {
                        uid.note->setText(tempyval);
                    }

                    if (tempyval == QString("table1")) {
                        i = 0;
                        tablea = 1;
                        tableb = 0;
                        xax = 0;
                        yax = 0;
                        notea = 0;
                    }
                    if (tempyval == QString("table2")) {
                        i = 0;
                        tablea = 0;
                        tableb = 1;
                        xax = 0;
                        yax = 0;
                        notea = 0;
                    }
                    if (tempyval == QString("xaxis"))  {
                        tablea = 0;
                        tableb = 0;
                        xax = 1;
                        yax = 0;
                        notea = 0;
                    }
                    if (tempyval == QString("yaxis"))  {
                        tablea = 0;
                        tableb = 0;
                        xax = 0;
                        yax = 1;
                        notea = 0;
                    }
                    if (tempyval == QString("note"))  {
                        tablea = 0;
                        tableb = 0;
                        xax = 0;
                        yax = 0;
                        notea = 1;
                    }
                    tempyval = "";
                }
            } while (!texto.eof());
            texto.close();
        }
    }
}

void titrationCalculator::on_actionSave_image_triggered()
{
    //This function saves the plot into a SVG file
    QString svgheader = "<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"no\"?> <!DOCTYPE svg PUBLIC \"-//W3C//Dtd SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/Dtd/svg11.dtd\"> <svg width=\"" + QString::number((xmax * 10) + 5) + "\" height=\"" + QString::number((ymax * 10) + 5) + "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\"><polyline points=\"5," + QString::number(ymax * 10) + " " + QString::number((xmax * 10) - 5) + "," + QString::number(ymax * 10) + " " + QString::number((xmax * 10) - 5) + "," + QString::number((ymax * 10) - 5) + " " + QString::number(xmax * 10) + "," + QString::number(ymax * 10) + " " + QString::number((xmax * 10) - 5) + "," + QString::number((ymax * 10) + 5) + " " + QString::number((xmax * 10) - 5) + "," + QString::number(ymax * 10) + "\" style=\"stroke:black;fill:none\"/> <polyline points=\"5," + QString::number(ymax * 10) + " 5,5 10,5 5,0 0,5 5,5\" style=\"stroke:black;fill:none\"/> ";
    QString svgcomplete = svgheader + redplot + greenplot + blueplot + "</svg> ";

    QString file = QFileDialog::getSaveFileName(this, i18n("Save plot"), "", i18n("Svg image (*.svg)"));
    if (!file.isEmpty()) {
        QByteArray svgt = svgcomplete.toLatin1();
        char *strsave = svgt.data();
        QByteArray ban = file.toLatin1();
        char *filec = ban.data();

        ofstream out(filec);
        cout << "|";
        cout << filec;
        cout << "|";
        if (!out) {
            QMessageBox::critical(this,i18n("Error"),i18n("Unable to create %1", file));
        }
        out << strsave;
        out.close();
    }
}

//#include "titrationCalculator.moc"
//#include "moc_titrationCalculator.cpp"
