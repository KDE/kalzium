/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus                                 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/
#include "tableinfowidget.h"
#include "prefs.h"
#include "kalziumschemetype.h"
#include "kalziumpainter.h"
#include "kalziumtabletype.h"

#include <kdebug.h>
#include <klocale.h>
#include <kglobalsettings.h>

#include <QLabel>
#include <QHBoxLayout>

TableInfoWidget::TableInfoWidget( QWidget *parent )
  : QWidget( parent )
{
}

void TableInfoWidget::setTableType( KalziumTableType * type )
{
    m_tableType = type;
    updateContent();
}

void TableInfoWidget::updateContent()
{
    QLabel * l = new QLabel( "test" , this );
    if (m_tableType) {
        l->setText(m_tableType->description() );
    }

    QHBoxLayout * la = new QHBoxLayout(this);
    la->addWidget( l );
    setLayout( la );
}

#include "tableinfowidget.moc"

