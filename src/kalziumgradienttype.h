/***************************************************************************
 *   Copyright (C) 2005, 2006      by Pino Toscano, toscano.pino@tiscali.it      *
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

#ifndef KALZIUMGRADIENTTYPE_H
#define KALZIUMGRADIENTTYPE_H

class KalziumGradientType;

#include <QByteArray>
#include <QColor>
#include <QStringList>

/**
 * Factory for KalziumGradientType classes.
 *
 * @author Pino Toscano
 */
class KalziumGradientTypeFactory
{
	public:
		/**
		 * Get the instance of this factory.
		 */
		static KalziumGradientTypeFactory* instance();

		/**
		 * Returns the KalziumGradientType with the @p id specified.
		 * It will gives 0 if none found.
		 */
		KalziumGradientType* build( int id ) const;
		/**
		 * Returns the KalziumGradientType whose name is the @p id
		 * specified.
		 * It will gives 0 if none found.
		 */
		KalziumGradientType* build( const QByteArray& id ) const;

		/**
		 * Returns a list with the names of the gradients we support.
		 */
		QStringList gradients() const;

	private:
		KalziumGradientTypeFactory();

		QList<KalziumGradientType*> m_gradients;
};

/**
 * Base class representing a gradient.
 * Inherit it and add its instance to the factory to add it globally.
 *
 * @author Pino Toscano
 */
class KalziumGradientType
{
	public:
		/**
		 * Get its instance.
		 */
		static KalziumGradientType* instance();

		virtual ~KalziumGradientType();

		/**
		 * Returns the ID of this gradient.
		 * Mainly used when saving/loading.
		 */
		virtual QByteArray name() const = 0;
		/**
		 * Returns the description of this gradient.
		 * Used in all the visible places.
		 */
		virtual QString description() const = 0;

		/**
		 * Calculate the coefficient of the element with atomic number
		 * @p el according to this gradient. The calculated coefficient
		 * will be always in the range [0, 1].
		 */
		virtual double elementCoeff( int el ) const;
		/**
		 * Return the value, related to the current gradient, of the
		 * element with atomic number @p el.
		 * It will return -1 if the data is not available.
		 */
		virtual double value( int el ) const = 0;
		/**
		 * Returns the minimum value of the data this gradient
		 * represents.
		 */
		virtual double minValue() const = 0;
		/**
		 * Returns the maximum value of the data this gradient
		 * represents.
		 */
		virtual double maxValue() const = 0;

		/**
		 * Returns the first color of the gradient.
		 */
		virtual QColor firstColor() const;
		/**
		 * Returns the second color of the gradient.
		 */
		virtual QColor secondColor() const;
		/**
		 * Returns the color used to represent an element whose data is
		 * not available.
		 */
		virtual QColor notAvailableColor() const;

		/**
		 * Calculates the color of an element which has a @p coeff which
		 * is a percentage of the maximum value.
		 * @param coeff is the coefficient in the range [0, 1], usually
		 * calculated with elementCoeff()
		 */
		QColor calculateColor( const double coeff ) const;

	protected:
		KalziumGradientType();
};

/**
 * The gradient by covalent radius.
 *
 * @author Pino Toscano
 */
class KalziumCovalentRadiusGradientType : public KalziumGradientType
{
	public:
		static KalziumCovalentRadiusGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumCovalentRadiusGradientType();
};

/**
 * The gradient by van Der Waals radius.
 *
 * @author Pino Toscano
 */
class KalziumVanDerWaalsRadiusGradientType : public KalziumGradientType
{
	public:
		static KalziumVanDerWaalsRadiusGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumVanDerWaalsRadiusGradientType();
};

/**
 * The gradient by atomic mass.
 *
 * @author Pino Toscano
 */
class KalziumMassGradientType : public KalziumGradientType
{
	public:
		static KalziumMassGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumMassGradientType();
};

/**
 * The gradient by boiling point.
 *
 * @author Pino Toscano
 */
class KalziumBoilingPointGradientType : public KalziumGradientType
{
	public:
		static KalziumBoilingPointGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumBoilingPointGradientType();
};

/**
 * The gradient by melting point.
 *
 * @author Pino Toscano
 */
class KalziumMeltingPointGradientType : public KalziumGradientType
{
	public:
		static KalziumMeltingPointGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumMeltingPointGradientType();
};

/**
 * The gradient by electronegativity.
 *
 * @author Pino Toscano
 */
class KalziumElectronegativityGradientType : public KalziumGradientType
{
	public:
		static KalziumElectronegativityGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumElectronegativityGradientType();
};

/**
 * The gradient by discoverydate.
 *
 * @author Carsten Niehaus
 */
class KalziumDiscoverydateGradientType : public KalziumGradientType
{
	public:
		static KalziumDiscoverydateGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumDiscoverydateGradientType();
};

/**
 * The gradient by electronaffinity.
 *
 * @author Carsten Niehaus
 */
class KalziumElectronaffinityGradientType : public KalziumGradientType
{
	public:
		static KalziumElectronaffinityGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumElectronaffinityGradientType();
};

/**
 * The gradient by the first ionization energy.
 *
 * @author Carsten Niehaus
 */
class KalziumIonizationGradientType : public KalziumGradientType
{
	public:
		static KalziumIonizationGradientType* instance();

		QByteArray name() const;
		QString description() const;

		double value( int el ) const;

		double minValue() const;
		double maxValue() const;

	private:
		KalziumIonizationGradientType();
};

#endif // KALZIUMGRADIENTTYPE_H
