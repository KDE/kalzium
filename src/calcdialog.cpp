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

#include <klocale.h>


CalcDialog::CalcDialog(QWidget * parent, const char * name, bool modal)
    : KDialog (parent, name, modal)
{
    this->setCaption( i18n( "Calculations" ) );
    main_layout = new QGridLayout(this);

    tabwidget = new CalcDlg(this, "tabwidget");

    main_layout->addWidget(tabwidget,0,0);
}
