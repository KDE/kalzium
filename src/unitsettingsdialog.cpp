/*
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

#include "unitsettingsdialog.h"

#include <QGridLayout>
#include <QLabel>

#include <KLocalizedString>
#include <KUnitConversion/Converter>

UnitSettingsDialog::UnitSettingsDialog(QWidget* parent) : QWidget(parent)
{
    QLabel *labelEnergy = new QLabel(i18n("Energy:"), this);
    QList<int> energy;
    energy << KUnitConversion::Electronvolt << KUnitConversion::KiloJoulePerMole <<
           KUnitConversion::JoulePerMole << KUnitConversion::Joule;
    m_comboBoxLEnergiesUnit = new KalziumUnitCombobox(energy, this);
    m_comboBoxLEnergiesUnit->setObjectName(QStringLiteral("kcfg_combobox_energies"));

    QLabel *labelLenght = new QLabel(i18n("Length:"), this);
    QList<int> length;
    length << KUnitConversion::Picometer << KUnitConversion::Nanometer << KUnitConversion::Angstrom;
    m_comboBoxLengthUnit = new KalziumUnitCombobox(length, this);
    m_comboBoxLengthUnit->setObjectName(QStringLiteral("kcfg_combobox_length"));

    QLabel *labelTemperature = new QLabel(i18n("Temperature:"), this);
    QList<int> temperature;
    temperature << KUnitConversion::Kelvin << KUnitConversion::Celsius << KUnitConversion::Fahrenheit <<
                KUnitConversion::Reaumur;
    m_comboBoxLTemperatureUnit = new KalziumUnitCombobox(temperature, this);
    m_comboBoxLTemperatureUnit->setObjectName(QStringLiteral("kcfg_combobox_temperature"));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(labelEnergy, 0, 0);
    layout->addWidget(m_comboBoxLEnergiesUnit, 0, 1);

    layout->addWidget(labelLenght, 1, 0);
    layout->addWidget(m_comboBoxLengthUnit, 1, 1);

    layout->addWidget(labelTemperature, 2, 0);
    layout->addWidget(m_comboBoxLTemperatureUnit, 2, 1);

    layout->setRowStretch(3, 1);

    setLayout(layout);
}

int UnitSettingsDialog::getEnergyUnitId()
{
    return m_comboBoxLEnergiesUnit->getCurrentUnitId();
}

int UnitSettingsDialog::getLenghtUnitId()
{
    return m_comboBoxLengthUnit->getCurrentUnitId();
}

int UnitSettingsDialog::getTemperatureUnitId()
{
    return m_comboBoxLTemperatureUnit->getCurrentUnitId();
}

UnitSettingsDialog::~UnitSettingsDialog()
{
    delete m_comboBoxLEnergiesUnit;
    delete m_comboBoxLengthUnit;
    delete m_comboBoxLTemperatureUnit;
}
