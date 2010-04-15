#ifndef ISOTOPEVIEW_H
#define ISOTOPEVIEW_H
/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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

#include <QWidget>
#include <QGraphicsView>
#include <QResizeEvent>

class IsotopeScene;

class IsotopeView : public QGraphicsView
{
    Q_OBJECT

    public:
        IsotopeView( QWidget * parent = 0);

    private:
	IsotopeScene *m_scene;
    double m_zoomLevel;

    public:
        IsotopeScene* scene(){ 
            return m_scene; 
        }
        double zoomLevel() {
            return m_zoomLevel;
        }
        QPolygonF visibleSceneRect() const {
            return mapToScene( viewport()->rect() );
        }

    signals:
        void zoomLevelChanged( double zoomLevel );
        void visibleSceneRectChanged( const QPolygonF &sceneRect );

    public:
        void setZoom ( double zoom);
        
    protected:
	    void resizeEvent(QResizeEvent * event );
        void mouseMoveEvent( QMouseEvent *event );
        void wheelEvent( QWheelEvent * event );
};
#endif
	
