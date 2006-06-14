#include "moleculeview.h"
#include "moleculewidgetplugin.h"

#include <QtPlugin>

MoleculeWidgetPlugin::MoleculeWidgetPlugin( QObject *parent )
: QObject( parent )
{
	initialized = false;
}

void MoleculeWidgetPlugin::initialize( QDesignerFormEditorInterface * /* core */ )
{
	if ( initialized )
		return;

	initialized = true;
}

bool MoleculeWidgetPlugin::isInitialized() const
{
	return initialized;
}

QWidget *MoleculeWidgetPlugin::createWidget( QWidget *parent )
{
	return new MoleculeWidget( parent );
}

QString MoleculeWidgetPlugin::name() const
{
	return "MoleculeWidget";
}

QString MoleculeWidgetPlugin::group() const
{
	return "Display Widgets [Examples]";
}

QIcon MoleculeWidgetPlugin::icon() const
{
	return QIcon();
}

QString MoleculeWidgetPlugin::toolTip() const
{
	return "";
}

QString MoleculeWidgetPlugin::whatsThis() const
{
	return "";
}

bool MoleculeWidgetPlugin::isContainer() const
{
	return false;
}

QString MoleculeWidgetPlugin::domXml() const
{
	return "<widget class=\"MoleculeWidget\" name=\"moleculeWidget\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>200</width>\n"
		"   <height>200</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString MoleculeWidgetPlugin::includeFile() const
{
	return "moleculeview.h";
}

Q_EXPORT_PLUGIN2( moleculewidgetplugin, MoleculeWidgetPlugin )
