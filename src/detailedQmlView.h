/***************************************************************************
 *   Copyright (C) 2011   by Etienne Rebetez etienne.rebetez@oberwallis.ch *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef DETAILEDQMLVIEW_H
#define DETAILEDQMLVIEW_H

#include <QWidget>
#include <QDeclarativeContext>

#include "kalziumelementproperty.h"
#include "element.h"

class DetailedQmlView : public QWidget
{
    Q_OBJECT
public:

    DetailedQmlView(QWidget *parent);
    ~DetailedQmlView();

public slots:
    /**
     * Set @p el as the element to be drawn
     */
    void setElement(int el);

    void itemPressed();

private:

    KalziumElementProperty *m_elementProperty;
    Element *m_element;

    QDeclarativeContext *m_context;

protected:
    /**
     * Generic event handler, currently defaults to calling parent class
     * (included for future compatibility)
     */
//     bool event(QEvent *e);

    /**
     * is caled every time the view is resized.
     */
    void resizeEvent(QResizeEvent * event);
};

#endif //DETAILEDQMLVIEW_H
