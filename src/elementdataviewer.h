/***************************************************************************
    copyright            : (C) 2004, 2005, 2006 by Carsten Niehaus
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
#ifndef ELEMENTDATAVIEWER_H
#define ELEMENTDATAVIEWER_H

#include <QDialog>
#include "ui_plotsetupwidget.h"
#include "kalziumdataobject.h"
#include "kalziumutils.h"

class QTimer;
class KActionCollection;

typedef QList<double> DoubleList;

/**
 * @short the values of an axis
 * @author Carsten Niehaus
 */
class AxisData
{
public:
    /**
     * This represents the possible datasets.
     */
    enum PAXISDATA {
        NUMBER = 0,
        MASS,
        EN,
        MELTINGPOINT,
        BOILINGPOINT,
        ATOMICRADIUS,
        COVALENTRADIUS
    };

    enum AXISTYPE {
        X = 0,
        Y
    };

    AxisData(AxisData::AXISTYPE);

    /**
     * @return the value of the selected dataset of element @p element
     */
    double value(int element) const;

    /**
     * the dataList contains the values off all elements
     * but only of the currently selected data type. This
     * means that it eg contains all boiling points
     */
    DoubleList dataList;

    QString unit;

    int currentDataType;

    ChemicalDataObject::BlueObelisk kind;

    AXISTYPE type() const
    {
        return m_type;
    }

private:
    AXISTYPE m_type;
};


/**
 * @short This widget shows the plot and the widget
 * where you can setup the plot
 * @author Carsten Niehaus
 */
class ElementDataViewer : public QDialog
{
    Q_OBJECT

public:
    ElementDataViewer(QWidget *parent = nullptr);

    ~ElementDataViewer();

    /**
     * the AxixData for the y-Axis
     */
    AxisData *m_yData;

    /**
     * the AxixData for the x-Axis
     */
    AxisData *m_xData;


protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    Ui::PlotSetupWidget ui;

    void getMinMax(double& min, double& max, AxisData * data);

    QStringList         names;
    QStringList         symbols;
    QStringList         elecConfig; //Electronic configuration of elements
    QStringList         block;  //Indicates the periodic table block s,p,d,f...
    QTimer *m_timer;

    KActionCollection* m_actionCollection;

    void initData();
    void setupAxisData(AxisData * data);

    void setLimits();

protected slots:
    /**
     * invoke the help of the correct chapter
     */
    virtual void slotHelp();

private slots:
    void rangeChanged();
    void fullRange();

public slots:
    void slotZoomIn();
    void slotZoomOut();

    /**
     * draws the plot
     */
    void drawPlot();
};

#endif // ELEMENTDATAVIEWER_H
