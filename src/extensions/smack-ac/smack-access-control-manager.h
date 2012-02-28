/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
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
    /*!
     * Constructs a SmackAccessControlManager object with the given parent.
     * @param parent
     */
    SmackAccessControlManager(QObject *parent = 0);

    /*!
     * Destroys a SmackAccessControlManager object.
     */
    ~SmackAccessControlManager();

    // reimplemented virtual methods
    /*!
<<<<<<< HEAD
     * Checks if a client process is allowed to access objects with a certain
     * security context.
     * The access type to be checked is read or execute.
     * @param peerMessage, the request message sent over DBUS by the process.
     * @param securityContext, the securityContext to be checked against.
     * @returns true, if the peer is allowed, false otherwise.
     */
    bool isPeerAllowedToAccess(const QDBusMessage &peerMessage,
                               const QString &securityContext);
=======
     * Checks if a client process is allowed to perform operations on specified identity
     * The access type to be checked is execute. 
     * @param peerMessage, the request message sent over DBUS by the process. Identifies the process  itself. 
     * @param securityContext, the security context of identity to be checked against.
     * @returns true, if the peer is allowed, false otherwise.
     */
    bool isPeerAllowedToUseIdentity(const QDBusMessage &peerMessage,
                                    const QString &securityContext);
    /*!
     * Checks if a client process is owner of identify.
     * The access type to be checked is read and write, since the owner can manipulate identity and its parameters fully. 
     * @param peerMessage, the request message sent over DBUS by the process. Identifies the process  itself. 
     * @param securityContext, the security context of identity to be checked against.
     * @returns true, if the peer is allowed, false otherwise.
     */
    bool isPeerOwnerOfIdentity(const QDBusMessage &peerMessage,
<<<<<<< HEAD
                                       const QString &securityContext);
>>>>>>> adding ac fixes
=======
                               const QString &securityContext);
>>>>>>> cleaning up

    /*!
     * Looks up for the application identifier of a specific client process.
     * @param peerMessage, the request message sent over DBUS by the process.
     * @returns the application identifier of the process, or an empty string
     * if none found.
     */
     QString appIdOfPeer(const QDBusMessage &peerMessage);

    /*!
     * @returns the application identifier of the keychain widget
     */
    QString keychainWidgetAppId();

    /*!
     *  Checks if a client process is allowed to set the specified acl on data item.
     *  A valid acl can contain only application identifier (application domain name) or application domain labels.
     *  For example for an application with AppId "Camera", the valid acl entries are "Camera", "Camera::certs", "Camera::keys".
     *  @param peerMessage, the request message sent over DBUS by the process.
     *  @param aclList, the acl list to be checked against
     *  @returns true, if the peer is allowed, false otherwise.
     */
    bool isPeerAllowedToSetACL(const QDBusMessage &peerMessage,
                               const QStringList &aclList);

};

#endif // SMACK_ACCESS_CONTROL_MANAGER_H
