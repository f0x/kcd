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
#include "kcdmeter.h"

#include <Plasma/IconWidget>
#include <Plasma/ToolTipManager>

#include <QGraphicsLinearLayout>
#include <KDebug>

#include <KIcon>
#include <KIconLoader>

Options::Options(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
      m_volume(new Plasma::IconWidget(this)),
      m_meter(new KcdMeter(this)),
      m_tracklist(new Plasma::IconWidget(this)),
      m_random(new Plasma::IconWidget(this)),
      m_loop(new Plasma::IconWidget(this))
{
 
   m_volume->setIcon("audio-volume-high");
   //m_volume->setMinimumSize(m_random->sizeFromIconSize(10));
   connect (m_volume, SIGNAL(clicked()), this, SLOT(handleVolume()));

   m_meter->setMinimum(0);
   m_meter->setMaximum(100);
   m_meter->setMeterType(Plasma::Meter::BarMeterHorizontal);

   m_tracklist->setIcon("format-list-unordered");
   m_tracklist->setMinimumSize(m_random->sizeFromIconSize(10));
   connect(m_tracklist, SIGNAL(clicked()), this, SIGNAL(showTrackList()));

   Plasma::ToolTipContent data;
   data.setMainText(i18n("Tracklist"));
   data.setSubText(i18n("View the tracklist"));
   data.setImage(KIcon("format-list-unordered").pixmap(IconSize(KIconLoader::Desktop)));
   Plasma::ToolTipManager::self()->setContent(m_tracklist, data);

   m_random->setIcon("roll");
   m_random->setMinimumSize(m_random->sizeFromIconSize(10));
   connect(m_random, SIGNAL(clicked()), this, SLOT(randomTrack()));
   
   data.setMainText(i18n("Random - off"));
   data.setSubText(i18n("Play random track"));
   data.setImage(KIcon("roll").pixmap(IconSize(KIconLoader::Desktop)));
   Plasma::ToolTipManager::self()->setContent(m_random, data);

   m_loop->setIcon("object-rotate-right");
   m_loop->setMinimumSize(m_loop->sizeFromIconSize(10));
   connect(m_loop, SIGNAL(clicked()), this, SLOT(loopList()));

   data.setMainText(i18n("Repeat - off"));
   data.setSubText(i18n("Play again the tracklist"));
   data.setImage(KIcon("object-rotate-right").pixmap(IconSize(KIconLoader::Desktop)));
   Plasma::ToolTipManager::self()->setContent(m_loop, data);

   QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
   layout->setOrientation(Qt::Horizontal);
   layout->addItem(m_volume);
   layout->addItem(m_meter);
   layout->addItem(m_random);
   layout->addItem(m_loop);
   layout->addItem(m_tracklist);
   layout->setStretchFactor(m_meter, 20);
   layout->setSpacing(0);
   //layout->setMaximumSize(500,50);

   layout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

   setLayout(layout);

   randomFlag = false;
   loopFlag = false;
   volumeFlag = true;
}

Options::~Options()
{
}


void Options::randomTrack()
{
   Plasma::ToolTipContent data;
   data.setSubText(i18n("Play random track"));
   data.setImage(KIcon("roll").pixmap(IconSize(KIconLoader::Desktop)));
   if (randomFlag) {
       data.setMainText(i18n("Random - off"));
       randomFlag = false;
       emit activeRandom(false);
   }
   else {
       data.setMainText(i18n("Random - on"));
       randomFlag = true;
       emit activeRandom(true);
   }

   Plasma::ToolTipManager::self()->setContent(m_random, data);
}

void Options::loopList()
{
   Plasma::ToolTipContent data;
   data.setSubText(i18n("Play again the tracklist"));
   data.setImage(KIcon("object-rotate-right").pixmap(IconSize(KIconLoader::Desktop)));
   
   if (loopFlag) {
       data.setMainText(i18n("Repeat - off"));
       loopFlag = false;
       emit activeRepeat(false);
   }
   else {
       data.setMainText(i18n("Repeat - on"));
       loopFlag = true;
       emit activeRepeat(true);
   }

   Plasma::ToolTipManager::self()->setContent(m_loop, data);
}

void Options::handleVolume()
{
  if (volumeFlag) {
     m_volume->setIcon("audio-volume-muted");
     volumeFlag = false;
     emit volumeActived(false);
  } else {
     m_volume->setIcon("audio-volume-high");
     volumeFlag = true;
     emit volumeActived(true);
  }
}