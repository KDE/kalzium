/***************************************************************************
                          stateofmatterdlg.h  -  description
                             -------------------
    begin                : Wed Feb 06 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#ifndef STATEOFMATTERDLG_H
#define STATEOFMATTERDLG_H

//KDE-Includes
class KConfig;
class KDialog;
class KPushButton;

//Qt-Includes
class QColor;
class QLabel;
class QLCDNumber;
class QSlider;
class QWidget;


class Kalzium;

class StateOfMatterDlg : public KDialog  {
    Q_OBJECT

    public: 
	/** construtor */
	StateOfMatterDlg(QWidget *parent, const char *name, Kalzium *kalzium_tmp);
	Kalzium *kalzium;
    private:
	QLCDNumber *celsiusLCD, *fahrenheitLCD, *kelvinLCD;
	QSlider *tempS;
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
