/*
 *  Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>
 *  Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>
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

#include "kcd.h"
#include "infopanel.h"
#include "tracksdialog.h"
#include "controls.h"
#include "cdhandler.h"
#include "mbmanager.h"
#include "options.h"

#include <QGraphicsLinearLayout>

#include <Plasma/Slider>
#include <Plasma/ExtenderItem>

#include <Phonon/MediaSource>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <Phonon/MediaController>

Kcd::Kcd(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args)
{
   setAspectRatioMode(Plasma::IgnoreAspectRatio);
   setHasConfigurationInterface(false);
   setBackgroundHints(StandardBackground);

   resize(320,235);

   m_mediaObject = new Phonon::MediaObject(this);
   m_audioOutput = new Phonon::AudioOutput(Phonon::NoCategory, this);
   m_audioOutput->setVolume(1);
   m_mediaObject->setTickInterval(1000);
   m_mediaController = new Phonon::MediaController(m_mediaObject);
   Phonon::createPath(m_mediaObject, m_audioOutput);
   
   /** Music Brainz initialisation **/
   m_MBManager = new MBManager();
}
 
Kcd::~Kcd() {
   //delete tracksDialog;
}

void Kcd::init()
{
   layout = new QGraphicsLinearLayout(this);
   layout->setOrientation(Qt::Vertical);
   setLayout(layout);

   m_textPanel = new InfoPanel(this);
   m_controlsPanel = new Controls(this);
   m_optionsPanel = new Options(this);
   m_optionsPanel->setMeterValue(10);
   m_positionSlider = new Plasma::Slider(this);
   m_positionSlider->setOrientation(Qt::Horizontal);
   m_positionSlider->setMinimum(0);
   m_positionSlider->setMaximum(0);
   m_positionSlider->setValue(0);

   setupActions();
  
   Plasma::ExtenderItem* listItem = new Plasma::ExtenderItem(extender());
   listItem->setName("trackslist");
   initExtenderItem(listItem);

   layout->addItem(m_optionsPanel);
   layout->addItem(m_textPanel);
   layout->addItem(m_controlsPanel);
   layout->addItem(m_positionSlider);

   CdHandler *cdHandler = new CdHandler(this);
   connect (cdHandler, SIGNAL(cdInserted(const Phonon::MediaSource&)),
            this, SLOT(handleCd(const Phonon::MediaSource&)));
   cdHandler->checkForPreviousDevices();

}

void Kcd::initExtenderItem(Plasma::ExtenderItem *item) {
   tracksDialog = new TracksDialog(item);
   connect(tracksDialog, SIGNAL(changePlayed(int)), this, SLOT(playSelected(int)));
   //tracksDialog->setPreferredSize(250,375); 
   item->setWidget(tracksDialog);
   item->setTitle("Tracks List");
}

void Kcd::handleCd(const Phonon::MediaSource &mediaSource)
{
   m_mediaObject->setCurrentSource(mediaSource);
   retrieveInformations();
}

void Kcd::retrieveInformations()
{
   m_MBManager->discLookup();
   tracksDialog->setTracks(m_MBManager->getTrackList(), m_MBManager->getDiscInfo());
}

void Kcd::setupActions()
{
   connect(m_mediaObject, SIGNAL(metaDataChanged()), this, SLOT(insertMetaData()));
   connect(m_mediaObject, SIGNAL(tick(qint64)), this, SLOT(updateSlider(qint64)));
   connect(m_mediaObject, SIGNAL(tick(qint64)), this, SLOT(currentTime(qint64)));
   connect(m_controlsPanel, SIGNAL(play()), this, SLOT(play()));
   connect(m_controlsPanel, SIGNAL(pause()), this, SLOT(pause()));
   connect(m_controlsPanel, SIGNAL(stop()), this, SLOT(stop()));
   connect(m_controlsPanel, SIGNAL(previous()), this, SLOT(prev()));
   connect(m_controlsPanel, SIGNAL(next()), this, SLOT(next()));
   connect(m_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekTo(int)));
   connect(m_optionsPanel, SIGNAL(showTrackList()), this, SLOT(viewTrackList()));
   connect(m_optionsPanel, SIGNAL(activeRandom(bool)), this, SLOT(randomEnabled(bool)));
   connect(m_optionsPanel, SIGNAL(activeRepeat(bool)), this, SLOT(repeatEnabled(bool)));
   connect(m_optionsPanel, SIGNAL(volumeActived(bool)), this, SLOT(enableVolume(bool)));
   connect(m_optionsPanel, SIGNAL(volumeChanged(int)), this, SLOT(updateVolume(int)));
}

void Kcd::updateVolume(int value)
{
   m_audioOutput->setVolume(value / 10.0);
}

void Kcd::insertMetaData()
{   
   m_positionSlider->setValue(0);

   const MBTrackInfo trackInfo = m_MBManager->getTrackList()[(m_mediaController->currentTitle()) - 1];

   QMap<QString, QString> metaData;
   metaData["Artist"] = trackInfo.Artist;
   metaData["Title"] = QString::number(m_mediaController->currentTitle()) + " - " + trackInfo.Title;
   metaData["Time"] = trackInfo.Duration;
   metaData["Album"] = m_MBManager->getDiscInfo().Title;
   //kDebug() << metaData;
   m_textPanel->updateMetadata(metaData);
   
   m_positionSlider->setMaximum(trackInfo.Duration.toInt() / 1000);
   currentTime(0);
}

void Kcd::currentTime(qint64 mstime)
{
   QTime time;
   time = time.addMSecs(mstime);

   QTime totalTime;
   totalTime = totalTime.addMSecs(m_mediaObject->totalTime());

   m_textPanel->setCurrentTime(time.toString("mm:ss") + " / " + totalTime.toString("mm:ss"));
}

void Kcd::play()
{
   m_mediaObject->play();
}

void Kcd::pause()
{
   m_mediaObject->pause();
}

void Kcd::stop()
{
   m_mediaObject->stop();
   m_controlsPanel->stateChanged(Controls::Paused);
   currentTime(0);
}

void Kcd::prev()
{
   if (m_mediaController->currentTitle() > 1) {
      m_mediaController->previousTitle();
      currentTime(0);
   }
}

void Kcd::next()
{   
   if (m_mediaController->currentTitle() != m_mediaController->availableTitles()) {
      m_mediaController->nextTitle();
      currentTime(0);
   }
}

void Kcd::seekTo(int value)
{
   m_mediaObject->seek(value * 1000);
}

void Kcd::updateSlider(qint64 seconds)
{
   m_positionSlider->setValue(seconds / 1000);
}

void Kcd::playSelected(int selection)
{
   m_mediaController->setCurrentTitle(selection + 1);
   play();
}

void Kcd::enableVolume(bool active)
{
   m_audioOutput->setMuted(!active);
}

void Kcd::randomEnabled(bool random)
{
   if (random) {
      connect(m_mediaController, SIGNAL(titleChanged(int)), this, SLOT(randomSource(int)));
   } else {
      disconnect(m_mediaController, SIGNAL(titleChanged(int)), this, SLOT(randomSource(int)));
   }
}

void Kcd::repeatEnabled(bool repeat)
{
   if (repeat) {
       connect(m_mediaObject, SIGNAL(aboutToFinish()), this, SLOT(repeatSource()));
   } else {
       disconnect(m_mediaObject, SIGNAL(aboutToFinish()), this, SLOT(repeatSource()));
   }
}

void Kcd::randomSource(int)
{
   disconnect(m_mediaController, SIGNAL(titleChanged(int)), this, SLOT(randomSource(int)));

   int number;

   do {
      number = 1 + qrand() % m_mediaController->availableTitles();
   } while (number == m_mediaController->currentTitle());

   m_mediaController->setCurrentTitle(number);

   connect(m_mediaController, SIGNAL(titleChanged(int)), this, SLOT(randomSource(int)));
}

void Kcd::repeatSource()
{
   m_mediaObject->enqueue(m_mediaObject->currentSource());
}

void Kcd::viewTrackList()
{

}


#include "kcd.moc"
