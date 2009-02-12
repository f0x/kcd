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

#include "tracksdialog.h"
#include "mbmanager.h"

#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTime>
#include <QHeaderView>
#include <QModelIndex>

#include <KDebug>

#include <Plasma/Delegate>
#include <Plasma/Corona>

TracksDialog::TracksDialog(QGraphicsWidget *widget, QWidget *parent) : Plasma::Dialog(parent),
        isMoving(false),
        m_base(new QGraphicsWidget(widget)),
        m_model(new QStandardItemModel(this))
{
    m_treeView = new Plasma::TreeView;

    m_view = m_treeView->nativeWidget();
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView *header = m_view->header();
    header->setResizeMode(QHeaderView::ResizeToContents);
    m_view->setAlternatingRowColors(true); 
    m_treeView->setModel(m_model);
    
    connect(m_view, SIGNAL(clicked(const QModelIndex &)), this, SLOT(playSelected(const QModelIndex &)));

    setResizeHandleCorners(Dialog::All);

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
    layout->addItem(m_treeView);
    m_base->setLayout(layout);

    static_cast<Plasma::Corona*>(widget->scene())->addOffscreenWidget(m_base);
    setGraphicsWidget(m_base);

    //m_treeView->nativeWidget()->setItemDelegate(new Plasma::Delegate(this));

    resize(400,300);
}

TracksDialog::~TracksDialog()
{
}

void TracksDialog::playSelected(const QModelIndex &modelIndex)
{
    int row = modelIndex.row();
    emit changePlayed(row);
}

void TracksDialog::mousePressEvent(QMouseEvent *event)
{
    if (!inControlArea(event->pos())) {
        isMoving = true;
        startPos = event->pos();
    }
    Plasma::Dialog::mousePressEvent(event);
}

void TracksDialog::mouseMoveEvent(QMouseEvent *event)
{ 
    if (isMoving) {
        QPoint offset( event->pos().x()-startPos.x(), event->pos().y()-startPos.y());
        move(pos()+=offset);
    }
    Plasma::Dialog::mouseMoveEvent(event);
}

void TracksDialog::mouseReleaseEvent(QMouseEvent *event)
{ 
    isMoving = false;
    Plasma::Dialog::mouseReleaseEvent(event);
}

void TracksDialog::setTracks(const QList<MBTrackInfo> &tracks)
{

    m_tracks = tracks;
    int number = 1;
   
    QStringList headers;
    headers << i18n("Track") << i18n("Title") << i18n("Duration");
    m_model->setHorizontalHeaderLabels(headers);

    foreach (const MBTrackInfo &track, m_tracks) {
        QTime time;
        time = time.addMSecs((track.Duration).toInt());
        QList<QStandardItem *> items; 
        items << new QStandardItem(QString::number(number));
        items << new QStandardItem(track.Title);
        items << new QStandardItem(time.toString("mm:ss"));
        m_model->appendRow(items);
        number++;
    }


}
