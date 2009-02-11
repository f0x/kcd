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

#include <QMouseEvent>

// Plasma
#include <Plasma/Dialog>
#include <Plasma/TreeView>

class TracksDialog : public Plasma::Dialog
{
    Q_OBJECT

    public:
        TracksDialog(QWidget *parent = 0);
        ~TracksDialog();

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    private:
        bool isMoving;
        QPoint startPos;
        QGraphicsWidget *m_base;

        Plasma::TreeView* m_treeView;
};

#endif

