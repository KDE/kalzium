/***************************************************************************
                          kalziumconverter.h  -  description
                             -------------------
    begin                : Sunday Oct 20 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#ifndef KALZIUMCONVERTER_H
#define KALZIUMCONVERTER_H

class QLabel;

#include <converter.h>

class KConvert : public KalziumConvert  {
    Q_OBJECT

    public: 
        KConvert(QWidget *parent, const char *name=0 );

    private:
        long toSI( QString );
        QString toTarget( long );

    private slots:
        void slotCalculate();
        void slotSetResultDim(const QString&);
        void slotSetToDim(const QString&);
};

#endif // KALZIUMCONVERTER_H
