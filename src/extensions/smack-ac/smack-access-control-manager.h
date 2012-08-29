/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of signon
 *
 * Copyright (C) 2011-2012 Intel Corporation.
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
/*!
 * @file smack-access-control-manager.h
 * Smack implementation of AbstractAccessControlManager
 * @ingroup Accounts_and_SSO_Framework
 */

#ifndef SMACK_ACCESS_CONTROL_MANAGER_H
#define SMACK_ACCESS_CONTROL_MANAGER_H

#include <QDBusMessage>
#include <QStringList>

#include <SignOn/AbstractAccessControlManager>

/*!
 * @class SmackAccessControlManager
 * Smack implementation of AbstractAccessControlManager
 * ingroup Accounts_and_SSO_Framework
 */
class SmackAccessControlManager: public SignOn::AbstractAccessControlManager
{
    Q_OBJECT

public:
    SmackAccessControlManager(QObject *parent = 0);
    virtual ~SmackAccessControlManager();

    bool isPeerAllowedToUseIdentity(
                                const QDBusMessage &peerMessage,
                                const QString &applicationContext,
                                const SignOn::SecurityContext &securityContext);

    bool isPeerOwnerOfIdentity(const QDBusMessage &peerMessage,
                               const QString &applicationContext,
                               const SignOn::SecurityContext &securityContext);

    QString appIdOfPeer(const QDBusMessage &peerMessage);

    QString keychainWidgetAppId();

    bool isACLValid(const QDBusMessage &peerMessage,
                    const QString &applicationContext,
                    const SignOn::SecurityContextList &aclList);

};

#endif // SMACK_ACCESS_CONTROL_MANAGER_H
