/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *
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


#include "nuclideboardview.h"
#include "nuclideboard.h"
    
	IsotopeView::IsotopeView(QWidget *parent)
: QGraphicsView(parent)
{
    m_scene = new IsotopeScene();
    setScene(m_scene);
    setSceneRect(m_scene->itemsBoundingRect());
    ensureVisible(m_scene->sceneRect());
}

void IsotopeView::resizeEvent(QResizeEvent * event)
{

    ensureVisible(QRectF(0,0,100,100),0,0);

	event->accept();
}

#include "nuclideboardview.moc"

