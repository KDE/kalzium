/***************************************************************************

        detailedinfodlg.h  -  description
                             -------------------
    begin                : Tue Apr 2 20:43:44 2002 UTC
    copyright            : (C) 2003, 2004 by Carsten Niehaus
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

#ifndef _DETAILINFODLG_H_
#define _DETAILINFODLG_H_

#include <kdialogbase.h>
#include "element.h"

class QFrame;

/**
 * @short The widget which displays the most important information
 * in one widget like a lot people know it from school
 * @author Carsten Niehaus
 */
class DetailedTab : public QWidget
{
	Q_OBJECT

	public:
		DetailedTab( Element *el, QWidget *parent, const char *name=0 );

		/**
		 * @return the QColor of the block
		 */
		QColor PSEColor( const QString &Block ) const;
		
		/**
		 * draw the small symbol which symbolises the
		 * relevence for humans
		 */
		void drawBiologicalSymbol( QPainter *p );
	
		//calculation of the corners
		int x1,x2,y1,y2,h_t;
	private:
		Element *e;

	protected:
		virtual void paintEvent( QPaintEvent* );
};


/**
 * @short The dialog which shows all availeble information
 * @author Carsten Niehaus
 */
class DetailedInfoDlg : public KDialogBase
{
    Q_OBJECT
    
	public:
        	DetailedInfoDlg( Element *el , QWidget *parent=0, const char *name=0);
	
	private:
		Element *e;
		QFrame *m_pEnergyTab,
			*m_pOverviewTab,
			*m_pPictureTab,
			*m_pChemicalTab,
			*m_pMiscTab,
			*m_pModelTab;

		DetailedTab *dTab;

};
#endif
