/***************************************************************************
 *   copyright (C) 2006 by Carsten Niehaus  cniehaus@kde.org               *
 *   Copyright (C) 2010 by Etienne Rebetez  etienne.rebetez@oberwallis.ch  *
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
#ifndef GRADIENTWIDGET_IMPL_H
#define GRADIENTWIDGET_IMPL_H

#include <QWidget>
#include "ui_gradientwidget.h"

#include "kalziumelementproperty.h"

class Element;
class QTimer;

/**
 * @class GradientWidgetImpl

 * @short Provides a widget for setting the appearance of the pse table
 * @author Carsten Niehaus
 * @author Kashyap Puranik
 * @author Etienne Rebetez
 */
class GradientWidgetImpl : public QWidget, public Ui_gradientWidget
{
    Q_OBJECT

public:
    /**
     * @param elementProperty The elementProperty class
     * @param parent The parent of this widget
     */
    GradientWidgetImpl(QWidget *parent = nullptr);
    virtual ~GradientWidgetImpl();

Q_SIGNALS:
    /**
     * Is emitted when the value of the gradient or the spinbox is changed.
     */
    void gradientValueChanged(double);

public slots:
    /**
     * Sets the comboboxes to the current values.
     * the current values come form the elementProperty class.
     */
    void slotGradientChanged();

private slots:
    void play(void);
    void stop(void);
    void tick(void);
    void doubleToSlider(double var);
    void intToSpinbox(int var);
    /**
     * is used to display custom text in the widget according to the current value of the gradient
     */
    void setNewValue(double newValue);

private:
    bool m_play;        //Indicates whether mode is play or stop
    QTimer *m_timer;
};
#endif // GRADIENTWIDGET_IMPL_H
