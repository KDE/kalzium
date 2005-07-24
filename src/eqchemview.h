/***************************************************************************
 *   Copyright (C) 2004 by Thomas Nagy                                     *
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

#ifndef _EQCHEMVIEW_H_
#define _EQCHEMVIEW_H_

#include <qwidget.h>

class QPainter;
class KURL;
class EqResult;

class KLineEdit;
class KPushButton;

/**
 * This is the main view class for eqchem.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Thomas Nagy <tnagy2^8\@yahoo.fr>
 * @version 0.1
 */
class eqchemView : public QWidget
{
    Q_OBJECT
public:
	/**
	 * Default constructor
	 */
    eqchemView(QWidget *parent);

	/**
	 * Destructor
	 */
    virtual ~eqchemView();

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

public slots:
    void clear();
    void compute();

    
private slots:
    void settingsChanged();

private:
    KLineEdit * m_eqedit;
    KPushButton * m_eqclear;
    EqResult * m_eqResult;
};

#endif // _EQCHEMVIEW_H_
