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
#include "element.h"

#include <kdebug.h>
#include <klocale.h>

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
	QString name = i18n( e->elname().utf8() );
	QString cap = i18n( "%1 (%2)" ).arg( name ).arg( num );
	setCaption( cap );
	symbol_label->setText( e->symbol() );
	name_label->setText( name );
	weight_label->setText(  e->adjustUnits( Element::WEIGHT ) );
	elemno_label->setText(  QString::number( e->number() ) );
	melting_label->setText( e->adjustUnits( Element::MELTINGPOINT ) );
	boiling_label->setText( e->adjustUnits( Element::BOILINGPOINT ) );
	radius_label->setText(  e->adjustUnits( Element::RADIUS ) );
	electro_label->setText( QString::number( e->electroneg() ) );

	setupHelp();
}

void infoDlgSmallImpl::setupHelp()
{}

infoDlgSmallImpl::~infoDlgSmallImpl(){}

#include "infodialog_small_impl.moc"
