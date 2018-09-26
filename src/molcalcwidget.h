/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007 by Carsten Niehaus <cniehaus@kde.org>  *
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

#ifndef MOLCALCWIDGET_H
#define MOLCALCWIDGET_H

#include <QWidget>
#include "ui_molcalcwidgetbase.h"

#include "moleculeparser.h"

class QTimer;
class QKeyEvent;

/**
 * This widget calculates mass for molecules.
 *
 * @author Carsten Niehaus
 * @author Pino Toscano
 * @author Inge Wallin
 * @author Kashyap R Puranik
 */
class MolcalcWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * Constructor
     * @param parent parent widget
     */
    explicit MolcalcWidget(QWidget *parent = nullptr);
    void hideExtra();
    ~MolcalcWidget() override;

protected slots:
    void slotCalculate();

protected:
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void clear();
    void addAlias();

private:
    /**
     * @return the HTML code of an element symbol and its
     * subscripted amount. Eg: Mg<sub>2</sub>
     */
    QString compositionString(ElementCountMap &_map);

    /**
     * This methods gathers all the data and updates the
     * contents of the widget.
     */
    void updateUI();

    Ui_MolcalcWidgetBase ui;

    QTimer * m_timer;

private:
    MoleculeParser   *m_parser;
    QSet<QString>    m_aliasList;
    double           m_mass;
    bool             m_validInput;
    ElementCountMap  m_elementMap;
};

#endif // MOLCALCWIDGET_H
