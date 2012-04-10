/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2011-2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
 * Contact: Alberto Mardegan <alberto.mardegan@nokia.com>
 * Contact: Jussi Laako <jussi.laako@linux.intel.com>
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

#include "signonidentityadaptor.h"

#include "signonidentity.h"
#include "accesscontrolmanagerhelper.h"

namespace SignonDaemonNS {

    SignonIdentityAdaptor::SignonIdentityAdaptor(SignonIdentity *parent)
        : QDBusAbstractAdaptor(parent),
          m_parent(parent)
    {
        setAutoRelaySignals(true);
    }

    SignonIdentityAdaptor::~SignonIdentityAdaptor()
    {}

    void SignonIdentityAdaptor::securityErrorReply(const char *failedMethodName)
    {
        QString errMsg;
        QTextStream(&errMsg) << SIGNOND_PERMISSION_DENIED_ERR_STR
                             << "Method:"
                             << failedMethodName;

        errorReply(SIGNOND_PERMISSION_DENIED_ERR_NAME, errMsg);
        TRACE() << "\nMethod FAILED Access Control check:\n" << failedMethodName;
    }

    void SignonIdentityAdaptor::errorReply(const QString &name,
                                           const QString &message)
    {
        QDBusMessage msg = parentDBusContext().message();
        msg.setDelayedReply(true);
        QDBusMessage errReply = msg.createErrorReply(name, message);
        SIGNOND_BUS.send(errReply);
    }

    quint32 SignonIdentityAdaptor::requestCredentialsUpdate(const QString &msg,
                                                            const QDBusVariant &userdata)
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return 0;
        }

        return m_parent->requestCredentialsUpdate(msg, userdata);
    }

    QVariantMap SignonIdentityAdaptor::getInfo(const QVariant &applicationContext)
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
            parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return QVariantMap();
        }

        return m_parent->getInfo();
    }

    void SignonIdentityAdaptor::addReference(const QString &reference,
                                             const QDBusVariant &userdata)
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return;
        }

        if (!m_parent->addReference(reference, userdata)) {
            /* TODO: add a lastError() method to SignonIdentity */
            errorReply(SIGNOND_OPERATION_FAILED_ERR_NAME,
                       SIGNOND_OPERATION_FAILED_ERR_STR);
        }
    }

    void SignonIdentityAdaptor::removeReference(const QString &reference,
                                                const QDBusVariant &userdata)
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return;
        }

        if (!m_parent->removeReference(reference, userdata)) {
            /* TODO: add a lastError() method to SignonIdentity */
            errorReply(SIGNOND_OPERATION_FAILED_ERR_NAME,
                       SIGNOND_OPERATION_FAILED_ERR_STR);
        }
    }


    bool SignonIdentityAdaptor::verifyUser(const QVariantMap &params,
                                           const QDBusVariant &userdata)
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return false;
        }

        return m_parent->verifyUser(params, userdata);
    }

    bool SignonIdentityAdaptor::verifySecret(const QString &secret,
                                             const QDBusVariant &userdata)
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return false;
        }

        return m_parent->verifySecret(secret, userdata);
    }

    void SignonIdentityAdaptor::remove(const QDBusVariant &userdata)
    {
        /* Access Control */
        AccessControlManagerHelper::IdentityOwnership ownership =
                AccessControlManagerHelper::instance()->isPeerOwnerOfIdentity(
                            parentDBusContext().message(), m_parent->id());

        if (ownership != AccessControlManagerHelper::IdentityDoesNotHaveOwner) {
            //Identity has an owner
            if (ownership == AccessControlManagerHelper::ApplicationIsNotOwner
                && !AccessControlManagerHelper::instance()->isPeerKeychainWidget(parentDBusContext().message())) {

                securityErrorReply(__func__);
                return;
            }
        }

        m_parent->remove(userdata);
    }

    bool SignonIdentityAdaptor::signOut(const QDBusVariant &userdata)
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return false;
        }

        return m_parent->signOut(userdata);
    }

    quint32 SignonIdentityAdaptor::store(const QVariantMap &info,
                                         const QDBusVariant &userdata)
    {
        quint32 id = info.value(QLatin1String("Id"), SIGNOND_NEW_IDENTITY).toInt();
        /* Access Control */
        if (id != SIGNOND_NEW_IDENTITY) {
        AccessControlManagerHelper::IdentityOwnership ownership =
                AccessControlManagerHelper::instance()->isPeerOwnerOfIdentity(
                            parentDBusContext().message(), m_parent->id());

            if (ownership != AccessControlManagerHelper::IdentityDoesNotHaveOwner) {
                //Identity has an owner
                if (ownership == AccessControlManagerHelper::ApplicationIsNotOwner
                    && !AccessControlManagerHelper::instance()->isPeerKeychainWidget(parentDBusContext().message())) {

                    securityErrorReply(__func__);
                    return 0;
                }
            }
        }
        return m_parent->store(info, userdata);
    }

} //namespace SignonDaemonNS
