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

#include "cdhandler.h"

#include <Phonon/MediaObject>
#include <Solid/DeviceNotifier>
#include <Solid/Device>
#include <Solid/DeviceInterface>
#include <Solid/OpticalDisc>
#include <KDebug>

CdHandler::CdHandler(QObject *parent) : QObject(parent)
{
   connect (Solid::DeviceNotifier::instance(), SIGNAL(deviceAdded(const QString&)),
            this, SLOT(emitCdInserted(const QString &)));
}

CdHandler::~CdHandler()
{
}

void CdHandler::checkForPreviousDevices()
{
   foreach (const Solid::Device &device, Solid::Device::allDevices()) {
       emitCdInserted(device.udi());
   }
}

void CdHandler::emitCdInserted(const QString &udi)
{
   Solid::Device device(udi);
      if (!device.is<Solid::OpticalDisc>()) {
          return;
      }

   Solid::OpticalDisc *disc = device.as<Solid::OpticalDisc>();

   if (disc->availableContent() != Solid::OpticalDisc::Audio) {
       return;
   }

   emit cdInserted(Phonon::MediaSource(Phonon::Cd, udi));
}
