/***************************************************************************
                          elementkp.h  -  description
                             -------------------
    begin                : Mon Dec 10 2001
    copyright            : (C) 2001 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ELEMENTKP_H
#define ELEMENTKP_H

#include <kpushbutton.h>

class KStatusBar;

class QColor;
class QLabel;
class QPopupMenu;
class QString;

class Kalzium;

/**This class is derived from KPushButton. This is to make it be more specific
  to its tasks. I use a pointer to have MouseOver (eventEnter)-effects in the
  Mainwidget.
 *@author Carsten Niehaus
 */

/*This struct store all information about the elements.
 * Name = Name of the element (Helium)
 * Symbol = He
 * Block = s,p,d,f
 * weight = 2.004
 * acidbeh = if the elements tends to build acids, bases, neither or is 
 *              amphoteric
 * az = the state of matter (0 == solid, 1 == liquid, 2 == vapor)
 * date = when has the elements been identified?
 * Group = in which of the 8 groups is the element
 * number = number of the element in the PSE
 * MP = Melting Point
 * BP = Boiling Point
 * EN = Electronegativity
 * Density = Density
 * IE = Ionisationenergie
 * AR = Atomradius
 */

struct ElementInfo
{
   QString Name, Symbol, Block,
   Weight, acidbeh, az, date, Group;
   int number;
   double MP, BP, EN, Density, IE, AR;
};

class ElementKP : public KPushButton  {
    Q_OBJECT
    
    public: 
        ElementKP(const QString &text, QWidget *parent, ElementInfo ElemInfo, const char *name=0, int AElemNo=0, KStatusBar *zeiger=0, Kalzium *Kalzium_tmp=0);
	
	
    	/**
        * used when the mouse enters the pushbutton.
        * We use this function to setFocus() so that QWhatsThis works 
        * for each element.
        */
    	void enterEvent(QEvent *);
	
    	//used when the mouse leaves the pushbutton
    	void leaveEvent(QEvent *);

	Kalzium *kalzium;
        QString neighbourArray[2][2];

        void getNeighbours( int );
	

    	/** loads the name of the fitting element and shows it in
    	* the statusbar.
        */
    	void showName();
	
        
        ElementInfo Data;    
        
        /**
        * ElemNo is the number of the pushbutton AND the elementnumber. 
        * I can now use it to show data and so on.
        */
    	int ElemNo;

    	KStatusBar *zeigerle;
        QLabel *pmTitle, *label;
        QLabel *pmWeight;
        QPopupMenu *pmenu;

    private:
        /**
        *   We use this function to enable drag support for the element buttons.
        *   We parse all the element infos. 
        */
    	void mouseMoveEvent( QMouseEvent * );
        
    	void mouseReleaseEvent( QMouseEvent *mouse );

        QString parseElementInfo();
       
    public slots:
        /**
        * If the user clicks Web Lookup button
        */
        void lookup();
        
        /** 
        * If the user clicks on one button this slot will be called
        */
        void slotShowData();
       
      
};

#endif
