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

GlossaryDialog::GlossaryDialog( QWidget *parent, const char *name)
    : KDialogBase( Plain, i18n( "Glossary" ), Close, Close, parent, name, false )
{
	QString baseHtml = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	baseHtml.append("kalzium/data/");
	baseHtml.append("bg.jpg");

	m_picbasestring = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	m_picbasestring.append("kalzium/data/knowledgepics/");
	m_picbasestring.prepend( "<img src=\"" );

	
	m_htmlbasestring = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
	m_htmlbasestring.append( baseHtml );
	m_htmlbasestring.append("\">");
	
	QVBoxLayout *vbox = new QVBoxLayout( plainPage(), 0, KDialog::spacingHint() );
	vbox->activate();

	QHBoxLayout *hbox = new QHBoxLayout( 0L, 0, KDialog::spacingHint() );
	hbox->activate();

	QToolButton *clear = new QToolButton( plainPage() );
	clear->setIconSet( SmallIconSet( "locationbar_erase" ) );
	hbox->addWidget( clear );

	QLabel *lbl = new QLabel( plainPage() );
	lbl->setText( i18n( "Search:" ) );
	hbox->addWidget( lbl );

	m_search = new KListViewSearchLine( plainPage(), 0, "search-line" );
	hbox->addWidget( m_search );
	vbox->addLayout( hbox );
	setFocusProxy(m_search);

	QSplitter *vs = new QSplitter( plainPage() );
	vbox->addWidget( vs );
	
	m_glosstree = new KListView( vs, "treeview" );
	m_glosstree->addColumn( "entries" );
	m_glosstree->header()->hide();
	m_glosstree->setFullWidth( true );
	m_glosstree->setRootIsDecorated( true );

	m_search->setListView( m_glosstree );

	m_htmlpart = new KHTMLPart( vs, "html-part" );

	m_actionCollection = new KActionCollection(this);
        KStdAction::quit(this, SLOT(slotClose()), m_actionCollection);

	QDomDocument doc( "foo" );
	QString filename = "knowledge.xml";

	if ( loadLayout( doc, filename ) )
		m_itemList = readItems( doc );

	QDomDocument doc2( "foo" );
	filename = "tools.xml";

	if ( loadLayout( doc2, filename ) )
		m_toolList = readTools( doc2 );

	populateTree();

	connect( m_htmlpart->browserExtension(), SIGNAL( openURLRequestDelayed( const KURL &, const KParts::URLArgs & ) ), this, SLOT( displayItem( const KURL &, const KParts::URLArgs & ) ) );
	connect( m_glosstree, SIGNAL(clicked( QListViewItem * )), this, SLOT(slotClicked( QListViewItem * )));
	connect( clear, SIGNAL(clicked()), m_search, SLOT(clear()));

	resize( 550, 400 );
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
	// using the "host" part of a kurl as reference
	QString myurl = url.host().lower();
	m_search->setText( "" );
	m_search->updateSearch( "" );
	QListViewItem *found = 0;
	QListViewItemIterator it( m_glosstree );
	QListViewItem *item;
	while ( it.current() )
	{
		item = it.current();
		if ( item->text(0).lower() == myurl )
		{
			found = item;
			break;
		}
		++it;
	}
	slotClicked( found );
}

void GlossaryDialog::populateTree()
{
	QValueList<KnowledgeItem*>::iterator it = m_itemList.begin();
	const QValueList<KnowledgeItem*>::iterator itEnd = m_itemList.end();
	
	QListViewItem *main = new QListViewItem( m_glosstree, i18n( "Main Glossary" ) );
	main->setExpandable( true );
	main->setSelectable( false );
	for ( ; it != itEnd ; ++it )
	{
		QChar thisletter = ( *it )->name().upper()[0];
		QListViewItem *thisletteritem = findTreeWithLetter( thisletter, main );
		if ( !thisletteritem )
		{
			thisletteritem = new QListViewItem( main, thisletter );
			thisletteritem->setExpandable( true );
			thisletteritem->setSelectable( false );
		}
		new QListViewItem( thisletteritem, ( *it )->name() );
	}
	main->sort();

	QValueList<ToolItem*>::iterator itTools = m_toolList.begin();
	const QValueList<ToolItem*>::iterator itEndTools = m_toolList.end();
	
	QListViewItem *maintools = new QListViewItem( m_glosstree, i18n( "Tools" ) );
	maintools->setExpandable( true );
	maintools->setSelectable( false );
	for ( ; itTools != itEndTools ; ++itTools )
	{
		new QListViewItem( maintools, ( *itTools )->name() );
	}
	maintools->sort();

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
	if ( !item )
		return;
	
	QString html = m_htmlbasestring;
	
	/**
	 * The next lines are searching for the correct KnowledgeItem
	 * in the m_itemList. When it is found the HTML will be
	 * generated
	 */
	QValueList<KnowledgeItem*>::iterator it = m_itemList.begin();
	const QValueList<KnowledgeItem*>::iterator itEnd = m_itemList.end();
	bool found = false;
	KnowledgeItem *i = 0;
	while ( !found && it != itEnd )
	{
		if ( ( *it )->name() == item->text( 0 ) )
		{
			i = *it;
			found = true;
		}
		++it;
	}
	if ( found && i )
	{
		html += i->toHtml() + "</body></html>";
		m_htmlpart->begin();
		m_htmlpart->write( html );
		m_htmlpart->end();
		return;
	}

	QValueList<ToolItem*>::iterator itTools = m_toolList.begin();
	const QValueList<ToolItem*>::iterator itEndTools = m_toolList.end();
	ToolItem *iTools = 0;
	while ( !found && itTools != itEndTools )
	{
		if ( ( *itTools )->name() == item->text( 0 ) )
		{
			iTools = *itTools;
			found = true;
		}
		++itTools;
	}
	if ( found && iTools )
	{
		html += iTools->toHtml() + "</body></html>";
		m_htmlpart->begin();
		m_htmlpart->write( html );
		m_htmlpart->end();
	}
}

bool GlossaryDialog::loadLayout( QDomDocument &questionDocument, const QString& filename )
{
        KURL url;
        url.setPath( locate("data", "kalzium/data/"));
	url.setFileName( filename );

        QFile layoutFile( url.path() );

        if (!layoutFile.exists())
		kdDebug() << "no such file: " << layoutFile.name() << endl;

		//TODO really needed?
        if (!layoutFile.open(IO_ReadOnly))
                return false;

        ///Check if document is well-formed
        if (!questionDocument.setContent(&layoutFile))
        {
                kdDebug() << "wrong xml" << endl;
                layoutFile.close();
                return false;
        }
        layoutFile.close();

        return true;
}

QValueList<KnowledgeItem*> GlossaryDialog::readItems( QDomDocument &itemDocument )
{
	QValueList<KnowledgeItem*> list;

	QDomNodeList itemList;
	QDomNodeList refNodeList;
	QDomElement itemElement;
	QStringList reflist;

	itemList = itemDocument.elementsByTagName( "item" );

	const uint num = itemList.count();
	for ( uint i = 0; i < num; ++i )
	{
		reflist.clear();
		KnowledgeItem *item = new KnowledgeItem();
		
		itemElement = ( const QDomElement& ) itemList.item( i ).toElement();
		
		QDomNode nameNode = itemElement.namedItem( "name" );
		QDomNode descNode = itemElement.namedItem( "desc" );
		QDomElement refNode = ( const QDomElement& ) itemElement.namedItem( "references" ).toElement();

		QString desc = descNode.toElement().text();
		desc.replace("[img]", m_picbasestring );
		desc.replace("[/img]", "\" />" );
		desc.replace("[b]", "<b>" );
		desc.replace("[/b]", "</b>" );
		desc.replace("[i]", "<i>" );
		desc.replace("[/i]", "</i>" );

//		kdDebug() << desc << endl;
		
		item->setName( i18n( nameNode.toElement( ).text().utf8() ) );
		item->setDesc( i18n( desc.utf8() ) );
		refNodeList = refNode.elementsByTagName( "refitem" );
		for ( uint it = 0; it < refNodeList.count(); it++ )
		{
			reflist << i18n( refNodeList.item( it ).toElement().text().utf8() );
		}
		reflist.sort();
		item->setRef( reflist );
		
		list.append( item );
	}
	
	return list;
}

QValueList<ToolItem*> GlossaryDialog::readTools( QDomDocument &itemDocument )
{
	QValueList<ToolItem*> list;

	QDomNodeList itemList;
	QDomElement itemElement;

	itemList = itemDocument.elementsByTagName( "tool" );

	const uint num = itemList.count();
	for ( uint i = 0; i < num; ++i )
	{
		ToolItem *item = new ToolItem();
		
		itemElement = ( const QDomElement& ) itemList.item( i ).toElement();
		item->setPicture( itemElement.attribute( "picture" ) );

		QDomNode nameNode = itemElement.namedItem( "name" );
		QDomNode descNode = itemElement.namedItem( "desc" );
		
		item->setName( i18n( nameNode.toElement().text().utf8() ) );
		item->setDesc( i18n( descNode.toElement().text().utf8() ) );
		
		list.append( item );
	}
	
	return list;
}

void GlossaryDialog::slotClose()
{
	emit closed();
	accept();
}

KnowledgeItem::KnowledgeItem()
{
}

KnowledgeItem::~KnowledgeItem()
{
}

QString KnowledgeItem::toHtml() const
{
	QString code = "<h1>" + m_name + "</h1>" + m_desc;

//	QString pic_path = locate("data", "kalzium/data/knowledgepics/");
	if ( !m_ref.isEmpty() )
	{
		QString refcode = parseReferences();
		code += refcode;
	}
	return code;
}

QString KnowledgeItem::parseReferences() const
{
	QString htmlcode = "<h3>" + i18n( "References" ) + "</h3>";
	
	bool first = true;
	for ( uint i = 0; i < m_ref.size(); i++ )
	{
		if ( !first )
			htmlcode += "<br>";
		else
			first = false;
		htmlcode += QString( "<a href=\"item://%1\">%2</a>" ).arg( m_ref[i], m_ref[i] );
	}

	return htmlcode;
}

ToolItem::ToolItem()
{
}

ToolItem::~ToolItem()
{
}

QString ToolItem::toHtml() const
{
	QString code = "<h1>" + m_name + "</h1>";

	if ( !picture().isEmpty() )
	{
		QString pic_path = locate("data", "kalzium/data/toolpics/");
		pic_path += picture();
		if ( QFile::exists( pic_path ) )
		{
			pic_path = "<br><img src=\"" + pic_path + "\" /><br> ";
			code += pic_path;
		}
	}
	code += m_desc;
	return code;
}

#include "glossarydialog.moc"
