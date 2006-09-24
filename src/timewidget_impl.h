#ifndef TIMEWIDGET_IMPL_H
#define TIMEWIDGET_IMPL_H
/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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

#include <QWidget>
#include "ui_timewidget.h"

class Element;

/**
 * @author Carsten Niehaus
 */
class TimeWidgetImpl : public QWidget, public Ui_TimeWidget
{
    Q_OBJECT

    public:
        /**
         * @param parent The parent of this widget
         */
        TimeWidgetImpl( QWidget *parent = 0 );

    private:
        QList<Element*> m_list;
        QString m_htmlBegin;
        QString m_htmlEnd;

    private slots:
        void setNewTime( int newtime );
};
#endif // TIMEWIDGET_IMPL_H
