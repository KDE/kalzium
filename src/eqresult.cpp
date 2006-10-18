/***************************************************************************
 *   Copyright (C) 2004, 2005, 2006 by Thomas Nagy                               *
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

#include <QTimer>
#include <QPainter>

#include <kdebug.h>

#include "eqresult.h"

// inspired by speedcrunch

QuestionItem::QuestionItem( QListWidget* listBox, const QString& e ):
    QListWidgetItem ( listBox )
{
    m_msg = e;
}

AnswerItem::AnswerItem( QListWidget* listBox, const QString& e, const QString &r ):
    QListWidgetItem ( listBox )
{
    m_msg = "<p align=\"right\">"+r+"</p>";
    m_origmsg = e;
    m_richtext = new Q3SimpleRichText(m_msg, listBox->font());
    m_richtext->setWidth( listBox->width() );
}

void QuestionItem::paint( QPainter* painter )
{
    int tf = Qt::TextHideMnemonic | Qt::TextSingleLine | Qt::AlignVCenter;

    QRect r ( 0, 0, listWidget()->width(), height( listWidget() ) );
    painter->fillRect( r, background() );
    painter->drawText( r, tf, m_msg );
}

void AnswerItem::paint( QPainter* painter )
{
    checkSize();

    QRect r ( 0, 0, listWidget()->width(), height( listWidget() ) );
    painter->fillRect( r, background() );

//X     m_richtext->draw(painter, 0, 0, r, cg );
}

int QuestionItem::width( const QListWidget*lb ) const
{
    QFont font = lb->font();
    return QFontMetrics( font ).width( m_msg );
}

int QuestionItem::height( const QListWidget*lb ) const
{
    QFont font = lb->font();
    return QFontMetrics( font ).height() + 4;
}

void AnswerItem::checkSize()
{
    QFont font = listWidget()->font();
    int t1 = QFontMetrics( font ).width( m_origmsg+"padding" );
    int t2 = listWidget()->width() - 20;

    // take the maximum
    m_richtext->setWidth(   (t1>t2)?t1:t2   );
}

#include "eqresult.moc"

