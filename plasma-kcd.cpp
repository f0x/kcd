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

#include "plasma-kcd.h"
#include "infopanel.h"
#include "controls.h"
#include "cdhandler.h"
#include "options.h"
#include "mbmanager.h"
#include "volumecontroller.h"
#include "tracksdialog.h"

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QTime>

#include <Phonon/MediaSource>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <Phonon/MediaController>

// Plasma
#include <Plasma/Theme>
#include <Plasma/Slider>
#include <Plasma/Label>
#include <Plasma/ExtenderItem>
#include <Plasma/Extender>

#include <KDebug>


Kcd::Kcd(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
      m_textPanel(new InfoPanel(this)),
      m_buttonPanel(new Controls(this)),
      m_optionsPanel(new Options(this)),
      m_positionSlider(new Plasma::Slider(this)),
      m_volume(new VolumeController(Qt::Horizontal, this)),
      m_graphicsWidget(0)
{
    //setBackgroundHints(DefaultBackground);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setHasConfigurationInterface(false);
    resize(320, 235); // ideal planar size

    m_mediaObject = new Phonon::MediaObject(this);
    m_audioOutput = new Phonon::AudioOutput(Phonon::NoCategory, this);
    m_mediaController = new Phonon::MediaController(m_mediaObject);
    Phonon::createPath(m_mediaObject, m_audioOutput);
    m_mediaObject->setTickInterval(1000);

    /** Music Brainz initialisation **/
    m_MBManager = new MBManager();

    trackList = new Plasma::ExtenderItem(extender());
    trackList->setName("tracklist");
    trackList->setTitle(i18n("CD Tracks List"));
    initExtenderItem(trackList);
    
}

Kcd::~Kcd()
{
//    delete m_tracksDialog;
}

QGraphicsWidget* Kcd::graphicsWidget()
{
    if (!m_graphicsWidget) {
        m_graphicsWidget = new QGraphicsWidget(this);

        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
        layout->setOrientation(Qt::Vertical);
        QGraphicsLinearLayout* vlayout = new QGraphicsLinearLayout();
        vlayout->setOrientation(Qt::Vertical);
        QGraphicsLinearLayout* hlayout = new QGraphicsLinearLayout();
        hlayout->setOrientation(Qt::Horizontal);
        hlayout->addItem(m_volume);
        hlayout->addItem(m_optionsPanel);
        hlayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        vlayout->addItem(m_textPanel);
        vlayout->addItem(m_buttonPanel);
        vlayout->addItem(m_positionSlider);
        vlayout->insertStretch(1, 10);
        layout->addItem(hlayout);
        layout->addItem(vlayout);
        //layout->addItem(new VolumeController(Qt::Vertical, this), 0, 2);

        m_graphicsWidget->setLayout(layout);
  
        m_positionSlider->setOrientation(Qt::Horizontal);
        m_positionSlider->setMinimum(0);
        m_positionSlider->setMaximum(0);
        m_positionSlider->setValue(0);
 
        //m_graphicsWidget->setMinimumSize(300, 200);
    }

    return m_graphicsWidget;
}

void Kcd::setupActions()
{
   connect(m_mediaObject, SIGNAL(metaDataChanged()), this, SLOT(metaData()));
   connect(m_mediaObject, SIGNAL(tick(qint64)), this, SLOT(updateSlider(qint64)));
   connect(m_mediaObject, SIGNAL(tick(qint64)), this, SLOT(currentTime(qint64)));
   connect(m_buttonPanel, SIGNAL(play()), this, SLOT(play()));
   connect(m_buttonPanel, SIGNAL(pause()), this, SLOT(pause()));
   connect(m_buttonPanel, SIGNAL(stop()), this, SLOT(stop()));
   connect(m_buttonPanel, SIGNAL(previous()), this, SLOT(prev()));
   connect(m_buttonPanel, SIGNAL(next()), this, SLOT(next()));
   connect(m_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekTo(int)));
   connect(m_optionsPanel, SIGNAL(showTrackList()), this, SLOT(viewTrackList()));
   connect(m_optionsPanel, SIGNAL(activeRandom(bool)), this, SLOT(randomEnabled(bool)));
   connect(m_optionsPanel, SIGNAL(activeRepeat(bool)), this, SLOT(repeatEnabled(bool)));
   connect(m_volume, SIGNAL(volumeActived(bool)), this, SLOT(enableVolume(bool)));
}

void Kcd::handleCd(const Phonon::MediaSource &mediaSource)
{
   m_mediaObject->setCurrentSource(mediaSource);
   //kDebug() << QString::number(m_mediaController->availableTitles());
   retrieveInformations();
}

void Kcd::initExtenderItem(Plasma::ExtenderItem *item)
{
    m_tracksDialog = new TracksDialog(item);
    connect(m_tracksDialog, SIGNAL(changePlayed(int)), this, SLOT(playSelected(int)));
    item->setName("tracklist");
    item->setWidget(m_tracksDialog);
}

void Kcd::init()
{
    //static_cast<QGraphicsLinearLayout*>(graphicsWidget()->layout())->addItem(extender());
    setupActions();
    setPopupIcon("media-optical-audio");

    CdHandler *handler = new CdHandler(this);
    connect (handler, SIGNAL(cdInserted(const Phonon::MediaSource&)),
             this, SLOT(handleCd(const Phonon::MediaSource&)));
    handler->checkForPreviousDevices();
}

void Kcd::playSelected(int selection)
{
   m_mediaController->setCurrentTitle(selection + 1);
   play();
}

void Kcd::retrieveInformations()
{
    m_MBManager->discLookup();
    m_tracksDialog->setTracks(m_MBManager->getTrackList(), m_MBManager->getDiscInfo());
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
   m_buttonPanel->stateChanged(Controls::Paused);
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

void Kcd::enableVolume(bool active)
{
   m_audioOutput->setMuted(!active);
}

void Kcd::randomEnabled(bool random)
{
   if (random) {
      //kDebug() << "Random Enabled";
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

   //kDebug() << QString::number(number);
   m_mediaController->setCurrentTitle(number);

   connect(m_mediaController, SIGNAL(titleChanged(int)), this, SLOT(randomSource(int)));
}

void Kcd::repeatSource()
{
   m_mediaObject->enqueue(m_mediaObject->currentSource());
}

void Kcd::viewTrackList()
{
    m_tracksDialog->setVisible(!m_tracksDialog->isVisible());
}

void Kcd::updateSlider(qint64 seconds)
{
    m_positionSlider->setValue(seconds / 1000);
}

void Kcd::seekTo(int value)
{
    //kDebug() << QString::number(m_positionSlider->value());
    m_mediaObject->seek(value * 1000);
}


void Kcd::currentTime(qint64 mstime)
{
    //kDebug() << QString::number(mstime/1000);
    QTime time;
    time = time.addMSecs(mstime);
    //kDebug() << time.toString("mm:ss");

    QTime totalTime;
    totalTime = totalTime.addMSecs(m_mediaObject->totalTime());

    m_textPanel->setCurrentTime(time.toString("mm:ss") + " / " + totalTime.toString("mm:ss"));
}

void Kcd::metaData()
{   
    // kDebug() << QString::number((m_mediaObject->totalTime())/1000);

    m_positionSlider->setValue(0);

    QMap<QString, QString> metaData;
    const MBTrackInfo trackInfo = m_MBManager->getTrackList()[(m_mediaController->currentTitle()) - 1];

    metaData["Artist"] = trackInfo.Artist;
    metaData["Title"] = QString::number(m_mediaController->currentTitle()) + " - " + trackInfo.Title;
    metaData["Time"] = trackInfo.Duration;
    metaData["Album"] = m_MBManager->getDiscInfo().Title;

    m_textPanel->updateMetadata(metaData);

    m_positionSlider->setMaximum(trackInfo.Duration.toInt() / 1000);
    //kDebug() << trackInfo.Duration;
    //m_positionSlider->setMaximum(m_mediaObject->totalTime() / 1000);
    currentTime(0);
}

K_EXPORT_PLASMA_APPLET(kcd, Kcd)

#include "plasma-kcd.moc"
