/***************************************************************************
                          calcdlg.h  -  description
                             -------------------
    begin                : Thu Dec 24 2001
    copyright            : (C) 2001, 2002 by Carsten Niehaus
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

#ifndef CALCDLG_H
#define CALCDLG_H

class KPushButton;
class QComboBox;
class QMultiLineEdit;
class QLineEdit;
class QLabel;
class QGroupBox;
class QTabWidget;

#include <qtabwidget.h>

#include "../calculations/kmolcalc.h"
#include "../calculations/kmoledit.h"
#include "../calculations/kmolelements.h"

/*
 * this dialog contains some possibilities with which you
 * can do simple calculations.
*@author Carsten Niehaus
*/

typedef QString SymLabel;


class CalcDlg : public QTabWidget  {
    Q_OBJECT

    public: 
        CalcDlg(QWidget *parent, const char *name );
    private:
        /** construtor */
        SymLabel symlabel[118];
        SymLabel weight[118];

        QWidget *KMol, *MWG;

	//	
	//MWG - BEGIN
	//
	    
	//the grids for the MWG
	QGridLayout *grid, *comboboxgrid;

	QString resultvalue, dimension;

	//the QLabel for the MWG
	QLabel *general, *herefrom, *enter, *coeff, *conc, *resultMWG;
	QLabel *a,*b,*c,*d,*A,*B,*C,*D; 

	//We need QLineEdit in the MWG
	QLineEdit *QLEa, *QLEb, *QLEc, *QLEd, *QLEA, *QLEB, *QLEC, *QLED;

	//some QComboBox
	QComboBox *QCBA, *QCBB, *QCBC, *QCBD;
	
	//finally one KPushButton
	KPushButton *resultbutton;

	//and one QGroupBox for the QLineEdit
	QGroupBox *valuebox;

	//
	// MWG - END
	//
        KMolCalc *kmolcalc;
        QLineEdit *formula;
        QMultiLineEdit *anal_display;
        QLabel *result;

    private slots:
    	
        /**  
         * Calculates the result 
         */
	void slotCalculate();
            
        /**  
         * Make a call to KMOLCALC to parse the string in FORMULA, if any, and get Mw and EA back.
         */
        void calc();

        /** 
         * Clear all text entry / result fields.
         */
        void clear();

        /**
         * Call the built in editor to edit the definitions file.
         */
        void callEditor();
};

#endif
