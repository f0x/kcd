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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QGraphicsWidget>

namespace Plasma {
    class IconWidget;
}

class QGraphicsLinearLayout;

class Options : public QGraphicsWidget
{
    Q_OBJECT
    
    public:
        Options(QGraphicsWidget *parent = 0);
        ~Options();

    private:
        Plasma::IconWidget* m_tracklist;
        Plasma::IconWidget* m_random;
        Plasma::IconWidget* m_loop;
 
        bool randomFlag;
        bool loopFlag;

    private slots:
        void randomTrack();
        void loopList();

    signals:
        void showTrackList();
        void activeRandom(bool);
        void activeRepeat(bool);
};

#endif
