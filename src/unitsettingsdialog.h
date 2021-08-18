/*
    <one line to give the program's name and a brief idea of what it does.>
    SPDX-FileCopyrightText: 2011 Rebetez Etienne <etienne.rebetez@oberwallis.ch>

    SPDX-License-Identifier: GPL-2.0-or-later
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
