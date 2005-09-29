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

#include <qtimer.h>
#include <qpainter.h>
#include <qcolor.h>

#include <kdebug.h>

#include "eqresult.h"

// inspired by speedcrunch

QuestionItem::QuestionItem( QListBox* listBox, const QString& e, QColor bg ):
QListBoxItem ( listBox )
{
    m_msg = e;
    m_bgcolor = bg;
}

AnswerItem::AnswerItem( QListBox* listBox, const QString& e, const QString &r, QColor bg ):
QListBoxItem ( listBox )
{
    m_msg = "<p align=\"right\">"+r+"</p>";
    m_origmsg = e;
    m_bgcolor = bg;
    m_richtext = new QSimpleRichText(m_msg, listBox->font());
    m_richtext->setWidth( listBox->width() );
}

void QuestionItem::paint( QPainter* painter )
{
    int tf = Qt::NoAccel | Qt::SingleLine | Qt::AlignVCenter;

    QRect r ( 0, 0, listBox()->contentsWidth(), height( listBox() ) );
    painter->fillRect( r, m_bgcolor );
    painter->drawText( r, tf, m_msg );
}

void AnswerItem::paint( QPainter* painter )
{
    checkSize();

    QRect r ( 0, 0, listBox()->contentsWidth(), height( listBox() ) );
    painter->fillRect( r, m_bgcolor );

    QColorGroup cg = listBox()->colorGroup();
    cg.setColor( QColorGroup::Background, m_bgcolor );

    m_richtext->draw(painter, 0, 0, r, cg );
}

int QuestionItem::width( const QListBox*lb ) const
{
    QFont font = lb->font();
    return QFontMetrics( font ).width( m_msg );
}

int QuestionItem::height( const QListBox*lb ) const
{
    QFont font = lb->font();
    return QFontMetrics( font ).height() + 4;
}

void AnswerItem::checkSize()
{
    QFont font = listBox()->font();
    int t1 = QFontMetrics( font ).width( m_origmsg+"padding" );
    int t2 = listBox()->width() - 20;

    // take the maximum
    m_richtext->setWidth(   (t1>t2)?t1:t2   );
}

EqResult::EqResult(QWidget *parent) : QListBox(parent)
{
    m_alternate_color = false;
    setMinimumWidth(140);
}

EqResult::~EqResult()
{
}

void EqResult::add(const QString & question, const QString & answer)
{
    QColor bgcolor = colorGroup().base();

    if ( m_alternate_color )
	bgcolor = bgcolor.dark( 120 );

    m_alternate_color = !m_alternate_color;

    new QuestionItem( this, question, bgcolor );
    new AnswerItem( this, question, answer, bgcolor );

    QTimer::singleShot( 100, this, SLOT( scrollToEnd() ) );
}

void EqResult::scrollToEnd()
{
    ensureVisible( 0, contentsHeight()-1 );
}

void EqResult::resizeEvent( QResizeEvent* )
{
    triggerUpdate( true );
}

#include "eqresult.moc"

