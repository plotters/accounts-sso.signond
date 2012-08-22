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

#include "smack-access-control-manager.h"
#include "debug.h"

#include <DBusSmackContext>
#include <SmackQt>

static const char keychainAppId[] = "SignondKeychain";

SmackAccessControlManager::SmackAccessControlManager(QObject *parent):
    SignOn::AbstractAccessControlManager(parent)
{
}

SmackAccessControlManager::~SmackAccessControlManager()
{
}

QString SmackAccessControlManager::keychainWidgetAppId()
{
    return QLatin1String(keychainAppId);
}

bool SmackAccessControlManager::isPeerAllowedToUseIdentity(
                                const QDBusMessage &peerMessage,
                                const QDBusVariant &applicationContext,
                                const SignOn::SecurityContext &securityContext)
{
    QString appId =
        SmackQt::DBusSmackContext::getCallerSmackContext(peerMessage);
    QString appCtx = applicationContext.variant().toString();
    TRACE() << appId << ":["
            << securityContext.sysCtx << ","
            << securityContext.appCtx << "]";

    if (SmackQt::Smack::hasAccess(appId,
                                  securityContext.sysCtx,
                                  QLatin1String("x"))) {
        if (securityContext.appCtx.isEmpty()) {
            TRACE() << "Process ACCESS:TRUE";
            return true;
        } else {
            if (appCtx == securityContext.appCtx ||
                securityContext.appCtx == QLatin1String("*")) {
                TRACE() << "Process & Application Context ACCESS:TRUE";
                return true;
            }
            TRACE() << "Application Context ACCESS:FALSE";
            return false;
        }
    } else {
        TRACE() << "Process ACCESS:FALSE";
        return false;
    }
}

bool SmackAccessControlManager::isPeerOwnerOfIdentity(
                                const QDBusMessage &peerMessage,
                                const QDBusVariant &applicationContext,
                                const SignOn::SecurityContext &securityContext)
{
    QString appId =
        SmackQt::DBusSmackContext::getCallerSmackContext(peerMessage);
    QString appCtx = applicationContext.variant().toString();
    TRACE() << appId << ":["
            << securityContext.sysCtx << ","
            << securityContext.appCtx << "]";

    if ((SmackQt::Smack::hasAccess(
                            appId, securityContext, QLatin1String("r"))) &&
        (SmackQt::Smack::hasAccess(
                            appId, securityContext, QLatin1String("w")))) {
        if (securityContext.appCtx.isEmpty()) {
            TRACE() << "Process ACCESS:TRUE";
            return true;
        } else {
            if (appCtx == securityContext.appCtx ||
                securityContext.appCtx == QLatin1String("*")) {
                TRACE() << "Process & Application Context ACCESS:TRUE";
                return true;
            }
            TRACE() << "Application Context ACCESS:FALSE";
            return false;
        }
    } else {
        TRACE() << "Process ACCESS:FALSE";
        return false;
    }
}
  
QString SmackAccessControlManager::appIdOfPeer(const QDBusMessage &peerMessage)
{
    TRACE() << SmackQt::DBusSmackContext::getCallerSmackContext(peerMessage);
    return SmackQt::DBusSmackContext::getCallerSmackContext(peerMessage);
}

bool SmackAccessControlManager::isACLValid(
                                    const QDBusMessage &peerMessage,
                                    const QDBusVariant &applicationContext,
                                    const SignOn::SecurityContextList &aclList)
{
    Q_UNUSED(applicationContext);

    QString appId =
        SmackQt::DBusSmackContext::getCallerSmackContext(peerMessage);
    QString appIdPrefixed;
    QString sep = QLatin1String("::");
    appIdPrefixed.append(appId);
    appIdPrefixed.append(sep);
    TRACE() << appId << appIdPrefixed;

    if (!aclList.isEmpty()){
        foreach (SignOn::SecurityContext aclItem, aclList) {
            TRACE() << aclItem;
            // if app sets an acl entry for its appid, then it is always
            // allowed
            if (appId == aclItem.sysCtx)
                continue;
            // if app sets an acl entry for the label of its subdomain,
            // then it is allowed, too
            if (aclItem.sysCtx.indexOf(appId) == 0)
                continue;
            // if none of above then this acl must be denied
            TRACE() << "An attempt to setup an acl" << aclItem
                    << "for process domain" << appId
                    << "is denied";
            return false;
        }
    }
    return true;
}

