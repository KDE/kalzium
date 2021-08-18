/*
    SPDX-FileCopyrightText: 2007 Carsten Niehaus <cniehaus@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef ISOTOPEGUIDEVIEW_H
#define ISOTOPEGUIDEVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include <QMouseEvent>

#include "isotopeview.h"

class IsotopeGuideView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit IsotopeGuideView(QWidget *parent = nullptr);
    void setGuidedView(IsotopeView *guidedView);

protected:
    void drawForeground(QPainter *painter, const QRectF &rect) override;

private:
    IsotopeView *m_guidedView;
    double m_zoomLevel;
    double m_scale;
    QPolygonF m_visibleSceneRect;
    QPoint m_lastMousePos;
    bool m_dragEvent;

    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void setZoomLevel(double zoomLevel);
    void setVisibleSceneRect(const QPolygonF &sceneRect);
};

#endif // ISOTOPEGUIDEVIEW_H
