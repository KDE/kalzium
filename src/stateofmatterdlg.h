/***************************************************************************
                          stateofmatterdlg.h  -  description
                             -------------------
    begin                : Wed Feb 06 2002
    copyright            : (C) 2002 by Carsten Niehaus
    email                : cniehaus@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STATEOFMATTERDLG_H
#define STATEOFMATTERDLG_H

//KDE-Includes
#include <kconfig.h>
#include <kdialog.h>
#include <kpushbutton.h>

//Qt-Includes
#include <qcolor.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qslider.h>
#include <qwidget.h>


class Kalzium;

class StateOfMatterDlg : public KDialog  {
    Q_OBJECT

    public: 
	/** construtor */
	StateOfMatterDlg(QWidget *parent, const char *name, Kalzium *kalzium_tmp);
	QLCDNumber *celsiusLCD, *fahrenheitLCD, *kelvinLCD;
	QSlider *tempS;
	Kalzium *kalzium;
    private:
	QLabel *celsius, *fahrenheit, *kelvin;
	KConfig *main_config;
	KPushButton *solidbutton, *liquidbutton, *vaporbutton, *InputButton, *StdCondButton;
        QColor color_solid, color_vapor, color_liquid;
    public slots:
	void tempbeh();
        void slotSetTemp();
        void slotStdCond();
};

#endif
