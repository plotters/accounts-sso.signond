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
 * @file abstract-access-control-manager.h
 * Definition of the AbstractAccessControlManager object.
 * @ingroup Accounts_and_SSO_Framework
 */

#ifndef SIGNON_ABSTRACT_ACCESS_CONTROL_MANAGER_H
#define SIGNON_ABSTRACT_ACCESS_CONTROL_MANAGER_H

#include <SignOn/extension-interface.h>
#include <SignOn/securitycontext.h>

#include <QString>
#include <QDBusContext>


namespace SignOn {

/*!
 * @class AbstractAccessControlManager
 * Helps filtering incoming Singnon Daemon requests,
 * based on security priviledges of the client processes.
 * @ingroup Accounts_and_SSO_Framework
 */
class SIGNON_EXPORT AbstractAccessControlManager : public QObject
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
     * Checks if a client process is allowed to use specified identity.
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request context of the peer.
     * @param applicationContext, request context within a process.
     * @param securityContext, the security context of identity to be checked
     * against.
     * @returns true, if the peer is allowed, false otherwise.
     */
    virtual bool isPeerAllowedToUseIdentity(
                                        const QDBusMessage &peerMessage,
                                        const QString &applicationContext,
                                        const SecurityContext &securityContext);
    /*! @overload */
    virtual bool isPeerAllowedToUseIdentity(
                                        const QDBusContext &peerContext,
                                        const QString &applicationContext,
                                        const SecurityContext &securityContext)
    {
        return isPeerAllowedToUseIdentity(peerContext.message(),
                                          applicationContext,
                                          securityContext);
    }

    /*!
     * Checks if a client process is owner of identify.
     * The actual check depends on AC framework being used.   
     * @param peerMessage, the request context of the peer.
     * @param applicationContext, request context within a process.
     * @param securityContext, the security context of identity to be checked
     * against.
     * @returns true, if the peer is allowed, false otherwise.
     */
    virtual bool isPeerOwnerOfIdentity(const QDBusMessage &peerMessage,
                                       const QString &applicationContext,
                                       const SecurityContext &securityContext);
    /*! @overload */
    virtual bool isPeerOwnerOfIdentity(const QDBusContext &peerContext,
                                       const QString &applicationContext,
                                       const SecurityContext &securityContext)
    {
        return isPeerOwnerOfIdentity(peerContext.message(),
                                     applicationContext,
                                     securityContext);
    }

    /*!
     * Looks up for the application identifier of a specific client process.
     * @param peerMessage, the request context of the peer.
     * @returns the application identifier of the process, or an empty string
     * if none found.
     */
    virtual QString appIdOfPeer(const QDBusMessage &peerMessage);
    /*! @overload */
    virtual QString appIdOfPeer(const QDBusContext &peerContext)
    { return appIdOfPeer(peerContext.message()); }

    /*!
     * @returns the application identifier of the keychain widget
     */
    virtual QString keychainWidgetAppId();

    /*!
     * Checks if a client process is allowed to set the specified acl on data
     * item.
     * An actual check depends on AC framework being used.
     * @param peerMessage, the request context of the peer.
     * @param applicationContext, request context within a process.
     * @param aclList, the acl list to be checked against.
     * @returns true, if the peer is allowed, false otherwise.
     */
    virtual bool isACLValid(const QDBusMessage &peerMessage,
                            const QString &applicationContext,
                            const SecurityContextList &aclList);
    /*! @overload */
    virtual bool isACLValid(const QDBusContext &peerContext,
                            const QString &applicationContext,
                            const SecurityContextList &aclList)
    {
        return isACLValid(peerContext.message(),
                          applicationContext,
                          aclList);
    }

};

} // namespace

#endif // SIGNON_ABSTRACT_ACCESS_CONTROL_MANAGER_H
