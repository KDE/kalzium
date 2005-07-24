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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _EQRESULT_H_
#define _EQRESULT_H_

#include <qlistbox.h>
#include <qsimplerichtext.h>

/**
 * @author Thomas Nagy
 */
class questionitem : public QListBoxItem
{
    public:
	questionitem( QListBox*, const QString&, QColor bg );
    protected:
	void paint( QPainter* p );
	int width( const QListBox* ) const;
	int height( const QListBox* ) const;
    private:
	QString m_msg;
	QColor m_bgcolor;
};

/**
 * @author Thomas Nagy
 */
class answeritem : public QListBoxItem
{
    public:
	answeritem(  QListBox*, const QString&, const QString&, QColor bg );
	~answeritem() { delete m_richtext; }
	void checkSize();
    protected:
	void paint( QPainter* p );
	int width( const QListBox* ) const{
		return m_richtext->height();
	}
	
	int height( const QListBox* ) const{
		return m_richtext->height();
	}
	
    private:
	QString m_msg;
	QString m_origmsg;
	QColor m_bgcolor;
	QSimpleRichText * m_richtext;
};

/**
 * @author Thomas Nagy
 */
class eqresult : public QListBox
{
    Q_OBJECT
public:
    
    eqresult(QWidget *parent);
    ~eqresult();

public slots:
    void add(const QString &, const QString &);
    void scrollToEnd();
    void resizeEvent( QResizeEvent* );

private:
    bool m_alternate_color;
};

#endif // _EQRESULT_H_

