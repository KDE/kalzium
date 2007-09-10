/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus <cniehaus@kde.org>
 *   Copyright (C) 2006 by Jerome Pansanel                                 *
 *   j.pansanel@pansanel.net
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/
#include "obconverter.h"

// Qt includes
#include <QLinkedList>
#include <QRegExp>
#include <QProcess>

// KDE includes
#include <kdebug.h>
#include <klocale.h>
#include <kdialog.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kurl.h>

using namespace std;
using namespace OpenBabel;

OBConverter::OBConverter( QWidget *parent )
    : KDialog( parent )
{
    setCaption( i18n( "OpenBabel Frontend" ) );
    setButtons( Help | User1| Close );
    setDefaultButton( User1 );
    
    OBConvObject = new OBConversion();
	
    ui.setupUi( mainWidget() );
    
    setButtonGuiItem( User1, KGuiItem( i18n( "Convert" ), "edit-copy", i18n( "Convert selected files" ) ) );
    
    setupWindow();
}

void OBConverter::setupWindow()
{
    // Set multiple selection possible
    ui.FileListView->setSelectionMode( QAbstractItemView::SelectionMode(3) );

    // Creating the main layout
    QStringList InputType;
    vector<string> InputFormat = OBConvObject->GetSupportedInputFormat();
    for( vector<string>::iterator it = InputFormat.begin(); it!=InputFormat.end(); ++it) {
      InputType << QString((*it).c_str());
    }
    ui.InputTypeComboBox->addItems( InputType );

    QStringList OutputType;
    vector<string> OutputFormat = OBConvObject->GetSupportedOutputFormat();
    for( vector<string>::iterator it = OutputFormat.begin(); it!=OutputFormat.end(); ++it) {
      OutputType << QString((*it).c_str());
    }
    ui.OutputTypeComboBox->addItems( OutputType );

    // Create connection
    connect(ui.addFileButton, 
            SIGNAL( clicked() ), SLOT( slotAddFile() ));
 
    connect(ui.deleteFileButton, 
            SIGNAL( clicked() ), SLOT( slotDeleteFile() ));
 
    connect(ui.selectAllFileButton, 
            SIGNAL( clicked() ), SLOT( slotSelectAll() ));
 
    connect(this,
            SIGNAL( user1Clicked() ), SLOT( slotConvert() ));
    
    connect(ui.FileListView,
            SIGNAL( itemSelectionChanged() ), SLOT( slotGuessInput() ));
}

void OBConverter::slotAddFile()
{
    QStringList InputType;
    vector<string> InputFormat = OBConvObject->GetSupportedInputFormat();
    for( vector<string>::iterator it = InputFormat.begin(); it!=InputFormat.end(); ++it) {
        InputType << QString((*it).c_str());

    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // InputType is now something like this:                                                                                    //
    // "acr -- ACR format [Read-only]", "alc -- Alchemy format", "arc -- Accelrys/MSI Biosym/Insight II CAR format [Read-only]" //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QStringList tmpList = InputType;
    tmpList.replaceInStrings( QRegExp("^"), "*." );
    tmpList.replaceInStrings( QRegExp(" -- "), "|" );
    tmpList.replaceInStrings( QRegExp("/"), "\\/" ); //escape all '/' (because of MimeTypes)
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // tmpList is now something like this:                                                                                      //
    // "*.acr|ACR format [Read-only]", "*.alc|Alchemy format"                                                                   //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    KUrl::List fl = KFileDialog::getOpenUrls(
            KUrl(),
            "*.*|All Files\n"+tmpList.join("\n") //add all possible extensions like "*.cml *.mol"
            );

    foreach ( const KUrl& u , fl ) {
        new QListWidgetItem( u.prettyUrl(), ui.FileListView);
    }
}


void OBConverter::slotSelectAll()
{
    ui.FileListView->selectAll();
}


void OBConverter::slotDeleteFile()
{
    QList<QListWidgetItem*> p = ui.FileListView->selectedItems();
    foreach ( QListWidgetItem *item, p) {
        delete item;
    }
}

void OBConverter::slotGuessInput()
{
    QList<QListWidgetItem*> p = ui.FileListView->selectedItems();
    bool first = true;
    QString suffix;
    if( p.count() ) {
        foreach( QListWidgetItem * item, p) {
            if( first ) {
                first = false;
                suffix = item->text().remove(QRegExp("^.*\\."));
            }
            else {
                if( item->text().remove(QRegExp("^.*\\.")) == suffix ) {
                    continue;
                }
                else {
		    // All the file types are not same, set type to default
		    ui.InputTypeComboBox->setCurrentIndex(0);
                    return;
                }
            }
        }
    }
    for( int i = 0; i < ui.InputTypeComboBox->count(); i++) {
        if( ui.InputTypeComboBox->itemText(i).indexOf(QRegExp("^" + suffix + " --")) >=0 ) {
	    ui.InputTypeComboBox->setCurrentIndex(i);
	    return;
	}
    }
    // The suffix has not been found, set type to default
    ui.InputTypeComboBox->setCurrentIndex(0);
}

void OBConverter::slotConvert()
{
    QString iformat = ui.InputTypeComboBox->currentText();
    QString oformat = ui.OutputTypeComboBox->currentText();
    iformat = iformat.remove(QRegExp(" --.*"));
    oformat = oformat.remove(QRegExp(" --.*"));

    QList<QListWidgetItem*> p = ui.FileListView->selectedItems();
    if( p.count() == 0 ) {
        QMessageBox::information(this,i18n("KOpenBabel"),i18n("You must select some files first."));
        return;
    }
    QListIterator<QListWidgetItem*> it( p );
    QStringList cmdList; // Full command
    QLinkedList<QStringList> cmdArgList; // Arguments only
    foreach( QListWidgetItem * item, p) {
        QString ifname = QUrl( item->text() ).toLocalFile();
        QString ofname = ifname;
        ofname = ofname.remove(QRegExp("\\.([^\\.]*$)"));
	ofname = ofname + QString(".") + oformat;

        bool proceed = true;
        
        if( QFile::exists(ofname) ) {
            //something named ofname already exists
            switch( KMessageBox::questionYesNo(
	                this,
  	                i18n( "Overwrite File? -- KOpenBabel" ),
                        i18n( "The file %1 already exists. Do you want to overwrite if possible?", ofname) )
                  ) {
                case QMessageBox::No:
                    proceed = false;
                    break;
                default:
                    break;
            }
        }
        if( proceed ) {
            QStringList arguments;
	    arguments << QString( "-i") + iformat << ifname << QString("-o") + oformat << ofname;
            cmdArgList.append( arguments );
	    cmdList.append( QString("babel ") + arguments.join(" ") );
        }
    }
    if( cmdArgList.count() > 0 ) {
        switch( QMessageBox::question(
                    this, i18n("OK to run these commands? -- KOpenBabel"),
                    cmdList.join("\n"),
	   	    QMessageBox::Yes, QMessageBox::No)
  	      ) {
            case QMessageBox::Yes:
                foreach(const QStringList s, cmdArgList) {
                    QProcess::startDetached( "babel", s);
                }
                break;
            default:
                break;
        }
    }
}

void OBConverter::addFile(const QString &filename)
{
    ui.FileListView->addItem(filename);
}

#include "obconverter.moc"

