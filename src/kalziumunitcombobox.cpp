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

#include "kalziumunitcombobox.h"

#include "kalziumutils.h"

KalziumUnitCombobox::KalziumUnitCombobox(QWidget* parent): QComboBox(parent)
{
}

KalziumUnitCombobox::KalziumUnitCombobox(const QList< int > &unitList, QWidget* parent)
    : QComboBox(parent)
{
    setUnitList(unitList);
}

void KalziumUnitCombobox::setUnitList(const QList< int >& unitList)
{
    KalziumUtils::populateUnitCombobox(this, unitList);
}

int KalziumUnitCombobox::getCurrentUnitId() const
{
    return itemData(currentIndex()).toInt();
}

void KalziumUnitCombobox::setIndexWithUnitId(int unit)
{
    setCurrentIndex(findData(unit));
}
