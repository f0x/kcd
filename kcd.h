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

#ifndef KCD_H
#define KCD_H

#include <Plasma/PopupApplet>

namespace Plasma {
    class ExtenderItem;
    class Slider;
};

namespace Phonon {
    class MediaSource;
    class MediaObject;
    class AudioOutput;
    class MediaController;
};

class QGraphicsLinearLayout;
class InfoPanel;
class TracksDialog;
class MBManager;
class Controls;
class Options;
 
class Kcd : public Plasma::PopupApplet
{
   Q_OBJECT
   public:
       Kcd(QObject *parent, const QVariantList &args);
       ~Kcd();

       void init();

   private:
       QGraphicsLinearLayout* layout;
       InfoPanel* m_textPanel;
       Controls* m_controlsPanel;
       Options* m_optionsPanel;
       Plasma::Slider* m_positionSlider;
       TracksDialog* tracksDialog;
       MBManager* m_MBManager;
       Phonon::MediaObject* m_mediaObject;
       Phonon::AudioOutput* m_audioOutput;
       Phonon::MediaController* m_mediaController;

   protected:
       void initExtenderItem(Plasma::ExtenderItem *item);
       void setupActions();

   private slots:
       void handleCd(const Phonon::MediaSource &);
       void retrieveInformations();
       void insertMetaData();
       void currentTime(qint64);
       void seekTo(int);
       void updateSlider(qint64);
       void playSelected(int);
       void viewTrackList();
       void randomEnabled(bool);
       void repeatEnabled(bool);
       void repeatSource();
       void randomSource(int);

   public slots:
       void play(); 
       void pause();
       void stop();  
       void prev(); 
       void next(); 
       void enableVolume(bool);
};
 
K_EXPORT_PLASMA_APPLET(kcd, Kcd)
 
#endif
