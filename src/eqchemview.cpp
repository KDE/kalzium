/***************************************************************************
 *   Copyright (C) 2004, 2005 by Thomas Nagy                               *
 *   tnagy2^8@yahoo.fr                                                     *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <qlayout.h>
#include <qlabel.h>
#include <qstring.h>

#include <kdebug.h>
#include <klocale.h>
#include <klineedit.h>
#include <kpushbutton.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kapplication.h>

#include "config.h"
#include "eqresult.h"
#include "eqchemview.h"

#include <stdlib.h>

#ifdef HAVE_FACILE
extern "C" {
    char* solve_equation(const char *);
}
#endif

#ifndef HAVE_FACILE
char* solve_equation(const char *) { return NULL; }
#endif

eqchemView::eqchemView(QWidget *parent) : QWidget(parent)
{
    settingsChanged();

    QGridLayout *l = new QGridLayout(this, 2, 2, 11, 6, "eqchemView::eqchemView layout");

    m_eqResult = new EqResult(this);
    m_eqedit = new KLineEdit(this);
    m_eqclear = new KPushButton(this);

    l->addMultiCellWidget(m_eqResult, 0, 0, 0, 1);
    l->addWidget(m_eqedit, 1, 1);
    l->addWidget(m_eqclear, 1, 0);

    m_eqclear->setIconSet( KGlobal::instance()->iconLoader()->loadIconSet("locationbar_erase", 
		KIcon::NoGroup, 22 /*KIcon::SizeSmallMedium*/) );

    connect(m_eqclear, SIGNAL(clicked()), m_eqedit, SLOT(clear()) );
}

eqchemView::~eqchemView()
{

}

void eqchemView::settingsChanged()
{
    // i18n : internationalization
    emit signalChangeStatusbar( i18n("Settings changed") );
}


void eqchemView::clear()
{
    kdWarning()<<"eqchemView::clear"<<endl;

    // clear the result window
    m_eqResult->clear();
}

void eqchemView::compute()
{
    QString equation( m_eqedit->text() );
    equation.replace("+", "+");
    equation.replace("->", " -> ");
    equation.append(" ");
    equation.prepend(" ");
    
    char * result = solve_equation( equation.latin1() );
    QString disp = QString(result);

    // mem leak ?
    free(result);

    // add the equation in the result window
    m_eqResult->add( equation, disp );
}

EQChemDialog::EQChemDialog( QWidget *parent )
	: KDialogBase(parent, "EQChemDialog", true, i18n( "Solve Chemical Equations" ),
			KDialogBase::Apply|KDialogBase::Close|KDialogBase::Help, KDialogBase::Apply, true )
{
}

void EQChemDialog::slotHelp()
{
	emit helpClicked();
	if ( kapp )
		kapp->invokeHelp ( "eq_solver", "kalzium" );
}

#include "eqchemview.moc"

