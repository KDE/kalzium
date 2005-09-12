#ifndef KDEEDUGLOSSARY_H
#define KDEEDUGLOSSARY_H
/***************************************************************************

    copyright            : (C) 2005 by Carsten Niehaus
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


#include <khtml_part.h>
#include <kdialogbase.h>

class QChar;
class QDomDocument;
class Q3ListViewItem;
class KListView;
class KListViewSearchLine;
class KActionCollection;
class GlossaryItem;

/**
 * @class Glossary
 * @author Carsten Niehaus
 * @author Pino Toscano
 *
 * This class stores all items to be displayed. It also
 * has access-methods to the items
 */
class Glossary
{
	public:
		/**
		 * Creates a new glossary and add contents from an XML file.
		 * Use isEmpty() to know if any items were loaded.
		 *
		 * @param url the path of the file to load
		 * @param path the path of the pictures
		 */
		Glossary( const KURL& url, const QString& path = 0 );

		/**
		 * Creates a new empty glossary
		 */
		Glossary();

		virtual ~Glossary();

		/**
		 * add the item @p item to the glossary
		 */
		void addItem( GlossaryItem* item ){
			m_itemlist.append( item );
		}

		QList<GlossaryItem*> itemlist()const{
			return m_itemlist;
		}

		/**
		 * clear the Glossary
		 */
		void clear(){
			m_itemlist.clear();
		}

		/**
		 * does this glossary have items?
		 */
		bool isEmpty() const;

		/**
		 * Every glossary can have a name. It will be
		 * set to @p name
		 */
		void setName( const QString& name );

		/**
		 * @returns the name of the glossary
		 */
		QString name()const{
			return m_name;
		}

		/**
		 * sets the internal list of items to @p list
		 */
		void setItemlist( QList<GlossaryItem*> list ){
			m_itemlist = list;
		}

		/**
		 * Every glossaryitem can show pictures. [img src="foo.png]
		 * will look for the file foo.png in the path defined by
		 * @p path
		 */
		void setPicturePath( const QString& path );

		QString picturePath()const{
			return m_picturepath;
		}

		/**
		 * defines which picture to use as the background
		 * of the htmlview. The dialog
		 * will use the file specifiec by the @p filename
		 */
		void setBackgroundPicture( const QString& filename );

		/**
		 * @return the picuture used as the background in 
		 * this background
		 */
		QString backgroundPicture()const{
			return m_backgroundpicture;
		}

	protected:
		void init( const KURL& url, const QString& path );
	
	private:
		/**
		 * This methods parses the given XML code. It will extract
		 * the information of the items and return them as a
		 * QList<GlossaryItem*>
		 */
		virtual QList<GlossaryItem*> readItems( QDomDocument &itemDocument );
		
		QString m_backgroundpicture;

		/**
		 * replaces the [img]-pseudocode with valid HTML. The path where
		 * the pictures are stored will be used for pictures
		 */
		void fixImagePath();

		/**
		 * the path in which pictures of the glossary will be searched
		 */
		QString m_picturepath;
		
		/**
		 * Load the layout from an XML file.
		 *
		 * @param doc The QDomDocument which will contain the read XML
		 *            contents.
		 * @param url The path of the file to load
		 *
		 * @return a bool indicating whether the loading of the XML was
		 *         successfull or not
		 */
		bool loadLayout( QDomDocument& doc, const KURL& url );
	
		QList<GlossaryItem*> m_itemlist;
		
		/**
		 * the name of the glossary
		 */
		QString m_name;
};

/**
 * @class GlossaryItem
 * @author Carsten Niehaus
 *
 * A GlossaryItem stores the information of the content of
 * the item and its name. Furthermore, every item can have 
 * a number of pictures or references associated to it.
 * These are stored as QStringLists.
 */
class GlossaryItem
{
	public:
		GlossaryItem(){};
		~GlossaryItem(){};

		void setName( const QString& s ){
			m_name = s;
		}

		void setDesc( const QString& s){
			m_desc = s;
		}

		void setRef( const QStringList& s){
			m_ref = s;
		}
	
		void setPictures( const QString& s ){
			m_pic = QStringList(s);
		}

		QString name() const {
			return m_name;
		}
		
		QString desc() const {
			return m_desc;
		}
		
		QStringList ref() const {
			return m_ref;
		}
		
		QStringList pictures() const {
			return m_pic;
		}
		
		/**
		 * @return the formated HTML code for current item.
		 */
		QString toHtml() const;

		/**
		 * This method parses the references.
		 * @return the HTML code with the references as HTML links
		 */
		QString parseReferences() const;

	private:
		QString m_name;
		QString m_desc;
		QStringList m_ref;
		QStringList m_pic;
};

/**
 * @class GlossaryDialog
 * @author Pino Toscano
 * @author Carsten Niehaus
 */
class GlossaryDialog : public KDialogBase
{
	Q_OBJECT

	public:
		/**
		 * Creates a new dialog for a glossary.
		 *
		 * @param folded whether to fold the various items in subtrees depending on the
		 * first letter of every item
		 * @param parent the parent of the new dialog
		 * @param name the optional name of the new dialog
		 */
		GlossaryDialog( bool folded = true, QWidget *parent=0, const char *name=0);
		~GlossaryDialog();

		/**
		 * Add a new glossary.
		 *
		 * @param newgloss the new glossary to add
		 */
		void addGlossary( Glossary* newgloss );

	protected:
		void keyPressEvent(QKeyEvent*);

	private:
		QList<Glossary*> m_glossaries;

		/**
		 * if true the items will be displayed folded
		 */
		bool m_folded;

		void updateTree();

		KHTMLPart *m_htmlpart;
		KListView *m_glosstree;
		QString m_htmlbasestring;

		KActionCollection* m_actionCollection;

		Q3ListViewItem* findTreeWithLetter( const QChar&, Q3ListViewItem* );

		KListViewSearchLine *m_search;

	private slots:
		void slotClicked( Q3ListViewItem * );
		/**
		 * The user clicked on a href. Find and display the right item
		 */
		void displayItem( const KURL& url, const KParts::URLArgs& args );
};

#endif // KDEEDUGLOSSARY_H

