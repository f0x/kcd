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

TracksDialog::TracksDialog(QGraphicsWidget *parent) : QGraphicsWidget(parent),
                           m_model(new QStandardItemModel(this))
{
   m_treeView = new Plasma::TreeView(this);
   m_treeView->setModel(m_model);

   m_view = m_treeView->nativeWidget();
   m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
   m_view->setAlternatingRowColors(true); 

   QHeaderView *header = m_view->header();
   header->setHidden(true);   
    
   m_labelArtist = new Plasma::Label;
   m_labelAlbum = new Plasma::Label;

   QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
   layout->addItem(m_labelArtist);
   layout->addItem(m_labelAlbum);
   layout->addItem(m_treeView);
   layout->setSpacing(0);

   setLayout(layout);

   connect(m_view, SIGNAL(clicked(const QModelIndex &)), this, SLOT(playSelected(const QModelIndex &)));
}

TracksDialog::~TracksDialog()
{
}

void TracksDialog::playSelected(const QModelIndex &modelIndex)
{
   int row = modelIndex.row();
   emit changePlayed(row);
}

void TracksDialog::setTracks(const QList<MBTrackInfo> &tracks, const DiscInfo &info)
{
   m_tracks = tracks;
   m_info = info;
   int number = 1;
   
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

   m_view->resizeColumnToContents(0);
   m_view->resizeColumnToContents(1);
   m_view->resizeColumnToContents(2);

}

#include "tracksdialog.moc"
