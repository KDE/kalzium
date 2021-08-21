/*
    SPDX-FileCopyrightText: 2007, 2008 Carsten Niehaus <cniehaus@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ISOTOPESCENE_H
#define ISOTOPESCENE_H

#include <QGraphicsScene>

class IsotopeItem;

class IsotopeScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit IsotopeScene(QObject * parent = nullptr);
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
