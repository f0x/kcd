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
#include "volumecontroller.h"

// Qt
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QRect>
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsItem>

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Theme>

#define HIGH_VOLUME "audio-volume-high"
#define LOW_VOLUME "audio-volume-low"
#define MEDIUM_VOLUME "audio-volume-medium"
#define MUTED_VOLUME "audio-volume-muted"

#define ICON_SIZE 24
#define SEGMENT_WIDTH 3
#define SEGMENT_SPACE 5 // space between segments

VolumeController::VolumeController(Qt::Orientation orientation, QGraphicsItem *parent) : QGraphicsWidget(parent),
                                                                                         m_volume(1.0),
                                                                                         m_icon(new Plasma::IconWidget(this))
{
    m_icon->setIcon(HIGH_VOLUME);

    setOrientation(orientation);

    volumeOn = true;

    connect (Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(updateColors()));
    connect (m_icon, SIGNAL(clicked()), this, SLOT(handleVolume()));
}

VolumeController::~VolumeController()
{}

void VolumeController::handleVolume()
{
    if (volumeOn) {
       m_icon->setIcon(MUTED_VOLUME);
       volumeOn = false;
       emit volumeActived(false);
    } else {
       m_icon->setIcon(HIGH_VOLUME);
       volumeOn = true;
       emit volumeActived(true);
    }
}

void VolumeController::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;
    if (m_orientation == Qt::Horizontal) {
        setMaximumHeight(ICON_SIZE);
    } else {
        setMaximumWidth(ICON_SIZE);
    }

    //update();
}

void VolumeController::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event)

    // updating geometries
    m_segmentsRect = rect().toRect();
    if (m_orientation == Qt::Horizontal) {
        m_segmentsRect.setWidth(rect().width() - ICON_SIZE);
        m_segmentsRect.translate(ICON_SIZE, 0);
        m_icon->setPos(0, 0);
    } else {
        m_segmentsRect.setHeight(rect().height() - ICON_SIZE);
        m_icon->setPos(0, rect().height() - ICON_SIZE);
        // m_segmentsRect.translate(0, 0);
    }

    m_icon->resize(ICON_SIZE, ICON_SIZE);

    update();
}

void VolumeController::updateColors()
{
    update(m_segmentsRect);
}

void VolumeController::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)

    painter->save();
    const QColor fillColor = Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor);

    painter->setPen(fillColor);
    painter->setBrush(fillColor);

    painter->setRenderHint(QPainter::Antialiasing);

    QRect segment = m_segmentsRect;

    // TODO: it has been tested only with vertical mode.. test it
    // also with horizontal!!
    if (m_orientation == Qt::Horizontal) {
        segment.setWidth(SEGMENT_WIDTH);

        const int count = m_segmentsRect.width() / (SEGMENT_SPACE + SEGMENT_WIDTH);
        const int segmentFullSize = segment.height();

        for (int i = 0; i < count*m_volume; i++) {
            segment.setHeight(segmentFullSize * ((float)i / count));
            painter->drawRoundedRect(segment, 20.0, 20.0);
            segment.translate(SEGMENT_SPACE + SEGMENT_WIDTH, 0);
        }
    } else { // the widget is vertical
        segment.setHeight(SEGMENT_WIDTH);
        segment.translate(0, m_segmentsRect.height() * (1.0 - m_volume));

        const int count = m_segmentsRect.height() / (SEGMENT_SPACE + SEGMENT_WIDTH);
        const int segmentFullSize = segment.width();

        const int max = count * m_volume;
        for (int i = max; i >= 0; i--) {
            segment.setWidth(segmentFullSize * ((float)i / count));
            painter->drawRoundedRect(segment, 20.0, 20.0);
            segment.translate(0, SEGMENT_SPACE + SEGMENT_WIDTH);
        }
    }

    painter->restore();
}

void VolumeController::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_segmentsRect.contains(event->pos().toPoint())) {
        QGraphicsWidget::mousePressEvent(event);
        return;
    }

    if (m_orientation == Qt::Horizontal) {
        m_volume = (qreal) m_segmentsRect.width() / (event->pos().toPoint().x() + ICON_SIZE);
    } else { // vertical
        m_volume = (qreal) (m_segmentsRect.height() - event->pos().toPoint().y()) / m_segmentsRect.height();
    }

    emit volumeChanged(m_volume);
    update(m_segmentsRect);
}

void VolumeController::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{}

void VolumeController::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_segmentsRect.height() < event->pos().y() || event->pos().y() < 0.0) {
        QGraphicsWidget::mousePressEvent(event); 
        return;
    }

    if (m_orientation == Qt::Horizontal) {
        m_volume = (qreal) m_segmentsRect.width() / (event->pos().toPoint().x() + ICON_SIZE);
    } else { // vertical
        m_volume = (qreal) (m_segmentsRect.height() - event->pos().toPoint().y()) / m_segmentsRect.height();
    }

    emit volumeChanged(m_volume);
    update(m_segmentsRect);
}
