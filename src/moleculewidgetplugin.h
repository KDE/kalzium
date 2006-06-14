#ifndef MOLECULEWIDGETPLUGIN_H
#define MOLECULEWIDGETPLUGIN_H
#include <QDesignerCustomWidgetInterface>

class MoleculeWidgetPlugin : public QObject,
							 public QDesignerCustomWidgetInterface
{
	Q_OBJECT
		Q_INTERFACES( QDesignerCustomWidgetInterface )

	public:
		MoleculeWidgetPlugin( QObject *parent = 0 );

		bool isContainer() const;
		bool isInitialized() const;
		QIcon icon() const;
		QString domXml() const;
		QString group() const;
		QString includeFile() const;
		QString name() const;
		QString toolTip() const;
		QString whatsThis() const;
		QWidget *createWidget( QWidget *parent );
		void initialize( QDesignerFormEditorInterface *core );

	private:
		bool initialized;
};
#endif // MOLECULEWIDGETPLUGIN_H
