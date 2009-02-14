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

#ifndef TRACKSDIALOG_H
#define TRACKSDIALOG_H

#include "mbmanager.h"

#include <QMouseEvent>
#include <QList>
#include <QGraphicsWidget>

// Plasma
#include <Plasma/Dialog>
#include <Plasma/TreeView>
#include <Plasma/Label>

class QStandardItemModel;
class QModelIndex;

class TracksDialog : public QGraphicsWidget
{
    Q_OBJECT

    public:
        TracksDialog(QGraphicsWidget *parent);
        ~TracksDialog();

        void setTracks(const QList<MBTrackInfo> &tracks, const DiscInfo &info);

//     protected:
//         void mouseMoveEvent(QMouseEvent *event);
//         void mousePressEvent(QMouseEvent *event);
//         void mouseReleaseEvent(QMouseEvent *event);

    private:
        //bool isMoving;
        //QPoint startPos;
//         QGraphicsWidget *m_base;

        Plasma::TreeView* m_treeView;
        QTreeView*  m_view;

        QStandardItemModel *m_model;
        QList<MBTrackInfo> m_tracks;
        DiscInfo m_info;

        Plasma::Label *m_labelArtist;
        Plasma::Label *m_labelAlbum;

    private slots:
        void playSelected(const QModelIndex &);

    signals:
        void changePlayed(int);

};

#endif

