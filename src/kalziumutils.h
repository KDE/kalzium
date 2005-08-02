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
		 * calculate the 4-digit value of the value @p w. For 
		 * example, 12.3456 will be returned as 12.35
		 * @param value the value which will be stripped
		 * @return the 4-digit value
		 */
		static double strippedValue( double value );

		/**
		 * @param string the string which is measured
		 * @param font the used font
		 * @param painter the used painter
		 * @return the width of the string @p string
		 */
		static int StringWidth( const QString& string, const QFont& font, QPainter* painter );
		/**
		 * @param string the string which is measured
		 * @param font the used font
		 * @param painter the used painter
		 * @return the height of the string @p string
		 */
		static int StringHeight( const QString& string, const QFont& font, QPainter* painter );
};

#endif // KALZIUMUTILS_H

