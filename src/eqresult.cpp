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

#include <tqtimer.h>
#include <tqpainter.h>
#include <tqcolor.h>

#include <kdebug.h>

#include "eqresult.h"

// inspired by speedcrunch

QuestionItem::QuestionItem( TQListBox* listBox, const TQString& e, TQColor bg ):
TQListBoxItem ( listBox )
{
    m_msg = e;
    m_bgcolor = bg;
}

AnswerItem::AnswerItem( TQListBox* listBox, const TQString& e, const TQString &r, TQColor bg ):
TQListBoxItem ( listBox )
{
    m_msg = "<p align=\"right\">"+r+"</p>";
    m_origmsg = e;
    m_bgcolor = bg;
    m_richtext = new TQSimpleRichText(m_msg, listBox->font());
    m_richtext->setWidth( listBox->width() );
}

void QuestionItem::paint( TQPainter* painter )
{
    int tf = Qt::NoAccel | Qt::SingleLine | Qt::AlignVCenter;

    TQRect r ( 0, 0, listBox()->contentsWidth(), height( listBox() ) );
    painter->fillRect( r, m_bgcolor );
    painter->drawText( r, tf, m_msg );
}

void AnswerItem::paint( TQPainter* painter )
{
    checkSize();

    TQRect r ( 0, 0, listBox()->contentsWidth(), height( listBox() ) );
    painter->fillRect( r, m_bgcolor );

    TQColorGroup cg = listBox()->colorGroup();
    cg.setColor( TQColorGroup::Background, m_bgcolor );

    m_richtext->draw(painter, 0, 0, r, cg );
}

int QuestionItem::width( const TQListBox*lb ) const
{
    TQFont font = lb->font();
    return TQFontMetrics( font ).width( m_msg );
}

int QuestionItem::height( const TQListBox*lb ) const
{
    TQFont font = lb->font();
    return TQFontMetrics( font ).height() + 4;
}

void AnswerItem::checkSize()
{
    TQFont font = listBox()->font();
    int t1 = TQFontMetrics( font ).width( m_origmsg+"padding" );
    int t2 = listBox()->width() - 20;

    // take the maximum
    m_richtext->setWidth(   (t1>t2)?t1:t2   );
}

EqResult::EqResult(TQWidget *parent) : TQListBox(parent)
{
    m_alternate_color = false;
    setMinimumWidth(140);
}

EqResult::~EqResult()
{
}

void EqResult::add(const TQString & question, const TQString & answer)
{
    TQColor bgcolor = colorGroup().base();

    if ( m_alternate_color )
	bgcolor = bgcolor.dark( 120 );

    m_alternate_color = !m_alternate_color;

    new QuestionItem( this, question, bgcolor );
    new AnswerItem( this, question, answer, bgcolor );

    TQTimer::singleShot( 100, this, TQT_SLOT( scrollToEnd() ) );
}

void EqResult::scrollToEnd()
{
    ensureVisible( 0, contentsHeight()-1 );
}

void EqResult::resizeEvent( TQResizeEvent* )
{
    triggerUpdate( true );
}

#include "eqresult.moc"

