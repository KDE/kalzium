/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef PSE_H
#define PSE_H

class QLabel;
class QPixmap;
class QPoint;
class QVBoxLayout;

#include <qvaluelist.h>
#include <qwidget.h>
#include <qdialog.h>

#include "element.h"
#include "informationdialog_impl.h"

/**
 * @short basic class for the specific PSEs
 * @author Carsten Niehaus
 */
class PSE : public QWidget
{
	Q_OBJECT

	public:
		/**
		 * Constructor
		 * @param data is the object in which all data is stored
		 */
		PSE( KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
		~PSE();

		KalziumDataObject *d;

		enum NUMERATIONTYPE
		{
			NO=0,
			CAS = 1,
			IUPAC = 2,
			IUPACOLD = 3
		};

		/**
		 * if this mode is activated a click on a button will not open
		 * a informationdialog
		 */
		virtual void activateMolcalcmode( bool mode ){
			m_molcalcIsActive = mode;
		}

		virtual bool molcalcMode() const{
			return m_molcalcIsActive;
		}

		/**
		 * sets the NUMERATIONTYPE @p num of the periodic table
		 */
		void setNumerationType( int num ){
			m_num = num;
			updateNumeration();
		}

		/**
		 * This method sets the colors of the PSE. 
		 @param nr takes 5 different values:
		 @li normal view
		 @li groups
		 @li blocks
		 @li state-of-matter
		 @li acidic behavior
		 */
		void activateColorScheme( const int nr);

		/**
		 * @return the short and descriptive name of this PSE
		 */
		QString shortName() const{ 
			return m_ShortName; 
		}

		/**
		 * This method sets the color for the buttons corresponding to
		 * the given temperature @p temp
		 * @param temp is the temperature to which all buttons will be set
		 */
		void setTemperature( const double temp ){
			m_temperature = temp;
		}

		void setPSEType( bool simple ){
			m_isSimple = simple;
		}
		
		///if true the State Of Matter will be shown
		bool m_showSOM;
		
		void activateSOMMode( bool som ){
			m_showSOM = som;
		}

		/**
		 * This list includes all elementbuttons which are in the 
		 * specific PSE. For example. in the simple PSE
		 * the elements of the f- and d-Block would not
		 * be in this list
		 */
		QPtrList<Element> m_PSEElements;

		void showLegend( bool show ){
			m_showLegend = show;
		}

		void setLearning( bool );
		

	private:
		bool m_learningMode;
		
		/**
		 * updates the numeration of the PSE
		 */
		virtual void updateNumeration();

		///the currently selected element (the x/y-coordinates)
		QPoint m_currentPoint;
		
		void mouseReleaseEvent( QMouseEvent* );

		///if true the user looks at periods
		bool m_Vertikal;

		QStringList m_IUPAClist;
		QStringList m_IUPACOLDlist;
		
		bool m_showLegend;
		
		/**
		 * this is a short, descriptive name of the PSE
		 */
		QString m_ShortName;

		///the temperature of the table (for the SOM-feature)
		double m_temperature;

		bool m_isSimple;
		
		/**
		 * true if the molcalc-mode is active
		 */
		bool m_molcalcIsActive;

		/**
		 * the type of the nummeration ( NO, CAS, IUPACOLD, IUPAC )
		 */
		int m_num;

    QPixmap *table;
    bool doFullDraw;

  public slots:
	/**
	 * this method hides all elements which have not been know
	 * before the @p date.
	 * @param date is time where the user wants to see whether
	 * or not the element has already been kown
	 */
	void setDate( int date );
		
 	 void setLearningMode( int horizontal ){
			if ( horizontal == 1 )
				m_Vertikal = false;
			else
				m_Vertikal = true;
		}
		
	protected:
		virtual void paintEvent( QPaintEvent *e );
		virtual void resizeEvent( QResizeEvent *e );

	public slots:	
		/**
		 * this slot updates the currently selected point
		 */
		void slotUpdatePoint( QPoint point );

	signals:
		/**
		 * this signal is emited when the table is clicked
		 */
		void tableClicked(QPoint);
		
		/**
		 * this signal is emited when an element is clicked
		 */
		void ElementClicked(int);

	public:
		virtual void drawPSE( QPainter* p, bool useSimpleView );
		virtual void drawSOMPSE( QPainter* p );
		virtual void drawLegend( QPainter* p );
};


#endif
