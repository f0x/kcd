/*
 *  Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 
#ifndef VOLUMECONTROLLER_H
#define VOLUMECONTROLLER_H

#include <QGraphicsWidget>
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QRect;
class QGraphicsSceneResizeEvent;
class QGraphicsItem;

namespace Plasma {
    class IconWidget;
}

class VolumeController : public QGraphicsWidget
{
    Q_OBJECT
public:
    VolumeController(Qt::Orientation orientation, QGraphicsItem *parent = 0);
    ~VolumeController();

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    void setOrientation(Qt::Orientation);

private:
    Qt::Orientation m_orientation;
    qreal m_volume;
    Plasma::IconWidget *m_icon;
    QRect m_segmentsRect;

protected slots:
    void updateColors();

protected:
    void resizeEvent(QGraphicsSceneResizeEvent*);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);
};

#endif
