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

class KMolUI;

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
        CalcDlg(QWidget *parent, const char *name=0 );

    private:
        KMolCalc *kmolcalc;
        KMolUI *KMol;
        SymLabel symlabel[118];
        SymLabel weight[118];
	
    private slots:
        /**  
        * Make a call to KMOLCALC to parse the string in FORMULA, 
        * if any, and get Mw and EA back.
        */
        void calc();

         /**
        * Call the built in editor to edit the definitions file.
        */
        void callEditor();

        /** 
        * Clear all text entry / result fields.
        */
        void clear();
};

#endif
