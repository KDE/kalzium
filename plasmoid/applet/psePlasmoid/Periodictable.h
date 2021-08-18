/*
    Mass calculator: Plasmoid to calculate mass of a Molecule.
    SPDX-FileCopyrightText: 2009, 2010 Etienne Rebetez etienne.rebetez@oberwallis.ch

    SPDX-License-Identifier: GPL-2.0-or-later

*/
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
    explicit PeriodicGrid(Plasma::Applet *applet = 0);
    explicit PeriodicGrid(int tableTyp, Plasma::Applet *applet = 0);

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
