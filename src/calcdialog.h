/***************************************************************************
                        calcdialog.h  -  description
                             -------------------
    begin                    : Thu Feb 5 2002 
    copyright                : (C) 2002 by Carsten Niehaus  
    email                    : cniehaus@gmx.de
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef HEADER_CALCDIALOG
#define HEADER_CALCDIALOG

#include <kdialog.h>

class CalcDlg;
class QGridLayout;

class CalcDialog : public KDialog
{
    Q_OBJECT
    public:
        CalcDialog (QWidget * parent = 0, const char * name = 0 );

        CalcDlg *tabwidget;
    private:
        QGridLayout *main_layout;
};

#endif
