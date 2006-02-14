/***************************************************************************
    begin                : Tue Apr 2 20:43:44 2002 UTC
    copyright            : (C) 2003, 2004, 2005 by Carsten Niehaus
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

#include <qmap.h>

class DetailedGraphicalOverview;
class Element;
class OrbitsWidget;
class SpectrumViewImpl;

class QMouseEvent;
class QFrame;
class QLabel;
class QVBoxLayout;
class KActionCollection;
class KHTMLPart;

/**
 * @short The dialog which shows all available information of an element
 * @author Carsten Niehaus
 */
class DetailedInfoDlg : public KDialogBase
{
	Q_OBJECT

	public:
		DetailedInfoDlg( Element *el , QWidget *parent=0, const char *name=0);
 
		void setElement(Element *el);

	private:
		enum DATATYPE
		{
			CHEMICAL = 0,
			MISC,
			ENERGY
		};
	
		Element            *m_element;
		int m_elementNumber;

		KActionCollection* m_actionCollection;

		SpectrumViewImpl *m_spectrumview;

		QString isotopeTable();

		QMap<QString, KHTMLPart*> m_htmlpages;
		
		QLabel *piclabel;

		QWidgetStack* m_spectrumStack;
		QLabel* m_spectrumLabel;

		DetailedGraphicalOverview *dTab;

		OrbitsWidget *wOrbits;

		/**
		 * Create the inital set of tabs. Used it *ONLY* once in the
		 * constructor.
		 */
		void createContent();
		void reloadContent();

		QString getHtml(DATATYPE);

		QString m_baseHtml;
		QString m_picsdir;

		/**
		 * Add a new HTML page to the dialog.
		 *
		 * @param title The title of the tab, appears above the htmlview
		 * @param icontext The name of the tab, appears belov or instead
		 * of the icon
		 * @param iconname The name of the icon
		 * @returns the pointer to the resulting KHTMLPart, needed for
		 * writing HTML code on it
		 */
		KHTMLPart* addHTMLTab( const QString& title, const QString& icontext, const QString& iconname );
		/**
		 * Change the HTML code in an HTML page.
		 *
		 * @param htmlpart the KHTMLPart to edit
		 * @param htmlcode the HTML code to display
		 */
		void fillHTMLTab( KHTMLPart* htmlpart, const QString& htmlcode );
	
	protected slots:
		virtual void slotUser1();
		virtual void slotUser2();
		/**
		 * invoke the help of the correct chapter
		 */
		virtual void slotHelp();

	protected:
		virtual void wheelEvent (  QWheelEvent * ev );

	signals:
		void elementChanged( int );
};

#endif
