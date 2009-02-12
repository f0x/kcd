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

#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTime>
#include <QHeaderView>
#include <QModelIndex>

#include <KDebug> 
#include <KIcon>
#include <KPushButton>

#include <Plasma/Delegate>
#include <Plasma/Corona>
#include <Plasma/PushButton>

TracksDialog::TracksDialog(QGraphicsWidget *widget, QWidget *parent) : Plasma::Dialog(parent),
        isMoving(false),
        m_base(new QGraphicsWidget(widget)),
        m_model(new QStandardItemModel(this))
{
    m_treeView = new Plasma::TreeView;

    m_view = m_treeView->nativeWidget();
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView *header = m_view->header();
    header->setMovable(false);
    header->setResizeMode(QHeaderView::ResizeToContents);
    m_view->setAlternatingRowColors(true); 
    m_treeView->setModel(m_model);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    //layout->setSpacing(10);
    
    m_labelArtist = new Plasma::Label;
    m_labelAlbum = new Plasma::Label;
   
    Plasma::PushButton *buttonClose = new Plasma::PushButton;
    buttonClose->setText(i18n("Close"));
    KPushButton *nativeButtonClose = buttonClose->nativeWidget();
    nativeButtonClose->setIcon(KIcon("dialog-close"));

    layout->addItem(m_labelArtist);
    layout->addItem(m_labelAlbum);
    layout->addItem(m_treeView);
    layout->addItem(buttonClose);

    m_base->setLayout(layout);

    static_cast<Plasma::Corona*>(widget->scene())->addOffscreenWidget(m_base);
    setGraphicsWidget(m_base);

    resize(350,300);
    setWindowTitle(i18n("Tracklist"));
    setWindowIcon(KIcon("format-list-unordered"));
    setResizeHandleCorners(Dialog::All);

    connect(m_view, SIGNAL(clicked(const QModelIndex &)), this, SLOT(playSelected(const QModelIndex &)));
    connect(buttonClose, SIGNAL(clicked()), this, SLOT(hide()));
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

void TracksDialog::setTracks(const QList<MBTrackInfo> &tracks, const DiscInfo &info)
{

   m_tracks = tracks;
   m_info = info;
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

   m_labelArtist->setText("<b>" + i18n("Artist") + ": </b>" + m_info.Artist);
   m_labelAlbum->setText("<b>" + i18n("Album") + ": </b>  " + m_info.Title);

}
