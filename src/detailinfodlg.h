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

#ifndef DETAILINFODLG_H
#define DETAILINFODLG_H

#include <kdialogbase.h>

#include <QMap>

class DetailedGraphicalOverview;
class Element;
class OrbitsWidget;
class SpectrumViewImpl;

class QStackedWidget;
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
		DetailedInfoDlg( int el, QWidget *parent = 0 );
 
		void setElement( int el );

	private:
		enum DATATYPE
		{
			CHEMICAL = 0,
			MISC,
			ENERGY
		};

		Element            *m_element;
		int                m_elementNumber;

		KActionCollection* m_actionCollection;

		SpectrumViewImpl *m_spectrumview;
		QStackedWidget* m_spectrumStack;
		QLabel* m_spectrumLabel;

		QString isotopeTable();

		DetailedGraphicalOverview *dTab;
		QLabel *piclabel;
		OrbitsWidget *wOrbits;
		QMap<QString, KHTMLPart*> m_htmlpages;

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
