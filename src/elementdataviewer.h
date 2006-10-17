#ifndef ELEMENTDATAVIEWER_H
#define ELEMENTDATAVIEWER_H
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

#include <kdialog.h>
#include "ui_plotsetupwidget.h"

class QTimer;
class KActionCollection;
class AxisData;

/**
 * @short This widget shows the plot and the widget
 * where you can setup the plot
 * @author Carsten Niehaus
 */
class ElementDataViewer : public KDialog
{
    Q_OBJECT

    public:
        ElementDataViewer( QWidget *parent = 0 );

        ~ElementDataViewer();

        /**
         * the AxixData for the y-Axis
         */
        AxisData *yData;


    protected:
        virtual void keyPressEvent(QKeyEvent *e);

    private: 
        Ui::PlotSetupWidget ui;

        QStringList         names;
        QStringList         symbols;

        QTimer *m_timer;

        KActionCollection* m_actionCollection;

        void initData();
        void setupAxisData();

        void setLimits(int, int);

    protected slots:
        /**
         * invoke the help of the correct chapter
         */
        virtual void slotHelp();

    private slots:
        void rangeChanged();

    public slots:
        void slotZoomIn();
        void slotZoomOut();

        /**
         * draws the plot
         */
        void drawPlot();
};

#endif // ELEMENTDATAVIEWER_H

