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
#include <kdebug.h>
#include <klocale.h>

#include "kalziumglwidget.h"

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

//X 	protected:
//X 		/**
//X 		 * This method initilized OpenGL
//X 		 */
//X 		void initializeGL();
//X 		void paintGL();
//X 		void resizeGL( int width, int height );
//X 		void mousePressEvent( QMouseEvent * event );
//X 		void mouseReleaseEvent( QMouseEvent * event );
//X 		void mouseMoveEvent( QMouseEvent * event );
//X 
//X 		/**
//X 		 * This method...
//X 		 */
//X 		void drawGenericSphere();
//X 		
//X 		/**
//X 		 * This method...
//X 		 * @param x
//X 		 * @param y
//X 		 * @param z
//X 		 * @param radius
//X 		 * @param red
//X 		 * @param green
//X 		 * @param blue
//X 		 */
//X 		void drawSphere( 
//X 				GLdouble x, 
//X 				GLdouble y, 
//X 				GLdouble z, 
//X 				GLdouble radius,
//X 				GLfloat red, 
//X 				GLfloat green, 
//X 				GLfloat blue );
};

/**
 * @author Carsten Niehaus
 */
class MoleculeDialog : public KDialog
{
	public:
		MoleculeDialog( QWidget  * parent );
		~MoleculeDialog();

	private:
		QListWidget * m_listView;
		MoleculeWidget *m_moleculeWidget;

		void fillList();
};

#endif // MOLECULEVIEW_H
