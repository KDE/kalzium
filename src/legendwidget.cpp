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
#include <qcolor.h>

#include <klocale.h>

#include "prefs.h"
#include "legendwidget.h"

Legend::Legend( QWidget* parent )
	: QWidget( parent )
{
	
	QHBoxLayout *hlay = new QHBoxLayout( this );
	hlay->setAutoAdd( TRUE );
	hlay->setMargin(10);
		
	one = new QLabel( this );
	two = new QLabel( this );
	three = new QLabel( this );
	four = new QLabel( this );
	five = new QLabel( this );
	six = new QLabel( this );
	seven = new QLabel( this );
	eight = new QLabel( this );
}

void Legend::setScheme( int scheme )
{
	const QColor color_1 = Prefs::group_1();
	const QColor color_2 = Prefs::group_2();
	const QColor color_3 = Prefs::group_3();
	const QColor color_4 = Prefs::group_4();
	const QColor color_5 = Prefs::group_5();
	const QColor color_6 = Prefs::group_6();
	const QColor color_7 = Prefs::group_7();
	const QColor color_8 = Prefs::group_8();

	const QColor color_s = Prefs::block_s();
	const QColor color_p = Prefs::block_p();
	const QColor color_d = Prefs::block_d();
	const QColor color_f = Prefs::block_f();
		
	const QColor color_ba = Prefs::beh_basic();
	const QColor color_ac = Prefs::beh_acidic();
	const QColor color_neu = Prefs::beh_neutral();
	const QColor color_amp = Prefs::beh_amphoteric();

	switch ( scheme ) {
		case 0:
			one->hide();
			two->hide();
			three->hide();
			four->hide();
			five->hide();
			six->hide();
			seven->hide();
			eight->hide();
			break;
		case 1:
			one->show();
			two->show();
			three->show();
			four->show();
			five->show();
			six->show();
			seven->show();
			eight->show();
			one->setText( i18n( "Group 1" ) );
			two->setText( i18n( "Group 2" ) );
			three->setText( i18n( "Group 3" ) );
			four->setText( i18n( "Group 4" ) );
			five->setText( i18n( "Group 5" ) );
			six->setText( i18n( "Group 6" ) );
			seven->setText( i18n( "Group 7" ) );
			eight->setText( i18n( "Group 8" ) );
			one->setPaletteBackgroundColor( color_1 );
			two->setPaletteBackgroundColor( color_2 );
			three->setPaletteBackgroundColor( color_3 );
			four->setPaletteBackgroundColor( color_4 );
			five->setPaletteBackgroundColor( color_5 );
			six->setPaletteBackgroundColor( color_6 );
			seven->setPaletteBackgroundColor( color_7 );
			eight->setPaletteBackgroundColor( color_8 );
			break;
		case 2:
			one->setPaletteBackgroundColor( color_s );
			two->setPaletteBackgroundColor( color_p );
			three->setPaletteBackgroundColor( color_d );
			four->setPaletteBackgroundColor( color_f );
			one->setText( i18n( "s-Block" ) );
			two->setText( i18n( "p-Block" ) );
			three->setText( i18n( "d-Block" ) );
			four->setText( i18n( "f-Block" ) );
			one->show();
			two->show();
			three->show();
			four->show();
			five->hide();
			six->hide();
			seven->hide();
			eight->hide();
			break;
		case 3:
			one->setPaletteBackgroundColor( color_ba );
			two->setPaletteBackgroundColor( color_ac );
			three->setPaletteBackgroundColor( color_neu );
			four->setPaletteBackgroundColor( color_amp );
			one->setText( i18n( "Basic-Block" ) );
			two->setText( i18n( "Acidic" ) );
			three->setText( i18n( "Neutral" ) );
			four->setText( i18n( "Amphoteric" ) );
			one->show();
			two->show();
			three->show();
			four->show();
			five->hide();
			six->hide();
			seven->hide();
			eight->hide();
			break;
	}
}

#include "legendwidget.moc"
