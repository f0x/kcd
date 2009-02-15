/*
 *  Copyright 2008 by Francesco Grieco <fgrieco@gmail.com>

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

#ifndef CDHANDLER_H
#define CDHANDLER_H

#include <QObject>

namespace Phonon {
   class MediaSource;
};

class CdHandler : public QObject
{
   Q_OBJECT

   public:
       CdHandler(QObject *parent = 0);
       ~CdHandler();

   void checkForPreviousDevices();

   signals:
       void cdInserted(const Phonon::MediaSource &);
       void cdEjected();

   protected slots:
       void emitCdInserted(const QString &);
};

#endif
