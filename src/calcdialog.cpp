/***************************************************************************
                        calcdialog.cpp  -  description
                             -------------------
    begin                    : Thu Feb 5 2002 
    copyright                : (C) 2002 by Carsten Niehaus  
    email                    : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "calcdialog.h"
#include "calcdialog.moc"
#include "calcdlg.h"

#include <qlayout.h>
#include <qobject.h>


CalcDialog::CalcDialog(QWidget * parent, const char * name)
    : KDialog (parent, name)
{
    main_layout = new QGridLayout(this);

    tabwidget = new CalcDlg(this, "tabwidget");

    main_layout->addWidget(tabwidget,0,0);
}
