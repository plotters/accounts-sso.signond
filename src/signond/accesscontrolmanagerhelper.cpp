/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2011-2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
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

#include <QBuffer>
#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include "accesscontrolmanagerhelper.h"
#include "signond-common.h"
#include "credentialsaccessmanager.h"
#include "signonidentity.h"

using namespace SignonDaemonNS;

AccessControlManagerHelper *AccessControlManagerHelper::m_pInstance = NULL;

AccessControlManagerHelper *AccessControlManagerHelper::instance()
{
    return m_pInstance;
}

AccessControlManagerHelper::AccessControlManagerHelper(
                                SignOn::AbstractAccessControlManager *acManager)
{
    if (!m_pInstance) {
        m_pInstance = this;
        m_acManager = acManager;
    } else {
        BLAME() << "Creating a second instance of the CAM";
    }
}

AccessControlManagerHelper::~AccessControlManagerHelper()
{
    m_acManager = NULL;
    m_pInstance = NULL;
}


bool
AccessControlManagerHelper::isPeerAllowedToUseIdentity(
                                        const QDBusMessage &peerMessage,
                                        const QString &applicationContext,
                                        const quint32 identityId)
{
    // TODO - improve this, the error handling and more precise behaviour

    CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
    if (db == 0) {
        TRACE() << "NULL db pointer, secure storage might be unavailable,";
        return false;
    }
    SecurityContextList acl = db->accessControlList(identityId);

    QStringList aclStrings;
    foreach (SecurityContext securityContext, acl) {
        aclStrings.append(QLatin1String("[") +
                          securityContext.sysCtx +
                          QLatin1String(",") +
                          securityContext.appCtx +
                          QLatin1String("]"));
        TRACE() << QString(QLatin1String("Access control list of identity: "
                                     "%1: {%2}.item count: %3\t"))
                                    .arg(identityId)
                                    .arg(aclStrings.join(QLatin1String(", ")))
                                    .arg(aclStrings.size());
    }

    if (db->errorOccurred()) {
        TRACE() << "Database error!";
        return false;
    }

    if (acl.isEmpty()) {
        TRACE() << "ACL is empty!";
        return false;
    }

    if (acl.contains(SecurityContext(QString::fromLatin1("*"))))
        return true;

    return peerHasOneOfAccesses(peerMessage, applicationContext, acl);
}

AccessControlManagerHelper::IdentityOwnership
AccessControlManagerHelper::isPeerOwnerOfIdentity(
                                        const QDBusMessage &peerMessage,
                                        const QString &applicationContext,
                                        const quint32 identityId)
{
    CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
    if (db == 0) {
        TRACE() << "NULL db pointer, secure storage might be unavailable,";
        return ApplicationIsNotOwner;
    }
    QStringList ownerSecContexts = db->ownerList(identityId);

    if (db->errorOccurred())
        return ApplicationIsNotOwner;

    if (ownerSecContexts.isEmpty())
        return IdentityDoesNotHaveOwner;

    if (ownerSecContexts.contains(QLatin1String("*")))
        return ApplicationIsOwner;

    foreach(QString securityContext, ownerSecContexts) {
        TRACE() << securityContext;
        if (m_acManager->isPeerOwnerOfIdentity(peerMessage,
                                               applicationContext,
                                               securityContext))
            return ApplicationIsOwner;
    }
    return ApplicationIsNotOwner;
}

bool
AccessControlManagerHelper::isPeerKeychainWidget(
                                                const QDBusMessage &peerMessage)
{
    static SecurityContext keychainWidgetAppId = SecurityContext(
                                            m_acManager->keychainWidgetAppId(),
                                            QString());
    QString peerAppId = m_acManager->appIdOfPeer(peerMessage);
    return (peerAppId == keychainWidgetAppId);
}

SecurityContext AccessControlManagerHelper::appIdOfPeer(
                                        const QDBusMessage &peerMessage,
                                        const QString &applicationContext)
{
    TRACE() << m_acManager->appIdOfPeer(peerMessage);
    return SecurityContext(m_acManager->appIdOfPeer(peerMessage),
                           applicationContext);
}

bool
AccessControlManagerHelper::peerHasOneOfAccesses(
                                        const QDBusMessage &peerMessage,
                                        const QString &applicationContext,
                                        const SecurityContextList &secContexts)
{
    foreach (SecurityContext securityContext, secContexts) {
        TRACE() << securityContext;
        if (securityContext.sysCtx == QString::fromLatin1("*"))
            return true;
        if (m_acManager->isPeerAllowedToUseIdentity(peerMessage,
                                                    applicationContext,
                                                    securityContext))
            return true;
    }

    BLAME() << "given peer does not have needed permissions";
    return false;
}

bool
AccessControlManagerHelper::isPeerAllowedToAccess(
                                        const QDBusMessage &peerMessage,
                                        const QString &applicationContext,
                                        const SecurityContext &securityContext)
{
    TRACE() << securityContext;
    return m_acManager->isPeerAllowedToUseIdentity(peerMessage,
                                                   applicationContext,
                                                   securityContext);
}

pid_t AccessControlManagerHelper::pidOfPeer(const QDBusContext &peerContext)
{
    QString service = peerContext.message().service();
    return peerContext.connection().interface()->servicePid(service).value();
}

bool AccessControlManagerHelper::isACLValid(
                                        const QDBusMessage &peerMessage,
                                        const QString &applicationContext,
                                        const SecurityContextList &aclList)
{
    return m_acManager->isACLValid(peerMessage, applicationContext, aclList);
}
