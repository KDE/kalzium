/***************************************************************************
 *   Copyright (C) 2003-2005 by Carsten Niehaus                            *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#ifndef PerodicTableView_H
#define PerodicTableView_H

class TQLabel;
class TQPixmap;
class TQPoint;
class TQVBoxLayout;
class Element;
class KalziumDataObject;
class KalziumTip;

#include <tqvaluelist.h>
#include <tqwidget.h>
#include <tqtimer.h>

// A PerodicTableView is ...
/**
 * @short basic class for the specific PerodicTableViews
 * @author Carsten Niehaus
 */
class PerodicTableView : public QWidget
{
	Q_OBJECT

	public:
		/**
		 * Constructor
		 * @param parent parent widget
		 * @param name name of this widget
		 */
		PerodicTableView( TQWidget *parent = 0, const char *name = 0);
		~PerodicTableView();

		enum SCHEMETYPE
		{
			NOCOLOUR = 0,
			GROUPS,
			BLOCK,
			ACIDIC,
			FAMILY,
			GRADIENT,
			CRYSTAL
		};
		
		enum NUMERATIONTYPE
		{
			NO=0,              /// no numeration
			CAS = 1,           /// Chemical Abstract Service
			IUPAC = 2,         /// International Union of Pure and Applied Chemistry
			IUPACOLD = 3       /// old IUPAC numeration
		};

		/**
		 * if this mode is activated a click on a button will not open
		 * a information dialog
		 */
		virtual void activateMolcalcmode( bool mode ){
			m_molcalcIsActive = mode;
		}

		/**
		 * @return if the molcalc mode is active or not
		 */
		virtual bool molcalcMode() const{
			return m_molcalcIsActive;
		}

		/**
		 * sets the NUMERATIONTYPE @p num of the periodic table
		 * @see NUMERATIONTYPE
		 */
		void setNumerationType( NUMERATIONTYPE num ){
			m_num = num;
			update();
		}
		/**
		 * @return whether the tooltips are enabled
		 */
		bool tooltipsEnabled() const{
			return m_tooltipsEnabled;
		}

		/**
		 * if @p enabled is true the tooltips
		 * will be enabled
		 */
		void setTooltipsEnabled( bool enabled ){
			m_tooltipsEnabled = enabled;
		}

		/**
		 * This method sets the colors of the PerodicTableView.
		 * @param nr takes 5 different values:
		 * @li normal view
		 * @li groups
		 * @li blocks
		 * @li state of matter
		 * @li acidic behavior
		 * @li family view
		 */
		void activateColorScheme( const int nr);

		/**
		 * @return the short and descriptive name of this PerodicTableView
		 */
		TQString shortName() const{
			return m_ShortName;
		}

		/**
		 * if @p show is true the tooltip will be displayed
		 */
		void setShowTooltip( bool show ){
			m_showTooltip = show;
		}

		/**
		 * @return whether tooltips will be displayed
		 */
		bool showTooltip() const {
			return m_showTooltip;
		}

		/**
		 * @return whether if the SOM is active or not
		 */
		bool som() const{
			return m_showSOM;
		}
		
		void activateSOMMode( bool som ){
			m_showSOM = som;
			setFullDraw();
			update();
		}
		
		/**
		 * @return if the gradient mode is active
		 */
		bool gradient() const{
			return m_showGradient;
		}
		
		/**
		 * Defines if the gradient mode will be activated or
		 * deactivated
		 * @param gradient defines if the gradient mode should be activated or not
		 */
		void setGradient( bool gradient ){
			m_showGradient = gradient;
			setFullDraw();
			update();
		}

		//XXX can't use Element::TYPE here... why?
		void setGradientType( int type ){ 
			m_gradientType = type;
		}

		/**
		 * if false, the user disabled tooltips
		 */
		bool m_tooltipsEnabled;

		bool m_showLegendTooltip;

		/**
		 * activates or deactivates the legend
		 */
		void showLegend( bool show ){
			m_showLegend = show;
		}

		bool showLegend() const{
			return m_showLegend;
		}

		bool timeline() const{
			return m_timeline;
		}

		void setTimeline( bool timeline ){
			if ( m_timeline != timeline )
				m_timeline = timeline;
		}
		
		int date() const{
			return m_date;
		}

		/**
		 * load the colours from the config file. This is done
		 * on startup and everytime the user changed the configuration
		 */
		void reloadColours();

		/**
		 * JH: Draw the full table next time
		 */
		void setFullDraw(){
			doFullDraw = true;
		}

		/**
		 * @param type set the scheme
		 * @param which set the type of gradient
		 * @see Element::TYPE
		 */
		void setLook( PerodicTableView::SCHEMETYPE type, int which = 0 );

		/**
		 * This method paints the marker around the currently selected
		 * element
		 */
		virtual void paintCurrentSelection();

	private:
		/**
		 * if true the tooltips will be displayed
		 */
		bool m_showTooltip;

		/**
		 * the type of the gradient.
		 * @see Element::TYPE
		 */
		int m_gradientType;

		/**
		 * calculates the min and max values to prepare the painting
		 */
		void calculateGradient( TQPainter* );

		/**
		 * @return true if the mouse is over the legend area
		 */
		bool pointerOnLegend(int,int);

		/**
		 * @param p The painter for drawing
		 * @param e the element which is to be drawn
		 * @param coeff ?
		 * @param value the value
		 * @param minValue the smallest of all the values
		 */
		void drawGradientButton( TQPainter* p, Element* e, double coeff, double value, double minValue );

		/**
		 * calculates the color of an element which has a value which
		 * is @p percentage of the maximum value. This will be the
		 * color used in the gradient view for an element.
		 */
		TQColor calculateColor( const double percentage );
	
		/**
		 * the date used in the timeline
		 */
		int m_date;

		/**
		 * the current colour scheme
		 */
		int m_currentScheme;
	
		KalziumTip* m_kalziumTip;	
		bool m_timeline;
		
		/**
		 * the temperature of the table (for the SOM-feature)
		 */
		double m_temperature;

		/**
		 * if true the State Of Matter will be shown
		 */
		bool m_showSOM;
		
		/**
		 * if true the gradients will be shown
		 */
		bool m_showGradient;

		/**
		 * timer used for the tooltop
		 */
		TQTimer HoverTimer,
			   MouseoverTimer;

		KalziumDataObject *d;

		/**
		 * the number of the element the mouse-cursor is over
		 */
		int m_tooltipElementNumber;

		/**
		 * @return the number of the element at position x/y. If there
		 * is no element it will return 0
		 */
		int ElementNumber( int x, int y );
		
		/**
		 * @return the coordinates of the element under the mouseCursor.
		 * For example, H will be 1/1 and Li will be 1/2
		 */
		TQPoint ElementUnderMouse();

		/**
		 * the currently selected element (the x/y-coordinates)
		 */
		TQPoint m_currentPoint;

		void mouseReleaseEvent( TQMouseEvent* );
		void mousePressEvent( TQMouseEvent* );
		void mouseMoveEvent( TQMouseEvent* );

		TQStringList m_IUPAClist;
		TQStringList m_IUPACOLDlist;

		/**
		 * if the the legend will be displayed
		 */
		bool m_showLegend;

		/**
		 * this is a short, descriptive name of the PerodicTableView
		 */
		TQString m_ShortName;

		/**
		 * true if the molcalc-mode is active
		 */
		bool m_molcalcIsActive;

		/**
		 * the type of the nummeration ( NO, CAS, IUPACOLD, IUPAC )
		 */
		NUMERATIONTYPE m_num;

		/**
		 * implements double buffering of the widget.
		 */
		TQPixmap *table;			// The basic PerodicTableView
		TQPixmap *table2;		// Basic PerodicTableView + extra data such as tooltip, etc

		/**
		 * used for bitBlit. If true the complete table will be drawn
		 */
		bool doFullDraw;
		

		//I am holding all colours as member so that they don't need to 
		//be reloaded on every reload
		TQColor color_s;//Blocks
		TQColor color_p;
		TQColor color_d;
		TQColor color_f;
		TQColor color_1;//Groups
		TQColor color_2;
		TQColor color_3;
		TQColor color_4;
		TQColor color_5;
		TQColor color_6;
		TQColor color_7;
		TQColor color_8;
		TQColor color_ba;//Acidic
		TQColor color_ac;
		TQColor color_neu;
		TQColor color_amp;
		TQColor c_alkalie;//Family
		TQColor c_rare;
		TQColor c_nonmetal;
		TQColor c_alkaline;
		TQColor c_other_metal;
		TQColor c_halogene; 
		TQColor c_transition;
		TQColor c_noble_gas;
		TQColor c_metalloid;
		TQColor c_liquid;
		TQColor c_solid;
		TQColor c_vapor;
		
	protected:
		virtual void paintEvent( TQPaintEvent *e );

		/**
		 * draw the tooltip for the legend
		 */
		virtual void drawLegendToolTip( TQPainter *p );

		virtual void drawTimeLine( TQPainter *p );

		/**
		 * called if the user resized the table
		 */
		virtual void resizeEvent( TQResizeEvent *e );

		/**
		 * the central place for the drawing of the table
		 */
		virtual void drawPerodicTableView( TQPainter* p, bool isCrystal );

		/**
		 * draw a gradient of the type @p type
		 */
		virtual void drawGradientPerodicTableView( TQPainter* p, const double min, const double max );

		/**
		 * draw the state of matter
		 */
		virtual void drawSOMPerodicTableView( TQPainter* p );

		/**
		 * draw the legend
		 */
		virtual void drawLegend( TQPainter* p );
		
		/**
		 * draw the numeration
		 */
		virtual void drawNumeration( TQPainter* p );

	public slots:
		/**
		 * This method sets the color for the buttons corresponding to
		 * the given temperature @p temp
		 * @param temp is the temperature to which all buttons will be set
		 */
		void setTemperature( int temp ){
			m_temperature = (double)temp;
			update();
		}

		void setDate( int date ){
			//These elements have always been known:
			//6 16 26 29 33 47 50 51 79 80 82 83
			m_date = date;
			update();
		}

		/**
		 * this slot removes the selection of any point
		 */
		void unSelect();

	private slots:
		/**
		 * If called this slot will emit the signal MouseOver( num )
		 * where num is the number of the element the mouse if over.
		 * If the mouse is not over an element nothing will be emited
		 * @see MouseOver()
		 */
		void slotMouseover();
		
		/**
		 * start the calculation of the element over which the mouse-cursor
		 * is over. Finally the signal ToolTip( int ) is emitted
		 */
		void slotTransientLabel();
		
		/**
		 * this slot updates the currently selected point
		 */
		void selectPoint( const TQPoint& point );

		/**
		 * this slot updates the element given in the @p num
		 * @param num The number of the selected element
		 */
		void selectElement( int num );

		/**
		 * sets the current element to @p number
		 * and updates the table
		 */
		void slotToolTip( int number );
		
	signals:
		/**
		 * this signal is emitted when the table is clicked
		 */
		void tableClicked(TQPoint);

		/**
		 * this signal is emitted when an element is clicked
		 */
		void ElementClicked(int);

		/**
		 * this signal is emitted when the tooltip of an element
		 * has to be displayed
		 */
		void ToolTip(int);

		/**
		 * This signal is emited when the mouse pointer is
		 * over an element
		 */
		void MouseOver( int );
};


#endif
