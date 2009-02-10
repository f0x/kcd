/*
 *  Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>

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

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>

#include <Phonon/MediaSource>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <Phonon/MediaController>

// Plasma
#include <Plasma/Theme>

#include <KDebug>


Kcd::Kcd(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
      m_textPanel(new InfoPanel),
      m_buttonPanel(new Controls)
{
    //setBackgroundHints(DefaultBackground);
    resize(300, 200); // ideal planar size
    m_textPanel->show();
    QGraphicsGridLayout* layout = new QGraphicsGridLayout();
    layout->addItem(m_textPanel, 0, 0);
    m_buttonPanel->show();
    m_buttonPanel->setDisplayedButtons(Controls::AllButtons);
    layout->addItem(m_buttonPanel, 1, 0);
    setLayout(layout);

    m_mediaObject = new Phonon::MediaObject(this);
    m_audioOutput = new Phonon::AudioOutput(Phonon::NoCategory, this);
    m_mediaController = new Phonon::MediaController(m_mediaObject);
    Phonon::createPath(m_mediaObject, m_audioOutput);

    //connect (m_mediaObject, SIGNAL(metaDataChanged()), this, SLOT(metaData()));

    CdHandler *handler = new CdHandler(this);
    connect (handler, SIGNAL(cdInserted(const Phonon::MediaSource&)),
             this, SLOT(handleCd(const Phonon::MediaSource&)));
    handler->checkForPreviousDevices();
}

Kcd::~Kcd()
{
}

void Kcd::handleCd(const Phonon::MediaSource &mediaSource)
{
   m_mediaObject->setCurrentSource(mediaSource);
   //kDebug() << QString::number(m_mediaController->availableTitles());
   retrieveInformations();
}

void Kcd::init()
{
   setupActions();
}

void Kcd::retrieveInformations()
{
   //QMultiMap<QString, QString> cdData = m_mediaObject->metaData();
   //kDebug() << cdData;
   QStringList titles;
   kDebug() << QString::number(m_mediaController->availableTitles());
   for (int i = 1; i <= m_mediaController->availableTitles(); i++) {
       m_mediaController->nextTitle();
       titles << m_mediaObject->metaData(Phonon::TitleMetaData);
//        kDebug() << titles[i];
    }
    kDebug() << titles;
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
}

void Kcd::prev()
{
   m_mediaController->previousTitle();
}

void Kcd::next()
{   
   m_mediaController->nextTitle();
}

void Kcd::setupActions()
{
    connect(m_buttonPanel, SIGNAL(play()), this, SLOT(play()));
    connect(m_buttonPanel, SIGNAL(pause()), this, SLOT(pause()));
    connect(m_buttonPanel, SIGNAL(stop()), this, SLOT(stop()));
    connect(m_buttonPanel, SIGNAL(previous()), this, SLOT(prev()));
    connect(m_buttonPanel, SIGNAL(next()), this, SLOT(next()));
    //connect(this, SIGNAL(stateChanged(State)),
    //        m_buttonPanel, SLOT(stateChanged(State)));
}

void Kcd::metaData()
{   
   
   retrieveInformations();
}

K_EXPORT_PLASMA_APPLET(kcd, Kcd)

#include "plasma-kcd.moc"
