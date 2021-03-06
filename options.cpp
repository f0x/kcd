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
//#include "kcdmeter.h"

#include <Plasma/IconWidget>
#include <Plasma/ToolTipManager>
#include <Plasma/Label>
#include <Plasma/Theme>

#include <QGraphicsLinearLayout>
#include <QLabel>
#include <KDebug>

#include <KIcon>
#include <KIconLoader>

Options::Options(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
     // m_volume(new Plasma::IconWidget(this)),
      //m_meter(new KcdMeter(this)),
      //m_tracklist(new Plasma::IconWidget(this)),
      m_albumText(new Plasma::Label(this)),
      m_random(new Plasma::IconWidget(this)),
      m_loop(new Plasma::IconWidget(this))
{
 
   //m_volume->setIcon("audio-volume-high");
   //connect (m_volume, SIGNAL(clicked()), this, SLOT(handleVolume()));
   //m_volume->setMinimumSize(m_random->sizeFromIconSize(10));

   //m_meter->setMinimum(0);
   //m_meter->setMaximum(10);
   //m_meter->setMeterType(Plasma::Meter::BarMeterHorizontal);
   //connect (m_meter, SIGNAL(valueChanged(int)), this, SIGNAL(volumeChanged(int)));

   //m_tracklist->setIcon("format-list-unordered");
   //m_tracklist->setMinimumSize(m_random->sizeFromIconSize(10));
   //connect(m_tracklist, SIGNAL(clicked()), this, SIGNAL(showTrackList()));

   Plasma::ToolTipContent data;
   //data.setMainText(i18n("Tracklist"));
   //data.setSubText(i18n("View the tracklist"));
   //data.setImage(KIcon("format-list-unordered").pixmap(IconSize(KIconLoader::Desktop)));
   //Plasma::ToolTipManager::self()->setContent(m_tracklist, data);

   m_random->setIcon("roll");
   connect(m_random, SIGNAL(clicked()), this, SLOT(randomTrack()));
   
   data.setMainText(i18n("Random - off"));
   data.setSubText(i18n("Play random track"));
   data.setImage(KIcon("roll").pixmap(IconSize(KIconLoader::Desktop)));
   Plasma::ToolTipManager::self()->setContent(m_random, data);

   m_loop->setIcon("object-rotate-right");
   connect(m_loop, SIGNAL(clicked()), this, SLOT(loopList()));

   data.setMainText(i18n("Repeat - off"));
   data.setSubText(i18n("Play again the tracklist"));
   data.setImage(KIcon("object-rotate-right").pixmap(IconSize(KIconLoader::Desktop)));
   Plasma::ToolTipManager::self()->setContent(m_loop, data);

   QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
   layout->setOrientation(Qt::Horizontal);
   //layout->addItem(m_volume);
   //layout->addItem(m_meter);
   layout->addItem(m_albumText);
   layout->addItem(m_random);
   layout->addItem(m_loop);
   layout->setAlignment(m_albumText, Qt::AlignLeft);
   layout->setAlignment(m_random, Qt::AlignRight);
   layout->setAlignment(m_loop, Qt::AlignRight);
   layout->setStretchFactor(m_random, 0);
   layout->setStretchFactor(m_loop, 0);
   setPreferredHeight(35);
   
   //layout->setItemSpacing(0, 50);
  
  
   //layout->addItem(m_tracklist);
   //layout->setStretchFactor(m_meter, 20);
   //layout->setSpacing(0);
   //layout->setAlignment(m_random, Qt::AlignRight);
   //layout->setAlignment(m_loop, Qt::AlignRight);
   //layout->setAlignment(m_tracklist, Qt::AlignRight);
   //layout->setItemSpacing(1, 10);
   setLayout(layout);

   randomFlag = false;
   loopFlag = false;
   //volumeFlag = true;

   
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

void Options::updateAlbumTitle(QString title)
{
   Plasma::Theme *theme = Plasma::Theme::defaultTheme();
   QFont font = theme->font(Plasma::Theme::DefaultFont);
   QFontMetricsF fm(font);
   kDebug() << title;
   m_albumText->setText(fm.elidedText(title, Qt::ElideMiddle, m_albumText->size().width()));
   
   font.setBold(true);
   font.setItalic(true);
   font.setStretch(110);
   m_albumText->nativeWidget()->setFont(font);
   //m_albumText->setText(title);
}

// void Options::handleVolume()
// {
//   if (volumeFlag) {
//      m_volume->setIcon("audio-volume-muted");
//      volumeFlag = false;
//      emit volumeActived(false);
//   } else {
//      m_volume->setIcon("audio-volume-high");
//      volumeFlag = true;
//      emit volumeActived(true);
//   }
// }
// 
// void Options::setMeterValue(int value)
// {
//    m_meter->setValue(value);
// }
