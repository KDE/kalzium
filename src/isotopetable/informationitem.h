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

#ifndef INFORMATIONITEM_H
#define INFORMATIONITEM_H

#include <QGraphicsRectItem>

class IsotopeItem;

class QGraphicsTextItem;

class InformationItem : public QGraphicsRectItem
{
public:
    enum { Type = UserType + 2 };

    /**
     * @param isotope The Isotope represented
     */
    InformationItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

    /**
     * @return the Type of the item
     */
    int type() const override {
        return Type;
    }

    void setIsotope(IsotopeItem *item);

private:
    QGraphicsTextItem *m_textitem;
};

#endif // INFORMATIONITEM_H
