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


#include "unitsettingsdialog.h"

#include <kunitconversion/converter.h>

#include <KLocale>

#include <QGridLayout>
#include <QLabel>
#include <kunitconversion/unitcategory.h>

UnitSettingsDialog::UnitSettingsDialog(QWidget* parent): QWidget(parent)
{
    QLabel *labelEnergy = new QLabel(i18n("Energy:"), this);
    QList<int> energy;
    energy << KUnitConversion::Electronvolt << KUnitConversion::KiloJoulePerMole;
    m_comboEnergy = new KalziumUnitCombobox(energy, this);

    QLabel *labelLenght = new QLabel(i18n("Length:"), this);
    QList<int> length;
    length << KUnitConversion::Nanometer << KUnitConversion::Picometer << KUnitConversion::Angstrom;
    m_comboLenght = new KalziumUnitCombobox(length, this);

    QLabel *labelTemperature = new QLabel(i18n("Temperature:"), this);
    QList<int> temperature;
    temperature << KUnitConversion::Kelvin << KUnitConversion::Celsius << KUnitConversion::Fahrenheit;
    m_comboTemerature = new KalziumUnitCombobox(temperature, this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(labelEnergy, 0, 0);
    layout->addWidget(m_comboEnergy, 0, 1);

    layout->addWidget(labelLenght, 1, 0);
    layout->addWidget(m_comboLenght, 1, 1);

    layout->addWidget(labelTemperature, 2, 0);
    layout->addWidget(m_comboTemerature, 2, 1);

    layout->setRowStretch(3, 1);

    setLayout(layout);
}

