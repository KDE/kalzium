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
#ifndef MOLECULEVIEW_H
#define MOLECULEVIEW_H

#include <kdialog.h>

#include "kalziumglwidget.h"

class KComboBox;
class KPushButton;

#define SPHERE_TESSELATE_SLICES	30
#define SPHERE_TESSELATE_STACKS	30

class QListWidget;

/**
 * This class displays the 3D-view of a molecule
 * @autor Benoit Jacobs
 */
class MoleculeWidget : public KalziumGLWidget
{
	Q_OBJECT

	protected:
		GLuint sphereDisplayList;

		bool isDragging;
		QPoint lastDraggingPosition;
		QPoint deltaDragging;
		GLdouble RotationMatrix[16];

	public:
		/**
		 * Constructor
		 */
		MoleculeWidget( QWidget *parent = 0 );

		/**
		 * Destructor
		 */
		virtual ~MoleculeWidget();
};

/**
 * @author Carsten Niehaus
 */
class MoleculeDialog : public KDialog
{
	Q_OBJECT

	public:
		MoleculeDialog( QWidget  * parent );
		~MoleculeDialog();

	private:
		QString m_path;///to store the path were the molecules are located
		
		QListWidget * m_listView;
		MoleculeWidget *m_moleculeWidget;
		KComboBox *m_detailCombobox;
		KPushButton* m_loadButton;

		void fillList();

	public slots:
		void slotLoadMolecule();
};

#endif // MOLECULEVIEW_H
