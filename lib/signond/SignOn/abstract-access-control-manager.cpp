/*
 * This file is part of signon
 *
 * Copyright (C) 2011 Intel Corporation.
 *
 * Contact: Elena Reshetova <elena.reshetova@intel.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include "abstract-access-control-manager.h"

using namespace SignOn;


AbstractAccessControlManager::AbstractAccessControlManager(QObject *parent):
    QObject(parent)
{
}

AbstractAccessControlManager::~AbstractAccessControlManager()
{
}

QString AbstractAccessControlManager::keychainWidgetAppId()
{
    return QString();
}

<<<<<<< HEAD
<<<<<<< HEAD
bool AbstractAccessControlManager::isPeerAllowedToAccess(
                                               const QDBusMessage &peerMessage,
=======
=======
>>>>>>> adding ac fixes
bool AbstractAccessControlManager::isPeerAllowedToUseIdentity(const QDBusMessage &peerMessage,
                                                              const QString &securityContext)
{
    Q_UNUSED(peerMessage);
    Q_UNUSED(securityContext);
    return true;
}

bool AbstractAccessControlManager::isPeerOwnerOfIdentity(const QDBusMessage &peerMessage,
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> adding ac fixes
                                               const QString &securityContext)
=======
                                                         const QString &securityContext)
>>>>>>> cleaning up
=======
                                                         const QString &securityContext)
>>>>>>> adding ac fixes
{
    Q_UNUSED(peerMessage);
    Q_UNUSED(securityContext);
    return true;
}

QString
AbstractAccessControlManager::appIdOfPeer(const QDBusMessage &peerMessage)
{
    Q_UNUSED(peerMessage);
    return QString();
}

bool AbstractAccessControlManager::isACLValid(const QDBusMessage &peerMessage,
                                              const QStringList &aclList)
{
    Q_UNUSED(peerMessage);
    Q_UNUSED(aclList);
    return true;
}
