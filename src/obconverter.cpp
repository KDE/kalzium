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

// OpenBabel includes
#include <openbabel/obconversion.h>

// Qt includes
#include <QRegExp>
#include <QListWidget>
#include <QProcess>
#include <QString>


// KDE includes
#include <kdebug.h>
#include <klocale.h>
#include <kdialog.h>
#include <kpushbutton.h>
#include <kfiledialog.h>
#include <kurl.h>
#include <kmessagebox.h>

using namespace std;
using namespace OpenBabel;

OBConverter::OBConverter( QWidget *parent )
    : KDialog( parent )
{
    setCaption( i18n( "OpenBabel Frontend" ) );
    setButtons( Help | User1| Close );
    setDefaultButton( User1 );
	
    ui.setupUi( mainWidget() );
    
    setButtonGuiItem( User1, KGuiItem( i18n( "Convert" ), "editcopy", i18n( "Convert selected files" ) ) );
    
    setupWindow();
}

void OBConverter::setupWindow()
{
    // Supported format object 
    OBSupportedFormat *supportedFormat = new OBSupportedFormat();

    // Add the supported fileformats to the GUI
    QStringList InputType = supportedFormat->getInputFormat();
    ui.InputTypeComboBox->addItems( InputType );

    QStringList OutputType = supportedFormat->getOutputFormat();
    ui.OutputTypeComboBox->addItems( OutputType );

    // Create connection
    connect(ui.addFileButton, 
            SIGNAL( clicked() ), SLOT( slotAddFile() ));
 
    connect(ui.deleteFileButton, 
            SIGNAL( clicked() ), SLOT( slotDeleteFile() ));
 
    connect(ui.selectAllFileButton, 
            SIGNAL( clicked() ), SLOT( slotSelectAll() ));
 
    connect(this,
            SIGNAL( slotUser1() ), SLOT( slotConvert() ));
}

void OBConverter::slotAddFile()
{
    kDebug() << "OBConverter::slotAddFile()" << endl;
    OBSupportedFormat *supportedFormat = new OBSupportedFormat();
    supportedFormat->setFormatExtensions();
    
    QStringList tmpList = supportedFormat->getInputFormatExtensions();
    tmpList.replaceInStrings( QRegExp("^"), "*." );

    KUrl::List fl = KFileDialog::getOpenUrls( 
            KUrl(), 
            tmpList.join(" ") //add all possible extensions like "*.cml *.mol"
        );

    foreach ( const KUrl& u , fl ) {
        new QListWidgetItem( u.prettyUrl(), ui.FileListView);
    }

    slotGuessInput();
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
    kDebug() << "OBConverter::slotGuessInput()" << endl;
    QList <QListWidgetItem*> p = ui.FileListView->selectedItems ();
    bool first=true;
    QString suffix;
    if( p.count() )
    {
        foreach (QListWidgetItem * item , p ) {
            if (first){
                first=false;
                suffix=item->text().remove(QRegExp("^.*\\."));
            } else {
                if (item->text().remove(QRegExp("^.*\\.")) == suffix) continue;
                else return;
            }
        }
        foreach (QListWidgetItem * item , p ) {
            if (first){
                first=false;
                suffix=item->text().remove(QRegExp("^.*\\."));
            } else {
                if (item->text().remove(QRegExp("^.*\\.")) == suffix) continue;
                else return;
            }

//X If all the files to convert have the same type, then the input type is 
//X automatically selected. Input type are like "type -- description of this
//X type". 

//X //assertion: 'suffix' is the only suffix among the selected files.
//X for(int i=0; i<InputTypeComboBox->count();i++){
//X     if (InputTypeComboBox->text(i).find(QRegExp("^"+suffix+" -- "))>=0){
//X         InputTypeComboBox->setCurrentItem(i);
//X         break;
//X     }
//X }

        }
    }
}

void OBConverter::slotConvert()
{
    //iformat might become "c3d1 -- Chem3D CartChem3D Cartesian 1 format"
    QString iformat = ui.InputTypeComboBox->currentText();
    QString oformat = ui.OutputTypeComboBox->currentText();

    //iformat is now "c3d1"
    iformat=iformat.remove(QRegExp(" --.*"));
    oformat=oformat.remove(QRegExp(" --.*"));

    QList<QListWidgetItem*> p = ui.FileListView->selectedItems();

    if (p.count()==0) {
        KMessageBox::sorry(0,i18n("You must select some files first."));
        return;
    }

    QListIterator<QListWidgetItem*> it( p );
    QStringList cmdList;

    foreach (QListWidgetItem * item, p){
        QString ifname = KUrl( item->text() ).toLocalFile();
        QString ofname = ifname;

        //convert foo.blubb to foo.blibb
        ofname = ofname.replace( "."+iformat, "."+oformat);
        
        bool proceed=true;
        
        if ( QFile::exists(ofname) ) {
            //something named ofname already exists
            switch ( KMessageBox::questionYesNo(0, 
                        i18n( "The file %1 already exist. Do you want to overwrite if possible ?").arg(ofname), 
                        i18n("Authorization")) 
                   )
            {
                case KMessageBox::No:
                    proceed=false;
                    break;
                default:
                    break;
            }
        }
        if (proceed) {
            QString command = QString( "babel -i%1 %2 -o%3 %4" ).arg(iformat,ifname,oformat,ofname);
            cmdList.append( command );
        }
    }
    kDebug() << "cmdList is: " << cmdList << endl;
    switch (KMessageBox::questionYesNoList
            (0,
             i18n("OK to run these commands?"),
             cmdList,
             i18n("List of commands")
            )
           ) 
    {
        case KMessageBox::Yes:
            foreach (const QString s, cmdList) {
                QProcess::startDetached ( "babel", cmdList );
            }
            break;
        default:
            break;
    }
}

OBSupportedFormat::OBSupportedFormat()
{
    OBConversion conv;
    Formatpos pos;
    OBFormat* pFormat = NULL;
    const char* str = NULL;

    while( OBConversion::GetNextFormat(pos, str, pFormat) )
    {
        if(( pFormat->Flags() & NOTWRITABLE) && (pFormat->Flags() & NOTREADABLE) ){
            continue;
        }
        QString Description = pFormat->Description();
        Description.truncate(Description.indexOf("\n"));
        char buf[20];
        snprintf(buf,sizeof(buf),"%s -- ",str);
        Description.prepend(buf);
        if ( !(pFormat->Flags() & NOTREADABLE) ){
            m_InputFormat.append(Description);
        }
        if ( !(pFormat->Flags() & NOTWRITABLE) ){
            m_OutputFormat.append(Description);
        }
    }
}

OBSupportedFormat::~OBSupportedFormat()
{
}

QStringList OBSupportedFormat::getInputFormat() const
{
    return m_InputFormat;
}

QStringList OBSupportedFormat::getOutputFormat() const
{
    return m_OutputFormat;
}

void OBSupportedFormat::setFormatExtensions()
{
    kDebug() << "OBSupportedFormat::setFormatExtensions()" << endl;
    
    foreach (QString s, m_InputFormat) {
        QString tmp = s.remove(QRegExp(" --.*"));

        m_InputFormatExtensions << tmp;
    }

    kDebug() << "m_InputFormatExtensions: " << m_InputFormatExtensions << endl;
}

#include "obconverter.moc"

