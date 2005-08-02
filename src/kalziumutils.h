#ifndef KALZIUMUTILS_H
#define KALZIUMUTILS_H

class QString;
class QRect;
class QFont;
class QPainter;

class KalziumUtils
{
	public:
		/**
		 * @param string the string which is measured
		 * @param rect the rect needed for measurement
		 * @param font the used font
		 * @param painter the used painter
		 */
		static int maxSize( const QString& string, const QRect& rect, QFont font, QPainter* painter );

		/**
		 * calculate the 4-digit value of the value @p w
		 * @return the 4-digit value
		 */
		static double strippedValue( double w );
};

#endif // KALZIUMUTILS_H

