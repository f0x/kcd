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

#ifndef PREVIEWER_H
#define PREVIEWER_H


#include <QWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QStyleOptionViewItem>

#include <Plasma/PopupApplet>
#include <Plasma/Svg>

namespace Phonon {
    class MediaSource;
    class MediaObject;
    class AudioOutput;
    class MediaController;
};

namespace Plasma {
    class Slider;
    class ExtenderItem;
}

class InfoPanel;
class Controls;
class Options;
class MBManager;
class TracksDialog;

// here we define the applet
class Kcd : public Plasma::PopupApplet
{
    Q_OBJECT

    public:
       Kcd(QObject* parent, const QVariantList &args);
       ~Kcd();
       void init();

       QGraphicsWidget *graphicsWidget();
       void initExtenderItem(Plasma::ExtenderItem *item);

    public slots:
       void play(); 
       void pause();
       void stop();  
       void prev(); 
       void next(); 

    protected:
       void setupActions();

    private:
       InfoPanel* m_textPanel;
       Controls* m_buttonPanel;
       Options* m_optionsPanel;
       Plasma::Slider* m_positionSlider;
       Phonon::MediaObject* m_mediaObject;
       Phonon::AudioOutput* m_audioOutput;
       Phonon::MediaController* m_mediaController;
       MBManager* m_MBManager;
       QGraphicsWidget *m_graphicsWidget;
       TracksDialog *m_tracksDialog;


    private slots:
       void handleCd(const Phonon::MediaSource &);
       void metaData();
       void retrieveInformations();
       void currentTime(qint64);
       void updateSlider(qint64);
       void seekTo(int);
       void viewTrackList();
       void playSelected(int);
       void randomEnabled(bool);
       void repeatEnabled(bool);
       void repeatSource();

    signals:
       //void stateChanged(State state);
};

#endif
