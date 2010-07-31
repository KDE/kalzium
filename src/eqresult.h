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

#ifndef _EQRESULT_H_
#define _EQRESULT_H_

#include <tqlistbox.h>
#include <tqsimplerichtext.h>

/**
 * @author Thomas Nagy
 */
class QuestionItem : public QListBoxItem
{
    public:
	QuestionItem( TQListBox*, const TQString&, TQColor bg );
    protected:
	void paint( TQPainter* p );
	int width( const TQListBox* ) const;
	int height( const TQListBox* ) const;
    private:
	TQString m_msg;
	TQColor m_bgcolor;
};

/**
 * @author Thomas Nagy
 */
class AnswerItem : public QListBoxItem
{
    public:
	AnswerItem(  TQListBox*, const TQString&, const TQString&, TQColor bg );
	~AnswerItem() { delete m_richtext; }
	void checkSize();
    protected:
	void paint( TQPainter* p );
	int width( const TQListBox* ) const{
		return m_richtext->height();
	}
	
	int height( const TQListBox* ) const{
		return m_richtext->height();
	}
	
    private:
	TQString m_msg;
	TQString m_origmsg;
	TQColor m_bgcolor;
	TQSimpleRichText * m_richtext;
};

/**
 * @author Thomas Nagy
 */
class EqResult : public QListBox
{
    Q_OBJECT
public:
    
    EqResult(TQWidget *parent);
    ~EqResult();

public slots:
    void add(const TQString &, const TQString &);
    void scrollToEnd();
    void resizeEvent( TQResizeEvent* );

private:
    bool m_alternate_color;
};

#endif // _EQRESULT_H_

