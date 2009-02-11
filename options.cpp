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
#include <Plasma/ToolTipManager>

#include <QGraphicsLinearLayout>
#include <KDebug>

#include <KIcon>
#include <KIconLoader>

Options::Options(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
      m_tracklist(new Plasma::IconWidget(this)),
      m_random(new Plasma::IconWidget(this)),
      m_loop(new Plasma::IconWidget(this))
{
   m_tracklist->setIcon("format-list-unordered");
   connect(m_tracklist, SIGNAL(pressed(bool)), this, SLOT(playPauseClicked(bool)));
   m_tracklist->setMinimumSize(m_tracklist->sizeFromIconSize(10));

   Plasma::ToolTipContent data;
   data.setMainText(i18n("Tracklist"));
   data.setSubText(i18n("View the tracklist"));
   data.setImage(KIcon("format-list-unordered").pixmap(IconSize(KIconLoader::Desktop)));
   Plasma::ToolTipManager::self()->setContent(m_tracklist, data);

   m_random->setIcon("roll");
   connect(m_random, SIGNAL(clicked()), this, SIGNAL(stop()));
   m_random->setMinimumSize(m_random->sizeFromIconSize(10));
   data.setMainText(i18n("Random - off"));
   data.setSubText(i18n("Play random track"));
   data.setImage(KIcon("roll").pixmap(IconSize(KIconLoader::Desktop)));
   Plasma::ToolTipManager::self()->setContent(m_random, data);

   m_loop->setIcon("object-rotate-right");
   connect(m_loop, SIGNAL(clicked()), this, SIGNAL(previous()));
   m_loop->setMinimumSize(m_loop->sizeFromIconSize(10));
   data.setMainText(i18n("Repeat - off"));
   data.setSubText(i18n("Play again the tracklist"));
   data.setImage(KIcon("object-rotate-right").pixmap(IconSize(KIconLoader::Desktop)));
   Plasma::ToolTipManager::self()->setContent(m_loop, data);

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
