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

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Theme>

#define HIGH_VOLUME "audio-volume-high"
#define LOW_VOLUME "audio-volume-low"
#define MEDIUM_VOLUME "audio-volume-medium"
#define MUTED_VOLUME "audio-volume-muted"

#define ICON_SIZE 32
#define SEGMENT_WIDTH 3
#define SEGMENT_SPACE 7 // space between segments

VolumeController::VolumeController(Qt::Orientation orientation, QGraphicsItem *parent) : QGraphicsWidget(parent),
                                                                                         m_volume(1.0),
                                                                                         m_icon(new Plasma::IconWidget(this)),
{
    m_icon->setIcon(HIGH_VOLUME);

    setOrientation(orientation);

    connect (Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(updateColors()));
}

VolumeController::~VolumeController()
{}

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
    m_segmentsRect = rect();
    if (m_orientation == Qt::Horizontal) {
        m_segmentsRect.setWidth(rect().width() - ICON_SIZE);
        m_segmentsRect.translate(ICON_SIZE, 0);
    } else {
        m_segmentsRect.setHeight(rect().height() - ICON_SIZE);
        // m_segmentsRect.translate(0, 0);
    }

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

    QRect segment;
    QRect activeSegmentsRect;

    if (m_orientation == Qt::Horizontal) {
    }
}
