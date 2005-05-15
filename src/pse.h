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
#include <qtimer.h>

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

		enum NUMERATIONTYPE
		{
			NO=0,              //no numeration
			CAS = 1,           //Chemical Abstract Service
			IUPAC = 2,         //Intern. Union of Pure and Applied Chemistry
			IUPACOLD = 3       //old IUPAC numeration
		};

		/**
		 * if this mode is activated a click on a button will not open
		 * a informationdialog
		 */
		virtual void activateMolcalcmode( bool mode ){
			m_molcalcIsActive = mode;
		}

		/**
		 * @return if the the learningmode is active or not
		 */
		virtual bool learningMode() const{
			return m_learningMode;
		}

		/**
		 * @return if the the molcalc-Modus is active or not
		 */
		virtual bool molcalcMode() const{
			return m_molcalcIsActive;
		}

		/**
		 * sets the NUMERATIONTYPE @p num of the periodic table
		 */
		void setNumerationType( int num ){
			m_num = num;
			update();
		}

		/**
		 * This method sets the colors of the PSE. 
		 @param nr takes 5 different values:
		 @li normal view
		 @li groups
		 @li blocks
		 @li state-of-matter
		 @li acidic behavior
		 @li family view 
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

		void setShowTooltip( bool show ){
			m_showTooltip = show;
		}

		/**
		 * @para simple if true the table will only show the p and s block
		 */
		void setPSEType( bool simple ){
			m_isSimple = simple;
		}
		
		/**
		 * returns if the SOM is active or not
		 */
		bool som() const{
			return m_showSOM;
		}
		
		void activateSOMMode( bool som ){
			m_showSOM = som;
		}

		/**
		 * if true the tooltips will be displayed 
		 */
		bool m_showTooltip;

		/**
		 * activates or deactivates the legend
		 */
		void showLegend( bool show ){
			m_showLegend = show;
		}

		bool showLegend() const{
			return m_showLegend;
		}

		/**
		 * activates or deactivates the learningmode
		 */
		void setLearning( bool learningmode ){
			m_learningMode = learningmode;
		}

		bool timeline() const{
			return m_timeline;
		}

		void setTimeline( bool timeline ){
			m_timeline = timeline;
		}
		
		int date() const{
			return m_date;
		}


	private:
		///the date used in the timeline
		int m_date;
		
		bool m_timeline;
		
		///the temperature of the table (for the SOM-feature)
		double m_temperature;

		///if true the State Of Matter will be shown
		bool m_showSOM;

		///Timer used for the tooltop
		QTimer HoverTimer;

		KalziumDataObject *d;

		///if true the periodic table is in the leraningmode
		bool m_learningMode;

		///the number of the element the mouse-cursor is over
		int m_tooltipElementNumber;

		/**
		 * @return the number of the element at position x/y. If there
		 * is no element it will return 0
		 */
		int ElementNumber( int x, int y );

		///the currently selected element (the x/y-coordinates)
		QPoint m_currentPoint;

		void mouseReleaseEvent( QMouseEvent* );

		void mouseMoveEvent( QMouseEvent* );

		///if true the user looks at periods
		bool m_Vertikal;

		QStringList m_IUPAClist;
		QStringList m_IUPACOLDlist;

		///if the the legend will be displayed
		bool m_showLegend;

		/**
		 * this is a short, descriptive name of the PSE
		 */
		QString m_ShortName;

		///if true the pse is drawn in simple mode (s+p block only)
		bool m_isSimple;

		///true if the molcalc-mode is active
		bool m_molcalcIsActive;

		/**
		 * the type of the nummeration ( NO, CAS, IUPACOLD, IUPAC )
		 */
		int m_num;

		///the internal representation of the table
		QPixmap *table;

		///used for bitBlit. If true the complete table will be drawn
		bool doFullDraw;
		
		
	protected:
		virtual void paintEvent( QPaintEvent *e );

		///in this method the tooltops are drawn.
		virtual void drawToolTip( QPainter *p, Element *e );

		///called if the user resized the table
		virtual void resizeEvent( QResizeEvent *e );

		///the central place for the drawing of the table
		virtual void drawPSE( QPainter* p, bool useSimpleView );

		///draw the state of matter
		virtual void drawSOMPSE( QPainter* p );

		///draw the lengend
		virtual void drawLegend( QPainter* p );
		
		//draw the numeration
		virtual void drawNumeration( QPainter* p );

	public slots:	
		void setDate( int date ){
			m_date = date;
//These elements have always been known:
//6 16 26 29 33 47 50 51 79 80 82 83
		}

		void setLearningMode( int horizontal ){
			if ( horizontal == 1 )
				m_Vertikal = false;
			else
				m_Vertikal = true;
		}

		/**
		 * start the calculation of the element over which the mouse-cursor
		 * is over. Finally the signal ToolTip( int ) is emitted
		 */
		void slotTransientLabel();
		
		/**
		 * this slot updates the currently selected point
		 */
		void slotUpdatePoint( QPoint point );

		/**
		 * sets the current element to @p number
		 * and updates the table
		 */
		void slotToolTip( int number );

	signals:
		/**
		 * this signal is emited when the table is clicked
		 */
		void tableClicked(QPoint);

		/**
		 * this signal is emited when an element is clicked
		 */
		void ElementClicked(int);

		/**
		 * this signal is emited when the tooltip of an element
		 * has to be displayed
		 */
		void ToolTip(int);
};


#endif
