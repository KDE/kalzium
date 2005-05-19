/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kglobalsettings.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kglobal.h>
#include <kmessagebox.h>

#include <qlabel.h>
#include <qpainter.h>
#include <qimage.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qregexp.h>

#include "glossarydialog.h"
#include "detailinfodlg.h"
#include "orbitswidget.h"
#include "detail_chemical.h"
#include "detail_energy.h"
#include "prefs.h"
#include "isotopewidget.h"
#include "detailedgraphicaloverview.h"

GlossaryDialog::GlossaryDialog( QWidget *parent, const char *name)
    : KDialog(parent,name)
{
	QString m_baseHtml = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	m_baseHtml.append("kalzium/data/");
	m_baseHtml.append("bg.jpg");
	
	QString m_htmlbasestring = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
	m_htmlbasestring.append( m_baseHtml );
	m_htmlbasestring.append("\">");

	QHBoxLayout *hbox = new QHBoxLayout( this );
	
	m_htmlpart = new KHTMLPart();
	connect(  m_htmlpart->browserExtension(), SIGNAL(  openURLRequestDelayed(  const KURL &, const KParts::URLArgs & ) ), this, SLOT(  displayItem( const KURL &, const KParts::URLArgs & ) ) );

	itembox = new QListBox( this, "listbox" );
	connect( itembox, SIGNAL( clicked( QListBoxItem* ) ), this, SLOT(itemClicked( QListBoxItem* ) ) );
	
	
	m_htmlview = new KHTMLView( m_htmlpart, this, "html-view" );
	hbox->addWidget( itembox );
	hbox->addWidget( m_htmlview );
	
	QDomDocument doc( "foo" );
	QString filename = "knowledge.xml";

	if ( loadLayout( doc, filename ) )
		m_itemList = readItems( doc );

	populateList();
}

void GlossaryDialog::displayItem( const KURL& url, const KParts::URLArgs& )
{
	QString url_ =  url.url();
	//as the "url" starts with item:/ I need to take from the 7th letter
	QListBoxItem *item = itembox->findItem( url_.right( url_.length()-6 ) );
	if ( item )
	itemClicked( item );
}

void GlossaryDialog::populateList()
{
	QValueList<KnowledgeItem*>::iterator it = m_itemList.begin();
	const QValueList<KnowledgeItem*>::iterator itEnd = m_itemList.end();
	
	for ( ; it != itEnd ; ++it )
	{
		itembox->insertItem( ( *it )->name() );
	}
	itembox->sort();
	itemClicked( itembox->firstItem() );
}

void GlossaryDialog::itemClicked( QListBoxItem* item )
{
	QString html = m_htmlbasestring;
	
	/**
	 * The next lines are searching for the correct KnowledgeItem
	 * in the m_itemList. When it is found the HTML will be
	 * generated
	 */
	QValueList<KnowledgeItem*>::iterator it = m_itemList.begin();
	const QValueList<KnowledgeItem*>::iterator itEnd = m_itemList.end();
	bool found = false;
	KnowledgeItem *i;
	while ( !found && it != itEnd )
	{
		if ( ( *it )->name() == item->text() )
		{
			i = *it;
			found = true;
		}
		++it;
	}
	
	html.append( itemHtml( i ) );
	html.append( "</body></html>" );
	m_htmlpart->begin();
	m_htmlpart->write( html );
	m_htmlpart->end();
}

QString GlossaryDialog::itemHtml( KnowledgeItem* item )
{

	QString code = "<h1>";
	code.append( item->name() );
	code.append( "</h1>" );

	QString pic_path = locate("data", "kalzium/data/knowledgepics/");
	code.append(  item->desc().replace( "img src=\"" , "img src=\""+pic_path ) );
	if ( !item->ref().isNull() )
	{
		QString refcode = parseReferences( item->ref() );
		code.append( refcode );
	}
	return code;
}

QString GlossaryDialog::parseReferences( const QString& ref )
{
	QString code = ref;

	QString htmlcode = i18n( "<h3>References</h3>" );
	
	int pos, l;
	for ( int num = 0; num < ref.contains( "," ); ++num )
	{
		pos = code.find( "," );
		l = code.length();
		QString tmp = code.left( pos );
		QString new_code = code.right( l-pos-1 );

		htmlcode.append( "<a href=\"item:/" );
		htmlcode.append( tmp );
		htmlcode.append( "\">" );
		htmlcode.append( tmp );
		htmlcode.append( "</a><br>" );

		code = new_code;
	}
	htmlcode.append( "<a href=\"item:/" );
	htmlcode.append( code );
	htmlcode.append( "\">" );
	htmlcode.append( code );
	htmlcode.append( "</a>" );

	return htmlcode;
}

bool GlossaryDialog::loadLayout( QDomDocument &questionDocument, const QString& filename )
{
        KURL url;
        url.setPath( locate("data", "kalzium/data/"));
		url.setFileName( filename );

        QFile layoutFile( url.path() );

        if (!layoutFile.exists())
        {
                KMessageBox::information( 0, i18n("Error"), i18n( "Loading File - Error" ) );
        }

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
	QDomElement itemElement;

	itemList = itemDocument.elementsByTagName( "item" );

	const uint num = itemList.count();
	for ( uint i = 0; i < num; ++i )
	{
		KnowledgeItem *item = new KnowledgeItem();
		
		itemElement = ( const QDomElement& ) itemList.item( i ).toElement();
		
		QDomNode nameNode = itemElement.namedItem( "name" );
		QDomNode descNode = itemElement.namedItem( "desc" );
		QDomNode refNode =  itemElement.namedItem( "ref" );
		
		item->setName( nameNode.toElement( ).text() );
		item->setDesc( descNode.toElement( ).text() );
		item->setRef( refNode.toElement( ).text() );
		
		list.append( item );
	}
	
	return list;
}

void GlossaryDialog::closeEvent(QCloseEvent* e)
{
	QWidget::closeEvent(e);
	emit closed();
}

KnowledgeItem::KnowledgeItem()
{
}

KnowledgeItem::~KnowledgeItem(){}

#include "glossarydialog.moc"
