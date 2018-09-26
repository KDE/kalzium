/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Rebetez Etienne <etienne.rebetez@oberwallis.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
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
public:
    explicit UnitSettingsDialog(QWidget* parent = nullptr);
    virtual ~UnitSettingsDialog();

    int getLenghtUnitId();

    int getEnergyUnitId();

    int getTemperatureUnitId();

private:
    KalziumUnitCombobox *m_comboBoxLengthUnit;
    KalziumUnitCombobox *m_comboBoxLEnergiesUnit;
    KalziumUnitCombobox *m_comboBoxLTemperatureUnit;

};

#endif // UNITSETTINGSDIALOG_H
