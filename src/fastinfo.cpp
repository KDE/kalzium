/***************************************************************************
                      fastinfo.cpp -  description
                             -------------------
    begin                : Sun Nov 3 2002
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


#include <qwidget.h>
#include <qframe.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qfont.h>
#include <qhbox.h>
#include <qvbox.h>

#include <kconfig.h>
#include <ksimpleconfig.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kglobalsettings.h>

#include "fastinfo.h"
#include "elementkp.h"
#include "kalzium.h"
#include "fastinfo.moc"

Fastinfo::Fastinfo(QWidget *parent, const char *name , Kalzium *kalzium_tmp)	: QFrame(parent,name)
{
	kalzium = kalzium_tmp;
	QVBox *vbox = new QVBox ( this );
	elemname = new QLabel( vbox );
	QHBox *hbox = new QHBox( vbox );
    
	QFont bold_font=KGlobalSettings::generalFont();
    bold_font.setBold(true);
    QFont topic_font=bold_font;
    topic_font.setPointSize(bold_font.pointSize()+2);
	
	QLabel *w_label = new QLabel( hbox ); w_label->setText( i18n("Weight:") );
	weight = new QLabel( hbox );
	weight->show();

	QLabel *n_label = new QLabel( hbox ); n_label->setText( i18n("Number:") );
	number = new QLabel( hbox );
	
	elemname->setFont( topic_font );
}

void Fastinfo::setInfo( int ElemNr )
{
	elemname->setText(i18n( kalzium->element[ElemNr-1]->Data.Name.utf8() ));
	weight->setText( i18n( "%1 u" ).arg( kalzium->element[ElemNr-1]->Data.Weight ));
	number->setText( QString::number( ElemNr ) );
}

