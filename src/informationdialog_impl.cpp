#include "informationdialog_impl.h"

#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>


InformationWidget::InformationWidget( QWidget *parent )
	: InformationDialog( parent )
{
}

void InformationWidget::slotUpdate( QPoint point )
{
	m_topic->setText( getTopic(point) );
	m_explanation->setText( getDesc(point) );
}

QString InformationWidget::getDesc( QPoint point )
{
	QString information;
	if ( buttonGroup->selectedId() == 0 ) //the users looks for groups
		information = "foo bar bar foo groups";
	if ( buttonGroup->selectedId() == 1) //the users looks for groups
		information = "julia julia julia period";

	return information;
}

QString InformationWidget::getTopic( QPoint point )
{
	QString information;
	if ( buttonGroup->selectedId() == 0 ) //the users looks for groups
		information = "foo bar bar foo groups";
	if ( buttonGroup->selectedId() == 1) //the users looks for groups
		information = "julia julia julia period";

	return information;
}
