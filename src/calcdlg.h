/***************************************************************************
                          calcdlg.h  -  description
                             -------------------
    begin                : Thu Dec 24 2002
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

#ifndef CALCDLG_H
#define CALCDLG_H

#include <qwidget.h>
#include <kpushbutton.h>
#include <ksimpleconfig.h>

#include <qwidget.h>
#include <qmultilineedit.h>
#include <qtabwidget.h>
#include <qlabel.h>

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
        /** construtor */
        CalcDlg(QWidget *parent, const char *name );
        QLineEdit *prodconc1, *prodconc2, *prodconc3, *eduktconc1, *eduktconc2, *eduktconc3; 
        QLineEdit *prodcoeff1, *prodcoeff2, *prodcoeff3, *eduktcoeff1, *eduktcoeff2, *eduktcoeff3; 
        SymLabel symlabel[118];
        SymLabel weight[118];

        QWidget *KMol, *MWG;


        double e1, e2, e3, p1, p2, p3; //Edukt/Produktkonzentrationen
        double ecoeff1, ecoeff2, ecoeff3, pcoeff1, pcoeff2, pcoeff3; 
        public slots:
            void slotCalc();
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

    private:
        KMolCalc *kmolcalc;
        QLineEdit *formula;
        QMultiLineEdit *anal_display;
        QLabel *resultMWG, *result;
};

#endif
