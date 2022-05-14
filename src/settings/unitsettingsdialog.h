/*
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
    ~UnitSettingsDialog() override;

    int getLenghtUnitId() const;

    int getEnergyUnitId() const;

    int getTemperatureUnitId() const;

private:
    KalziumUnitCombobox *m_comboBoxLengthUnit = nullptr;
    KalziumUnitCombobox *m_comboBoxLEnergiesUnit = nullptr;
    KalziumUnitCombobox *m_comboBoxLTemperatureUnit = nullptr;

};

#endif // UNITSETTINGSDIALOG_H
