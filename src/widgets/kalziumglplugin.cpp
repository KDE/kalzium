#include "kalziumglwidget.h"
#include "kalziumglplugin.h"

#include <QtPlugin>

KalziumGLWidgetPlugin::KalziumGLWidgetPlugin( QObject *parent )
: QObject( parent )
{
	initialized = false;
}

void KalziumGLWidgetPlugin::initialize( QDesignerFormEditorInterface * /* core */ )
{
	if ( initialized )
		return;

	initialized = true;
}

bool KalziumGLWidgetPlugin::isInitialized() const
{
	return initialized;
}

QWidget *KalziumGLWidgetPlugin::createWidget( QWidget *parent )
{
	return new KalziumGLWidget( parent );
}

QString KalziumGLWidgetPlugin::name() const
{
	return "KalziumGLWidget";
}

QString KalziumGLWidgetPlugin::group() const
{
	return "Display Widgets [Examples]";
}

QIcon KalziumGLWidgetPlugin::icon() const
{
	return QIcon();
}

QString KalziumGLWidgetPlugin::toolTip() const
{
	return "";
}

QString KalziumGLWidgetPlugin::whatsThis() const
{
	return "";
}

bool KalziumGLWidgetPlugin::isContainer() const
{
	return false;
}

QString KalziumGLWidgetPlugin::domXml() const
{
	return "<widget class=\"KalziumGLWidget\" name=\"KalziumGLWidget\">\n"
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

QString KalziumGLWidgetPlugin::includeFile() const
{
	return "kalziumglwidget.h";
}

Q_EXPORT_PLUGIN2( kalzium_gl_plugin, KalziumGLWidgetPlugin )

#include "kalziumglplugin.moc"
