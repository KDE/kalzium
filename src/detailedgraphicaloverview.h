/***************************************************************************
    copyright            : (C) 2004, 2005, 2006, 2007 by Carsten Niehaus
    email                : cniehaus@kde.org

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DETAILEDGRAPHICALOVERVIEW_H
#define DETAILEDGRAPHICALOVERVIEW_H

#include <QWidget>
#include "kalziumelementproperty.h"

class Element;

/**
 * @brief The widget which displays the most important information
 *
 * In one widget like a lot people know it from school
 *
 * @author Carsten Niehaus
 */
class DetailedGraphicalOverview : public QWidget
{
    Q_OBJECT

public:
    /**
     * Construct a new DetailedGraphicalOverview.
     *
     * @param parent the parent of this widget
     */
    explicit DetailedGraphicalOverview(QWidget *parent);

public slots:
    /**
     * Set @p el as the element to be drawn
     */
    void setElement(int el);

private:
    /**
     * Set the background color to @p bgColor.
     */
    void setBackgroundColor(QColor bgColor);

    /**
     * the element whose data will be used
     */
    Element *m_element;

    /**
     * The background color.
     */
    QBrush m_backgroundBrush;

protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // DETAILEDGRAPHICALOVERVIEW_H
