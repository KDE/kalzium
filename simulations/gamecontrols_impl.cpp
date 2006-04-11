/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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
#include "gamecontrols_impl.h"
#include "ui_settings.h"

#include <QDialog>

GameControls_Impl::GameControls_Impl( QWidget * parent )
	: QWidget( parent )
{
	ui.setupUi( this );

	connect( ui.settingsButton, SIGNAL( clicked() ),
			this, SLOT( showSettings() ) );
}

void GameControls_Impl::showSettings()
{
	QDialog *dlg = new QDialog( this );
	Ui::SettingsDialog settingsDlg;
	
	settingsDlg.setupUi( dlg );
	dlg->show();
}

#include "gamecontrols_impl.moc"
