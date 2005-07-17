#ifndef KALZIUMDATAOBJECT_H
#define KALZIUMDATAOBJECT_H

#include "element.h"

/**
 * @author Carsten Niehaus
 *
 * 
 * @short this class contains all Element-objects as
 * a QValueList<Element*>
 */
class KalziumDataObject
{
	public:
		KalziumDataObject();
		~KalziumDataObject();

		/**
		 * The list of element in a QValueList<Element*>
		 */
		EList ElementList;
		
		CList CoordinateList;

		Element* element( int number );

		/**
		 * @return the number of elements we have
		 */
		const int numberOfElements() const;

	private:
		EList readData( QDomDocument &dataDocument );

		// caching the number of elements
		int m_numOfElements;
};
#endif // KALZIUMDATAOBJECT_H
