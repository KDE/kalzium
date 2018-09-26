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

#ifndef KALZIUMNUMERATIONTYPE_H
#define KALZIUMNUMERATIONTYPE_H

class KalziumNumerationType;

#include <QByteArray>
#include <QStringList>

/**
 * Factory for KalziumNumerationType classes.
 *
 * @author Pino Toscano
 */
class KalziumNumerationTypeFactory
{
public:
    /**
     * Get the instance of this factory.
     */
    static KalziumNumerationTypeFactory* instance();

    /**
     * Returns the KalziumNumerationType with the @p id specified.
     * It will gives 0 if none found.
     */
    KalziumNumerationType* build(int id) const;
    /**
     * Returns the KalziumNumerationType whose name is the @p id
     * specified.
     * It will gives 0 if none found.
     */
    KalziumNumerationType* build(const QByteArray& id) const;

    /**
     * Returns a list with the names of the numeration types we
     * support.
     */
    QStringList numerations() const;

private:
    KalziumNumerationTypeFactory();

    QList<KalziumNumerationType*> m_numerations;
};

/**
 * Base class for a numeration type.
 * It's quite simple, as a numeration doesn't have many data to represent.
 *
 * @author Pino Toscano
 */
class KalziumNumerationType
{
public:
    /**
     * Get its instance.
     */
    static KalziumNumerationType* instance();

    virtual ~KalziumNumerationType();

    /**
     * Returns the ID of this numeration type.
     * Mainly used when saving/loading.
     */
    virtual QByteArray name() const = 0;
    /**
     * Returns the description of this numeration type.
     * Used in all the visible places.
     */
    virtual QString description() const = 0;

    /**
     * Returns the @p num 'th item of this numeration type.
     */
    virtual QString item(const int num) const;
    /**
     * Returns all the items of this numeration type.
     */
    virtual QStringList items() const;

protected:
    KalziumNumerationType();

    QStringList m_items;
};

/**
 * The class representing no numeration.
 * This could look a bit weird, but this way makes quite modular even disabling
 * the numeration.
 *
 * @author Pino Toscano
 */
class KalziumNoneNumerationType : public KalziumNumerationType
{
public:
    static KalziumNoneNumerationType* instance();

    QByteArray name() const override;
    QString description() const override;

    QString item(const int num) const override;
    QStringList items() const override;

private:
    KalziumNoneNumerationType();
};

/**
 * The numeration "International Union of Pure and Applied Chemistry" (IUPAC).
 *
 * @author Pino Toscano
 */
class KalziumIUPACNumerationType : public KalziumNumerationType
{
public:
    static KalziumIUPACNumerationType* instance();

    QByteArray name() const override;
    QString description() const override;

private:
    KalziumIUPACNumerationType();
};

/**
 * The numeration "Chemical Abstract Service" (CAS).
 *
 * @author Pino Toscano
 */
class KalziumCASNumerationType : public KalziumNumerationType
{
public:
    static KalziumCASNumerationType* instance();

    QByteArray name() const override;
    QString description() const override;

private:
    KalziumCASNumerationType();
};

/**
 * The old IUPAC numeration.
 *
 * @author Pino Toscano
 */
class KalziumOldIUPACNumerationType : public KalziumNumerationType
{
public:
    static KalziumOldIUPACNumerationType* instance();

    QByteArray name() const override;
    QString description() const override;

private:
    KalziumOldIUPACNumerationType();
};

#endif // KALZIUMNUMERATIONTYPE_H
