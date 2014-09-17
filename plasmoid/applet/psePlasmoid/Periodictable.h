/***********************************************************************************
* Mass calculator: Plasmoid to calculate mass of a Molecule.
* Copyright (C) 2009, 2010 Etienne Rebetez, etienne.rebetez@oberwallis.ch
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
***********************************************************************************/
// Here we avoid loading the header multiple times
#ifndef PERIODICTABLE_H
#define PERIODICTABLE_H

#include <QGraphicsGridLayout>

#include <plasma/widgets/pushbutton.h>
#include <plasma/widgets/iconwidget.h>
#include <plasma/tooltipmanager.h>
#include <Plasma/Applet>

class ElementLabel;

class PeriodicGrid : public QGraphicsWidget
{
    Q_OBJECT
public:
//         Basic Create/Destroy
    PeriodicGrid(Plasma::Applet *applet = 0);
    PeriodicGrid(int tableTyp, Plasma::Applet *applet = 0);

    ~PeriodicGrid();

    /// gets the current periodic table id.
    int getCurrentPseTyp() const;

    /// creates all element labels and put them in the grid.
    void create();

public slots:
    /// sets the new table typ and creates the table.
    void setCurrentPseTyp(int tableTyp);

private:
    QGraphicsGridLayout *m_actualPeriodSystem;

    int m_psTableType;

    QList<ElementLabel*>  m_elementItemList;

    Plasma::DataEngine::Data m_element;
    Plasma::Applet *m_applet;
};

class ElementLabel : public Plasma::IconWidget
{
    Q_OBJECT
public:
//         Basic Create/Destroy
    ElementLabel( int element,  Plasma::Applet *applet, QGraphicsWidget * parent = 0);


signals:
    /// emits the text string of the current element.
    void ElementAction(QString text);

private slots:
    /// is used to add information to the clicked() signal.
    void ElementPressed() {
        emit ElementAction(text());
    }
private:
    void drawToolTip( Plasma::DataEngine::Data &element );
};

#endif // PERIODICTABLE_H
