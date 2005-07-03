/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "glossarydialog.h"

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kglobal.h>
#include <klistview.h>
#include <klistviewsearchline.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>

#include <qfile.h>
#include <qlabel.h>
#include <qheader.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qstringlist.h>
#include <qtoolbutton.h>

bool Glossary::loadLayout( QDomDocument &questionDocument, const QString& path, const QString& filename )
{
        KURL url;
        url.setPath( path );
		url.setFileName( filename );

        QFile layoutFile( url.path() );

        if (!layoutFile.exists())
			kdDebug() << "no such file: " << layoutFile.name() << endl;
//X 
//X 		//TODO really needed?
//X         if (!layoutFile.open(IO_ReadOnly))
//X                 return false;
//X 
//X         ///Check if document is well-formed
//X         if (!questionDocument.setContent(&layoutFile))
//X         {
//X                 kdDebug() << "wrong xml" << endl;
//X                 layoutFile.close();
//X                 return false;
//X         }
//X         layoutFile.close();

        return true;
}

QValueList<GlossaryItem*> Glossary::readItems( QDomDocument &itemDocument )
{
	QValueList<GlossaryItem*> list;

//X 	QDomNodeList itemList;
//X 	QDomNodeList refNodeList;
//X 	QDomElement itemElement;
//X 	QStringList reflist;
//X 
//X 	itemList = itemDocument.elementsByTagName( "item" );
//X 
//X 	const uint num = itemList.count();
//X 	for ( uint i = 0; i < num; ++i )
//X 	{
//X 		reflist.clear();
//X 		KnowledgeItem *item = new KnowledgeItem();
//X 		
//X 		itemElement = ( const QDomElement& ) itemList.item( i ).toElement();
//X 		
//X 		QDomNode nameNode = itemElement.namedItem( "name" );
//X 		QDomNode descNode = itemElement.namedItem( "desc" );
//X 		QDomElement refNode = ( const QDomElement& ) itemElement.namedItem( "references" ).toElement();
//X 
//X 		QString desc = descNode.toElement().text();
//X 		desc.replace("[img]", m_picbasestring );
//X 		desc.replace("[/img]", "\" />" );
//X 		desc.replace("[b]", "<b>" );
//X 		desc.replace("[/b]", "</b>" );
//X 		desc.replace("[i]", "<i>" );
//X 		desc.replace("[/i]", "</i>" );
//X 
//X //		kdDebug() << desc << endl;
//X 		
//X 		item->setName( i18n( nameNode.toElement( ).text().utf8() ) );
//X 		item->setDesc( i18n( desc.utf8() ) );
//X 		refNodeList = refNode.elementsByTagName( "refitem" );
//X 		for ( uint it = 0; it < refNodeList.count(); it++ )
//X 		{
//X 			reflist << i18n( refNodeList.item( it ).toElement().text().utf8() );
//X 		}
//X 		reflist.sort();
//X 		item->setRef( reflist );
//X 		
//X 		list.append( item );
//X 	}
//X 	
//X 	return list;
}



GlossaryDialog::GlossaryDialog( QWidget *parent, const char *name)
    : KDialogBase( Plain, i18n( "Glossary" ), Close, Close, parent, name, false )
{
//X 	QString baseHtml = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
//X 	baseHtml.append("kalzium/data/");
//X 	baseHtml.append("bg.jpg");
//X 
//X 	m_picbasestring = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
//X 	m_picbasestring.append("kalzium/data/knowledgepics/");
//X 	m_picbasestring.prepend( "<img src=\"" );
//X 
//X 	
//X 	m_htmlbasestring = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
//X 	m_htmlbasestring.append( baseHtml );
//X 	m_htmlbasestring.append("\">");
//X 	
//X 	QVBoxLayout *vbox = new QVBoxLayout( plainPage(), 0, KDialog::spacingHint() );
//X 	vbox->activate();
//X 
//X 	QHBoxLayout *hbox = new QHBoxLayout( 0L, 0, KDialog::spacingHint() );
//X 	hbox->activate();
//X 
//X 	QToolButton *clear = new QToolButton( plainPage() );
//X 	clear->setIconSet( SmallIconSet( "locationbar_erase" ) );
//X 	hbox->addWidget( clear );
//X 
//X 	QLabel *lbl = new QLabel( plainPage() );
//X 	lbl->setText( i18n( "Search:" ) );
//X 	hbox->addWidget( lbl );
//X 
//X 	m_search = new KListViewSearchLine( plainPage(), 0, "search-line" );
//X 	hbox->addWidget( m_search );
//X 	vbox->addLayout( hbox );
//X 	setFocusProxy(m_search);
//X 
//X 	QSplitter *vs = new QSplitter( plainPage() );
//X 	vbox->addWidget( vs );
//X 	
//X 	m_glosstree = new KListView( vs, "treeview" );
//X 	m_glosstree->addColumn( "entries" );
//X 	m_glosstree->header()->hide();
//X 	m_glosstree->setFullWidth( true );
//X 	m_glosstree->setRootIsDecorated( true );
//X 
//X 	m_search->setListView( m_glosstree );
//X 
//X 	m_htmlpart = new KHTMLPart( vs, "html-part" );
//X 
//X 	m_actionCollection = new KActionCollection(this);
//X         KStdAction::quit(this, SLOT(slotClose()), m_actionCollection);
//X 
//X 	QDomDocument doc( "foo" );
//X 	QString filename = "knowledge.xml";
//X 
//X 	if ( loadLayout( doc, filename ) )
//X 		m_itemList = readItems( doc );
//X 
//X 	QDomDocument doc2( "foo" );
//X 	filename = "tools.xml";
//X 
//X 	if ( loadLayout( doc2, filename ) )
//X 		m_toolList = readTools( doc2 );
//X 
//X 	populateTree();
//X 
//X 	connect( m_htmlpart->browserExtension(), SIGNAL( openURLRequestDelayed( const KURL &, const KParts::URLArgs & ) ), this, SLOT( displayItem( const KURL &, const KParts::URLArgs & ) ) );
//X 	connect( m_glosstree, SIGNAL(clicked( QListViewItem * )), this, SLOT(slotClicked( QListViewItem * )));
//X 	connect( clear, SIGNAL(clicked()), m_search, SLOT(clear()));
//X 
//X 	resize( 550, 400 );
}

GlossaryDialog::~GlossaryDialog()
{
}

void GlossaryDialog::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Return) {
		e->ignore();
	}
	KDialogBase::keyPressEvent(e);
}

void GlossaryDialog::displayItem( const KURL& url, const KParts::URLArgs& )
{
//X 	// using the "host" part of a kurl as reference
//X 	QString myurl = url.host().lower();
//X 	m_search->setText( "" );
//X 	m_search->updateSearch( "" );
//X 	QListViewItem *found = 0;
//X 	QListViewItemIterator it( m_glosstree );
//X 	QListViewItem *item;
//X 	while ( it.current() )
//X 	{
//X 		item = it.current();
//X 		if ( item->text(0).lower() == myurl )
//X 		{
//X 			found = item;
//X 			break;
//X 		}
//X 		++it;
//X 	}
//X 	slotClicked( found );
}

void GlossaryDialog::populateTree()
{
//X 	QValueList<KnowledgeItem*>::iterator it = m_itemList.begin();
//X 	const QValueList<KnowledgeItem*>::iterator itEnd = m_itemList.end();
//X 	
//X 	QListViewItem *main = new QListViewItem( m_glosstree, i18n( "Main Glossary" ) );
//X 	main->setExpandable( true );
//X 	main->setSelectable( false );
//X 	for ( ; it != itEnd ; ++it )
//X 	{
//X 		QChar thisletter = ( *it )->name().upper()[0];
//X 		QListViewItem *thisletteritem = findTreeWithLetter( thisletter, main );
//X 		if ( !thisletteritem )
//X 		{
//X 			thisletteritem = new QListViewItem( main, thisletter );
//X 			thisletteritem->setExpandable( true );
//X 			thisletteritem->setSelectable( false );
//X 		}
//X 		new QListViewItem( thisletteritem, ( *it )->name() );
//X 	}
//X 	main->sort();
//X 
//X 	QValueList<ToolItem*>::iterator itTools = m_toolList.begin();
//X 	const QValueList<ToolItem*>::iterator itEndTools = m_toolList.end();
//X 	
//X 	QListViewItem *maintools = new QListViewItem( m_glosstree, i18n( "Tools" ) );
//X 	maintools->setExpandable( true );
//X 	maintools->setSelectable( false );
//X 	for ( ; itTools != itEndTools ; ++itTools )
//X 	{
//X 		new QListViewItem( maintools, ( *itTools )->name() );
//X 	}
//X 	maintools->sort();

}

QListViewItem* GlossaryDialog::findTreeWithLetter( const QChar& l, QListViewItem* i )
{
	QListViewItem *it = i->firstChild();
	while ( it )
	{
		if ( it->text(0)[0] == l )
			return it;
		it = it->nextSibling();
	}
	return 0;
}

void GlossaryDialog::slotClicked( QListViewItem *item )
{
//X 	if ( !item )
//X 		return;
//X 	
//X 	QString html = m_htmlbasestring;
//X 	
//X 	/**
//X 	 * The next lines are searching for the correct KnowledgeItem
//X 	 * in the m_itemList. When it is found the HTML will be
//X 	 * generated
//X 	 */
//X 	QValueList<KnowledgeItem*>::iterator it = m_itemList.begin();
//X 	const QValueList<KnowledgeItem*>::iterator itEnd = m_itemList.end();
//X 	bool found = false;
//X 	KnowledgeItem *i = 0;
//X 	while ( !found && it != itEnd )
//X 	{
//X 		if ( ( *it )->name() == item->text( 0 ) )
//X 		{
//X 			i = *it;
//X 			found = true;
//X 		}
//X 		++it;
//X 	}
//X 	if ( found && i )
//X 	{
//X 		html += i->toHtml() + "</body></html>";
//X 		m_htmlpart->begin();
//X 		m_htmlpart->write( html );
//X 		m_htmlpart->end();
//X 		return;
//X 	}
//X 
//X 	QValueList<ToolItem*>::iterator itTools = m_toolList.begin();
//X 	const QValueList<ToolItem*>::iterator itEndTools = m_toolList.end();
//X 	ToolItem *iTools = 0;
//X 	while ( !found && itTools != itEndTools )
//X 	{
//X 		if ( ( *itTools )->name() == item->text( 0 ) )
//X 		{
//X 			iTools = *itTools;
//X 			found = true;
//X 		}
//X 		++itTools;
//X 	}
//X 	if ( found && iTools )
//X 	{
//X 		html += iTools->toHtml() + "</body></html>";
//X 		m_htmlpart->begin();
//X 		m_htmlpart->write( html );
//X 		m_htmlpart->end();
//X 	}
}

void GlossaryDialog::slotClose()
{
	emit closed();
	accept();
}

QString GlossaryItem::toHtml() const
{
//X 	QString code = "<h1>" + m_name + "</h1>" + m_desc;
//X 
//X //	QString pic_path = locate("data", "kalzium/data/knowledgepics/");
//X 	if ( !m_ref.isEmpty() )
//X 	{
//X 		QString refcode = parseReferences();
//X 		code += refcode;
//X 	}
//X 	return code;
}

QString KnowledgeItem::parseReferences() const
{
//X 	QString htmlcode = "<h3>" + i18n( "References" ) + "</h3>";
//X 	
//X 	bool first = true;
//X 	for ( uint i = 0; i < m_ref.size(); i++ )
//X 	{
//X 		if ( !first )
//X 			htmlcode += "<br>";
//X 		else
//X 			first = false;
//X 		htmlcode += QString( "<a href=\"item://%1\">%2</a>" ).arg( m_ref[i], m_ref[i] );
//X 	}
//X 
//X 	return htmlcode;
}


#include "kdeeduglossary.moc"
