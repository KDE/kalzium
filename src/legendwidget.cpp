/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
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

#include <qlayout.h>
#include <qlabel.h>

#include <klocale.h>

#include "prefs.h"
#include "legendwidget.h"

Legend::Legend( QWidget* parent )
	: QWidget( parent )
{
	const QColor color_1 = Prefs::group_1();
	const QColor color_2 = Prefs::group_2();
	const QColor color_3 = Prefs::group_3();
	const QColor color_4 = Prefs::group_4();
	const QColor color_5 = Prefs::group_5();
	const QColor color_6 = Prefs::group_6();
	const QColor color_7 = Prefs::group_7();
	const QColor color_8 = Prefs::group_8();
	const QColor c_liquid = Prefs::color_liquid();
	const QColor c_solid = Prefs::color_solid();
	const QColor c_vapor = Prefs::color_vapor();
	const QColor c_artificial = Prefs::color_artificial();
	const QColor c_radioactive = Prefs::color_radioactive();

	//just for testing
	setScheme( 2 );
}

void Legend::setScheme( int scheme )
{
	switch ( scheme )
	{
 		case 2: //blocks
				const QColor color_s = Prefs::block_s();
				const QColor color_p = Prefs::block_p();
				const QColor color_d = Prefs::block_d();
				const QColor color_f = Prefs::block_f();
				QVBoxLayout vlay;
				QLabel *s = new QLabel( i18n( "s-Block" ), this );
				QLabel *p = new QLabel( i18n( "p-Block" ), this );
				QLabel *d = new QLabel( i18n( "d-Block" ), this );
				QLabel *f = new QLabel( i18n( "f-Block" ), this );
				s->setPaletteBackgroundColor( color_s );
				p->setPaletteBackgroundColor( color_p );
				d->setPaletteBackgroundColor( color_d );
				f->setPaletteBackgroundColor( color_f );
				vlay.addWidget( s );
				vlay.addWidget( p );
				vlay.addWidget( d );
				vlay.addWidget( f );
 				break;
	}
}

#include "legendwidget.moc"
