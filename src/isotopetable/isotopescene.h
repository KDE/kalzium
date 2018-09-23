/***************************************************************************
 *   Copyright (C) 2007, 2008 by Carsten Niehaus <cniehaus@kde.org>        *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef ISOTOPESCENE_H
#define ISOTOPESCENE_H

#include <QGraphicsScene>

class IsotopeItem;

class IsotopeScene : public QGraphicsScene
{
    Q_OBJECT

public:
    IsotopeScene(QObject * parent = nullptr);
    virtual ~IsotopeScene();

    void updateContextHelp(IsotopeItem * item);

private:
    void drawIsotopes(void);

    //the size of each item
    int m_itemSize;

    ///this group stores all IsotopeItems
    QGraphicsItemGroup *m_isotopeGroup;

signals:
    void itemSelected(IsotopeItem *item);
};

#endif // ISOTOPESCENE_H
