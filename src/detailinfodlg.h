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
#include "detail_chemical.h"
#include "detail_energy.h"
#include "orbitswidget.h"

class QFrame;
class DetailedGraphicalOverview;
class KalziumDataObject;

class QMouseEvent;



/**
 * @short The dialog which shows all availeble information
 * @author Carsten Niehaus
 */
class DetailedInfoDlg : public KDialogBase
{
    Q_OBJECT
    
	public:
        	DetailedInfoDlg( KalziumDataObject* data, Element *el , QWidget *parent=0, const char *name=0);
	
	private:
		KalziumDataObject *m_data;
		Element *e;
		QFrame *m_pEnergyTab,
			*m_pOverviewTab,
			*m_pPictureTab,
			*m_pChemicalTab,
			*m_pMiscTab,
			*m_pModelTab;
		
		QLabel *piclabel,
			   *discovered_label,
			   *meanweight_label;

		QVBoxLayout *miscLayout, *mainLayout,
					*overviewLayout, *energyLayout,
					*chemicalLayout;

		DetailedGraphicalOverview *dTab;

		detail_chemical *wChemical;
		detail_energy *wEnergy;
		OrbitsWidget *wOrbits;

		/**
		 * create the tabs.
		 */
		void createContent( Element *e );
	
	protected:
		virtual void wheelEvent (  QWheelEvent * ev );
};
#endif
