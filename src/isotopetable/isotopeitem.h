/*
    SPDX-FileCopyrightText: 2007, 2008 Carsten Niehaus <cniehaus@kde.org>

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

#ifndef ISOTOPEITEM_H
#define ISOTOPEITEM_H

#include <QFont>
#include <QGraphicsRectItem>

class Isotope;

/**
 * The class represents the items which is drawn on the QGraphicsScene. Each such item represents on
 * Isotope.
 * @author Carsten Niehaus
 */
class IsotopeItem : public QAbstractGraphicsShapeItem
{
public:
    /**
     * there are several types of decay for an isotope.
     */
    enum IsotopeType { alpha, ec, multiple, bplus, bminus, unknown, stable };

    enum { Type = UserType + 1 };

    /**
     * @param isotope The Isotope represented
     */
    IsotopeItem(Isotope *isotope, qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

    /**
      * @return the Isotope the item represents
      */
    Isotope* isotope() const{
        return m_isotope;
    }

    QRectF boundingRect() const override {
        return m_rect;
    }

    /**
     * @return the Type of the item
     */
    int type() const override {
        return Type;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    IsotopeType m_type;
    Isotope* m_isotope;
    QRectF m_rect;
    QFont m_symbolFont;
    QFont m_otherFont;

    /**
     * @return the IsotopeType of the Isotope
     */
    static IsotopeType getType(Isotope *);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // ISOTOPEITEM_H
