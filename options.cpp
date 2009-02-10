/*
 * Copyright 2009  Francesco Grieco <fgrieco@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
*/

#include "options.h"

#include <Plasma/IconWidget>
#include <QGraphicsLinearLayout>
#include <KDebug>

Options::Options(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
      m_tracklist(new Plasma::IconWidget(this)),
      m_random(new Plasma::IconWidget(this)),
      m_loop(new Plasma::IconWidget(this))
{
   m_tracklist->setIcon("format-list-unordered");
   connect(m_tracklist, SIGNAL(clicked()), this, SLOT(playPauseClicked()));
   m_tracklist->setMinimumSize(m_tracklist->sizeFromIconSize(10));

   m_random->setIcon("roll");
   connect(m_random, SIGNAL(clicked()), this, SIGNAL(stop()));
   m_random->setMinimumSize(m_random->sizeFromIconSize(10));

   m_loop->setIcon("object-rotate-right");
   connect(m_loop, SIGNAL(clicked()), this, SIGNAL(previous()));
   m_loop->setMinimumSize(m_loop->sizeFromIconSize(10));

   QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
   layout->setOrientation(Qt::Vertical);
   layout->addItem(m_tracklist);
   layout->addItem(m_random);
   layout->addItem(m_loop);
   layout->setSpacing(20);
   setLayout(layout);
}

Options::~Options()
{
}
