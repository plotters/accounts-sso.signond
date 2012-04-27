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
 * @file abstract-access-control-manager.h
 * Definition of the AbstractAccessControlManager object.
 * @ingroup Accounts_and_SSO_Framework
 */

#ifndef SIGNON_ABSTRACT_ACCESS_CONTROL_MANAGER_H
#define SIGNON_ABSTRACT_ACCESS_CONTROL_MANAGER_H

#include <SignOn/extension-interface.h>

#include <QString>
#include <QDBusMessage>


namespace SignOn {

/*!
 * @class AbstractAccessControlManager
 * Helps filtering incoming Singnon Daemon requests,
 * based on security priviledges of the client processes.
 * @ingroup Accounts_and_SSO_Framework
 */
<<<<<<< HEAD
class SIGNON_EXPORT AbstractAccessControlManager: public QObject
=======
class SIGNON_EXPORT AbstractAccessControlManager : public QObject
>>>>>>> Merge & cleanup from master
{
    Q_OBJECT

public:
    /*!
     * Constructs a AbstractAccessControlManager object with the given parent.
     * @param parent
     */
    explicit AbstractAccessControlManager(QObject *parent = 0);

    /*!
     * Destructor.
     */
    virtual ~AbstractAccessControlManager();

    /*!
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
     * Checks if a client process is allowed to access objects with a certain
     * security context.
     * The access type to be checked depends on the concrete implementation of
     * this function.
     * @param peerMessage, the request message sent over DBUS by the process.
     * @param securityContext, the securityContext to be checked against.
=======
     * Checks if a client process is allowed to perform operations on specified identity
=======
     * Checks if a client process is allowed to use specified identity.
>>>>>>> cleaning up
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request message sent over DBUS by the process.
<<<<<<< HEAD
     * @param securityContext, the security context of identity to be checked against.
>>>>>>> adding ac fixes
=======
     * @param securityContext, the security context of identity to be checked
     * against.
>>>>>>> Merge & cleanup from master
     * @returns true, if the peer is allowed, false otherwise.
     */
    virtual bool isPeerAllowedToUseIdentity(const QDBusMessage &peerMessage,
                                            const QString &securityContext);
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> Merge & cleanup from master
    /*!
     * Checks if a client process is owner of identify.
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request message sent over DBUS by the process.
     * @param securityContext, the security context of identity to be checked
     * against.
     * @returns true, if the peer is allowed, false otherwise.
     */
<<<<<<< HEAD
=======
     * Checks if a client process is allowed to use specified identity.
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request message sent over DBUS by the process.
=======
     * Checks if a client process is allowed to perform operations on specified identity
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request message sent over DBUS by the process. Identifies the process  itself. 
>>>>>>> adding ac fixes
=======
     * Checks if a client process is allowed to use specified identity.
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request message sent over DBUS by the process.
>>>>>>> cleaning up
     * @param securityContext, the security context of identity to be checked against.
     * @returns true, if the peer is allowed, false otherwise.
     */
<<<<<<< HEAD
    bool isPeerAllowedToUseIdentity(const QDBusMessage &peerMessage,
<<<<<<< HEAD
<<<<<<< HEAD
=======
    virtual bool isPeerAllowedToUseIdentity(const QDBusMessage &peerMessage,
>>>>>>> fixing missing virtual specifiers
                                    const QString &securityContext);
=======
>>>>>>> arg alligment fix
    /*!
     * Checks if a client process is owner of identify.
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request message sent over DBUS by the process.
     * @param securityContext, the security context of identity to be checked against.
     * @returns true, if the peer is allowed, false otherwise.
     */
<<<<<<< HEAD
>>>>>>> adding ac fixes
    bool isPeerOwnerOfIdentity(const QDBusMessage &peerMessage,
=======
    virtual bool isPeerOwnerOfIdentity(const QDBusMessage &peerMessage,
<<<<<<< HEAD
>>>>>>> fixing missing virtual specifiers
=======
    virtual bool isPeerOwnerOfIdentity(const QDBusMessage &peerMessage,
<<<<<<< HEAD
>>>>>>> fixing missing virtual specifiers
                               const QString &securityContext);
=======
=======
                                       const QString &securityContext);
=======
                                    const QString &securityContext);
>>>>>>> cleaning up
    /*!
     * Checks if a client process is owner of identify.
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request message sent over DBUS by the process.
     * @param securityContext, the security context of identity to be checked against.
     * @returns true, if the peer is allowed, false otherwise.
     */
    bool isPeerOwnerOfIdentity(const QDBusMessage &peerMessage,
<<<<<<< HEAD
>>>>>>> adding ac fixes
                                       const QString &securityContext);
>>>>>>> arg alligment fix
=======
                               const QString &securityContext);
>>>>>>> cleaning up
=======
                                       const QString &securityContext);
>>>>>>> arg alligment fix

    /*!
     * Looks up for the application identifier of a specific client process.
     * @param peerMessage, the request message sent over DBUS by the process.
     * @returns the application identifier of the process, or an empty string
     * if none found.
     */
    virtual QString appIdOfPeer(const QDBusMessage &peerMessage);

    /*!
     * @returns the application identifier of the keychain widget
     */
    virtual QString keychainWidgetAppId();
<<<<<<< HEAD
=======

    /*!
        Checks if a client process is allowed to set the specified acl on data item.
        An actual check depends on AC framework being used.
        @param peerMessage, the request message sent over DBUS by the process.
        @param aclList, the acl list to be checked against
        @returns true, if the peer is allowed, false otherwise.
    */
    virtual bool isACLValid(const QDBusMessage &peerMessage,
                            const QStringList &aclList);

    /*!
        Checks if a client process is allowed to set the specified acl on data item.
        An actual check depends on AC framework being used.
        @param peerMessage, the request message sent over DBUS by the process.
        @param aclList, the acl list to be checked against
        @returns true, if the peer is allowed, false otherwise.
    */
    virtual bool isACLValid(const QDBusMessage &peerMessage,
                            const QStringList &aclList);

    /*!
     * Checks if a client process is allowed to set the specified acl on data
     * item.
     * An actual check depends on AC framework being used.
     * @param peerMessage, the request message sent over DBUS by the process.
     * @param aclList, the acl list to be checked against
     * @returns true, if the peer is allowed, false otherwise.
     */
    virtual bool isACLValid(const QDBusMessage &peerMessage,
                            const QStringList &aclList);

<<<<<<< HEAD

>>>>>>> adding ac fixes
=======
>>>>>>> Merge & cleanup from master
};

} // namespace

#endif // SIGNON_ABSTRACT_ACCESS_CONTROL_MANAGER_H
