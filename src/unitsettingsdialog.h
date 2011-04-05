/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Rebetez Etienne <etienne.rebetez@oberwallis.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef UNITSETTINGSDIALOG_H
#define UNITSETTINGSDIALOG_H

#include <QWidget>
#include "kalziumunitcombobox.h"

class UnitSettingsDialog: public QWidget
{
    Q_OBJECT

public:
    explicit UnitSettingsDialog(QWidget* parent = 0);

    int getLenghtUnitId();

    int getEnergyUnitId();

    int getTemperatureUnitId();

Q_SIGNALS:
    void comboboxChanged();

private:
    KalziumUnitCombobox *m_comboLenght;
    KalziumUnitCombobox *m_comboEnergy;
    KalziumUnitCombobox *m_comboTemperature;

};

#endif // UNITSETTINGSDIALOG_H
