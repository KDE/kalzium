/***************************************************************************
 *   Copyright (C) 2003, 2004 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "infodialog_small_impl.h"

#include <kdebug.h>
#include <klocale.h>
#include <qstring.h>
#include <qlabel.h>

infoDlgSmallImpl::infoDlgSmallImpl(Element *el, QWidget *parent, const char *name)
 : infoDlgSmall(parent, name)
{
	e = el;

	setValues();
}

void infoDlgSmallImpl::setValues()
{
	QString num = QString::number( e->number() );
	QString name = e->elname().utf8();
	QString cap = i18n( "%1 (%2)" ).arg( name ).arg( num );
	setCaption( cap );
	symbol_label->setText( e->symbol() );
	name_label->setText( name.utf8() );
	weight_label->setText(  Element::adjustUnits( e->weight(),3 ) );
	elemno_label->setText(  QString::number( e->number() ) );
	melting_label->setText( Element::adjustUnits( e->melting(),0 ) );
	boiling_label->setText( Element::adjustUnits( e->boiling(),0 ) );
	radius_label->setText(  Element::adjustUnits( e->radius(),2 ) );
	electro_label->setText( Element::adjustUnits( e->electroneg(),6 ) );

	setupHelp();
}

void infoDlgSmallImpl::setupHelp()
{}

infoDlgSmallImpl::~infoDlgSmallImpl(){}

#include "infodialog_small_impl.moc"
