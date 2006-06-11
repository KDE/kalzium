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
#include <QGLWidget>

#define SPHERE_TESSELATE_SLICES	30
#define SPHERE_TESSELATE_STACKS	30

class QListWidget;

/**
 * This class displays the 3D-view of a molecule
 * @autor Benoit Jacobs
 */
class MoleculeWidget : public QGLWidget
{
	Q_OBJECT

	protected:
		GLuint sphereDisplayList;
	
/*		struct VertexArrayEntry {
			float nx, ny, nz, vx, vy, vz;
		};
*/

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

	protected:
		/**
		 * This method initilized OpenGL
		 */
		void initializeGL();
		void paintGL();
		void resizeGL( int width, int height );
		void mousePressEvent( QMouseEvent * event );
		void mouseReleaseEvent( QMouseEvent * event );
		void mouseMoveEvent( QMouseEvent * event );

		/**
		 * This method...
		 */
		void drawGenericSphere();
		
		/**
		 * This method...
		 * @param x
		 * @param y
		 * @param z
		 * @param radius
		 * @param red
		 * @param green
		 * @param blue
		 */
		void drawSphere( 
				GLdouble x, 
				GLdouble y, 
				GLdouble z, 
				GLdouble radius,
				GLfloat red, 
				GLfloat green, 
				GLfloat blue );
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
