/***************************************************************************
 *   Copyright (C) 2005, 2006 by Pino Toscano, toscano.pino@tiscali.it     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "kalziumnumerationtype.h"

#include "kalziumdataobject.h"

#include <klocale.h>

KalziumNumerationTypeFactory::KalziumNumerationTypeFactory()
{
    m_numerations << KalziumNoneNumerationType::instance();
    m_numerations << KalziumIUPACNumerationType::instance();
    m_numerations << KalziumCASNumerationType::instance();
    m_numerations << KalziumOldIUPACNumerationType::instance();
}

KalziumNumerationTypeFactory* KalziumNumerationTypeFactory::instance()
{
    static KalziumNumerationTypeFactory kntf;
    return &kntf;
}

KalziumNumerationType* KalziumNumerationTypeFactory::build(int id) const
{
    if ((id < 0) || (id >= m_numerations.count())) {
        return 0;
    }

    return m_numerations.at(id);
}

KalziumNumerationType* KalziumNumerationTypeFactory::build(const QByteArray& id) const
{
    for (int i = 0; i < m_numerations.count(); ++i) {
        if (m_numerations.at(i)->name() == id) {
            return m_numerations.at(i);
        }
    }

    // not found
    return 0;
}

QStringList KalziumNumerationTypeFactory::numerations() const
{
    QStringList l;
    for (int i = 0; i < m_numerations.count(); ++i) {
        l << m_numerations.at(i)->description();
    }
    return l;
}


KalziumNumerationType* KalziumNumerationType::instance()
{
    return 0;
}

KalziumNumerationType::KalziumNumerationType()
{
}

KalziumNumerationType::~KalziumNumerationType()
{
}

QString KalziumNumerationType::item(const int num) const
{
    if ((num < 0) || (num >= m_items.count())) {
        return QString();
    }

    return m_items.at(num);
}

QStringList KalziumNumerationType::items() const
{
    return m_items;
}


KalziumNoneNumerationType* KalziumNoneNumerationType::instance()
{
    static KalziumNoneNumerationType knnt;
    return &knnt;
}

KalziumNoneNumerationType::KalziumNoneNumerationType()
        : KalziumNumerationType()
{
}

QByteArray KalziumNoneNumerationType::name() const
{
    return "None";
}

QString KalziumNoneNumerationType::description() const
{
    return i18n("No Numeration");
}

QString KalziumNoneNumerationType::item(const int num) const
{
    Q_UNUSED(num);
    return QString();
}

QStringList KalziumNoneNumerationType::items() const
{
    return QStringList();
}

KalziumIUPACNumerationType* KalziumIUPACNumerationType::instance()
{
    static KalziumIUPACNumerationType kint;
    return &kint;
}

KalziumIUPACNumerationType::KalziumIUPACNumerationType()
        : KalziumNumerationType()
{
    // cache them
    m_items << QString("1");
    m_items << QString("2");
    m_items << QString("3");
    m_items << QString("4");
    m_items << QString("5");
    m_items << QString("6");
    m_items << QString("7");
    m_items << QString("8");
    m_items << QString("9");
    m_items << QString("10");
    m_items << QString("11");
    m_items << QString("12");
    m_items << QString("13");
    m_items << QString("14");
    m_items << QString("15");
    m_items << QString("16");
    m_items << QString("17");
    m_items << QString("18");
}

QByteArray KalziumIUPACNumerationType::name() const
{
    return "IUPAC";
}

QString KalziumIUPACNumerationType::description() const
{
    return i18n("IUPAC");
}

KalziumCASNumerationType* KalziumCASNumerationType::instance()
{
    static KalziumCASNumerationType kcnt;
    return &kcnt;
}

KalziumCASNumerationType::KalziumCASNumerationType()
        : KalziumNumerationType()
{
    // cache them
    m_items << QString("IA");
    m_items << QString("IIA");
    m_items << QString("IIIB");
    m_items << QString("IVB");
    m_items << QString("VB");
    m_items << QString("VIB");
    m_items << QString("VIIB");
    m_items << QString("VIII");
    m_items << QString("VIII");
    m_items << QString("VIII");
    m_items << QString("IB");
    m_items << QString("IIB");
    m_items << QString("IIIA");
    m_items << QString("IVA");
    m_items << QString("VA");
    m_items << QString("VIA");
    m_items << QString("VIIA");
    m_items << QString("VIIIA");
}

QByteArray KalziumCASNumerationType::name() const
{
    return "CAS";
}

QString KalziumCASNumerationType::description() const
{
    return i18n("CAS");
}

KalziumOldIUPACNumerationType* KalziumOldIUPACNumerationType::instance()
{
    static KalziumOldIUPACNumerationType koint;
    return &koint;
}

KalziumOldIUPACNumerationType::KalziumOldIUPACNumerationType()
        : KalziumNumerationType()
{
    // cache them
    m_items << QString("1A");
    m_items << QString("2A");
    m_items << QString("3A");
    m_items << QString("4A");
    m_items << QString("5A");
    m_items << QString("6A");
    m_items << QString("7A");
    m_items << QString("8");
    m_items << QString("8");
    m_items << QString("8");
    m_items << QString("1B");
    m_items << QString("2B");
    m_items << QString("3B");
    m_items << QString("4B");
    m_items << QString("5B");
    m_items << QString("6B");
    m_items << QString("7B");
    m_items << QString("0");
}

QByteArray KalziumOldIUPACNumerationType::name() const
{
    return "OldIUPAC";
}

QString KalziumOldIUPACNumerationType::description() const
{
    return i18n("Old IUPAC");
}
