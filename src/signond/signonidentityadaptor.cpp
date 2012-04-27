/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2011-2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
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

SignonIdentityAdaptor::SignonIdentityAdaptor(SignonIdentity *parent):
    QDBusAbstractAdaptor(parent),
    m_parent(parent)
{
    setAutoRelaySignals(true);
}

SignonIdentityAdaptor::~SignonIdentityAdaptor()
{
}

void SignonIdentityAdaptor::securityErrorReply(const char *failedMethodName)
{
    QString errMsg;
    QTextStream(&errMsg) << SIGNOND_PERMISSION_DENIED_ERR_STR
                         << "Method:"
                         << failedMethodName;

    errorReply(SIGNOND_PERMISSION_DENIED_ERR_NAME, errMsg);
    TRACE() << "Method FAILED Access Control check:" << failedMethodName;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Merge & cleanup from master
void SignonIdentityAdaptor::errorReply(const QString &name,
                                       const QString &message)
{
    QDBusMessage msg = parentDBusContext().message();
    msg.setDelayedReply(true);
    QDBusMessage errReply = msg.createErrorReply(name, message);
    SIGNOND_BUS.send(errReply);
}
<<<<<<< HEAD
=======
    void SignonIdentityAdaptor::errorReply(const QString &name,
                                           const QString &message)
    {
        QDBusMessage msg = parentDBusContext().message();
        msg.setDelayedReply(true);
        QDBusMessage errReply = msg.createErrorReply(name, message);
        SIGNOND_BUS.send(errReply);
    }

    quint32 SignonIdentityAdaptor::requestCredentialsUpdate(const QString &msg,
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
                                                            const QDBusVariant &applicationContext)
=======
                                                            const QVariant &userdata)
>>>>>>> Add user data parameter to server side interfaces
=======
                                                            const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                                            const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return 0;
        }
>>>>>>> Add user data parameter to server side interfaces

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
quint32 SignonIdentityAdaptor::requestCredentialsUpdate(const QString &msg)
=======

quint32 SignonIdentityAdaptor::requestCredentialsUpdate(
                                        const QString &msg,
                                        const QDBusVariant &applicationContext)
>>>>>>> Merge & cleanup from master
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                    parentDBusContext().message(),
                                    m_parent->id())) {
        securityErrorReply(__func__);
        return 0;
<<<<<<< HEAD
    }

<<<<<<< HEAD
<<<<<<< HEAD
    return m_parent->requestCredentialsUpdate(msg);
}
=======
    QList<QVariant> SignonIdentityAdaptor::queryInfo(const QVariant &userdata)
=======
=======
>>>>>>> Use QDBusVariant instead of QVariant
        return m_parent->requestCredentialsUpdate(msg, userdata);
=======
        return m_parent->requestCredentialsUpdate(msg, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
    }

<<<<<<< HEAD
    QList<QVariant> SignonIdentityAdaptor::queryInfo(const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->requestCredentialsUpdate(msg, applicationContext);
    }

<<<<<<< HEAD
    QList<QVariant> SignonIdentityAdaptor::queryInfo(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
    QVariantMap SignonIdentityAdaptor::getInfo(const QVariant &applicationContext)
>>>>>>> Add user data parameter to server side interfaces
=======
    QVariantMap SignonIdentityAdaptor::getInfo(const QDBusVariant &applicationContext)
>>>>>>> Rebase
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return QList<QVariant>();
        }
>>>>>>> Add user data parameter to server side interfaces

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
QVariantMap SignonIdentityAdaptor::getInfo()
=======
    }

    return m_parent->requestCredentialsUpdate(msg, applicationContext);
}

QVariantMap SignonIdentityAdaptor::getInfo(
                                        const QDBusVariant &applicationContext)
>>>>>>> Merge & cleanup from master
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
        parentDBusContext().message(), m_parent->id())) {
        securityErrorReply(__func__);
        return QVariantMap();
<<<<<<< HEAD
=======
        return m_parent->queryInfo(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->queryInfo(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        return m_parent->getInfo(applicationContext);
>>>>>>> Rebase
    }

<<<<<<< HEAD
<<<<<<< HEAD
    return m_parent->getInfo();
}
=======
    void SignonIdentityAdaptor::addReference(const QString &reference,
                                             const QDBusVariant &applicationContext)
=======
    void SignonIdentityAdaptor::addReference(const QString &reference,
<<<<<<< HEAD
<<<<<<< HEAD
                                             const QVariant &userdata)
>>>>>>> Add user data parameter to server side interfaces
=======
                                             const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                             const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return;
        }
>>>>>>> Add user data parameter to server side interfaces

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
void SignonIdentityAdaptor::addReference(const QString &reference)
=======
    }

    return m_parent->getInfo(applicationContext);
}

void SignonIdentityAdaptor::addReference(const QString &reference,
                                         const QDBusVariant &applicationContext)
>>>>>>> Merge & cleanup from master
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                    parentDBusContext().message(),
                                    m_parent->id())) {
        securityErrorReply(__func__);
        return;
<<<<<<< HEAD
=======
        if (!m_parent->addReference(reference, userdata)) {
=======
        if (!m_parent->addReference(reference, applicationContext)) {
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        if (!m_parent->addReference(reference, userdata)) {
>>>>>>> Use QDBusVariant instead of QVariant
=======
        if (!m_parent->addReference(reference, applicationContext)) {
>>>>>>> Rename 'userdata' to 'applicationContext'
            /* TODO: add a lastError() method to SignonIdentity */
            errorReply(SIGNOND_OPERATION_FAILED_ERR_NAME,
                       SIGNOND_OPERATION_FAILED_ERR_STR);
        }
>>>>>>> Use QDBusVariant instead of QVariant
    }

<<<<<<< HEAD
<<<<<<< HEAD
    if (!m_parent->addReference(reference)) {
        /* TODO: add a lastError() method to SignonIdentity */
        errorReply(SIGNOND_OPERATION_FAILED_ERR_NAME,
                   SIGNOND_OPERATION_FAILED_ERR_STR);
    }
}
=======
    void SignonIdentityAdaptor::removeReference(const QString &reference,
                                                const QDBusVariant &applicationContext)
<<<<<<< HEAD
=======
    void SignonIdentityAdaptor::removeReference(const QString &reference,
<<<<<<< HEAD
                                                const QVariant &userdata)
>>>>>>> Add user data parameter to server side interfaces
=======
                                                const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return;
        }
>>>>>>> Add user data parameter to server side interfaces

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
void SignonIdentityAdaptor::removeReference(const QString &reference)
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                    parentDBusContext().message(),
                                    m_parent->id())) {
        securityErrorReply(__func__);
        return;
=======
        if (!m_parent->removeReference(reference, userdata)) {
=======
        if (!m_parent->removeReference(reference, applicationContext)) {
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        if (!m_parent->removeReference(reference, userdata)) {
>>>>>>> Use QDBusVariant instead of QVariant
=======
        if (!m_parent->removeReference(reference, applicationContext)) {
>>>>>>> Rename 'userdata' to 'applicationContext'
            /* TODO: add a lastError() method to SignonIdentity */
            errorReply(SIGNOND_OPERATION_FAILED_ERR_NAME,
                       SIGNOND_OPERATION_FAILED_ERR_STR);
        }
>>>>>>> Use QDBusVariant instead of QVariant
=======
    }

    if (!m_parent->addReference(reference, applicationContext)) {
        /* TODO: add a lastError() method to SignonIdentity */
        errorReply(SIGNOND_OPERATION_FAILED_ERR_NAME,
                   SIGNOND_OPERATION_FAILED_ERR_STR);
>>>>>>> Merge & cleanup from master
    }
}

<<<<<<< HEAD
    if (!m_parent->removeReference(reference)) {
        /* TODO: add a lastError() method to SignonIdentity */
        errorReply(SIGNOND_OPERATION_FAILED_ERR_NAME,
                   SIGNOND_OPERATION_FAILED_ERR_STR);
    }
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
    bool SignonIdentityAdaptor::verifyUser(const QVariantMap &params,
                                           const QDBusVariant &applicationContext)
<<<<<<< HEAD
=======
    bool SignonIdentityAdaptor::verifyUser(const QVariantMap &params,
<<<<<<< HEAD
                                           const QVariant &userdata)
>>>>>>> Add user data parameter to server side interfaces
=======
                                           const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return false;
        }
>>>>>>> Add user data parameter to server side interfaces

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
bool SignonIdentityAdaptor::verifyUser(const QVariantMap &params)
=======
void SignonIdentityAdaptor::removeReference(
                                        const QString &reference,
                                        const QDBusVariant &applicationContext)
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                    parentDBusContext().message(),
                                    m_parent->id())) {
        securityErrorReply(__func__);
        return;
    }

    if (!m_parent->removeReference(reference, applicationContext)) {
        /* TODO: add a lastError() method to SignonIdentity */
        errorReply(SIGNOND_OPERATION_FAILED_ERR_NAME,
                   SIGNOND_OPERATION_FAILED_ERR_STR);
    }
}

bool SignonIdentityAdaptor::verifyUser(const QVariantMap &params,
                                       const QDBusVariant &applicationContext)
>>>>>>> Merge & cleanup from master
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                    parentDBusContext().message(),
                                    m_parent->id())) {
        securityErrorReply(__func__);
        return false;
<<<<<<< HEAD
=======
        return m_parent->verifyUser(params, userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->verifyUser(params, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        return m_parent->verifyUser(params, userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->verifyUser(params, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
    }

<<<<<<< HEAD
<<<<<<< HEAD
    return m_parent->verifyUser(params);
}
=======
    bool SignonIdentityAdaptor::verifySecret(const QString &secret,
                                             const QDBusVariant &applicationContext)
<<<<<<< HEAD
=======
    bool SignonIdentityAdaptor::verifySecret(const QString &secret,
<<<<<<< HEAD
                                             const QVariant &userdata)
>>>>>>> Add user data parameter to server side interfaces
=======
                                             const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
            securityErrorReply(__func__);
            return false;
        }
>>>>>>> Add user data parameter to server side interfaces

<<<<<<< HEAD
bool SignonIdentityAdaptor::verifySecret(const QString &secret)
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                    parentDBusContext().message(),
                                    m_parent->id())) {
        securityErrorReply(__func__);
        return false;
    }

<<<<<<< HEAD
<<<<<<< HEAD
    return m_parent->verifySecret(secret);
}
=======
    void SignonIdentityAdaptor::remove(const QVariant &userdata)
=======
        return m_parent->verifySecret(secret, userdata);
    }

<<<<<<< HEAD
    void SignonIdentityAdaptor::remove(const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->verifySecret(secret, applicationContext);
    }

<<<<<<< HEAD
<<<<<<< HEAD
    void SignonIdentityAdaptor::remove(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
    void SignonIdentityAdaptor::remove(const QVariant &userdata)
>>>>>>> Add user data parameter to server side interfaces
=======
        return m_parent->verifySecret(secret, userdata);
    }

    void SignonIdentityAdaptor::remove(const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->verifySecret(secret, applicationContext);
    }

    void SignonIdentityAdaptor::remove(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        /* Access Control */
        AccessControlManagerHelper::IdentityOwnership ownership =
                AccessControlManagerHelper::instance()->isPeerOwnerOfIdentity(
                            parentDBusContext().message(), m_parent->id());

        if (ownership != AccessControlManagerHelper::IdentityDoesNotHaveOwner) {
            //Identity has an owner
            if (ownership == AccessControlManagerHelper::ApplicationIsNotOwner
                && !AccessControlManagerHelper::instance()->isPeerKeychainWidget(parentDBusContext().message())) {
>>>>>>> Add user data parameter to server side interfaces

void SignonIdentityAdaptor::remove()
=======
    }

    return m_parent->verifyUser(params, applicationContext);
}

bool SignonIdentityAdaptor::verifySecret(const QString &secret,
                                         const QDBusVariant &applicationContext)
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                    parentDBusContext().message(),
                                    m_parent->id())) {
        securityErrorReply(__func__);
        return false;
    }

    return m_parent->verifySecret(secret, applicationContext);
}

void SignonIdentityAdaptor::remove(const QDBusVariant &applicationContext)
>>>>>>> Merge & cleanup from master
{
    /* Access Control */
    AccessControlManagerHelper::IdentityOwnership ownership =
            AccessControlManagerHelper::instance()->isPeerOwnerOfIdentity(
                        parentDBusContext().message(), m_parent->id());
<<<<<<< HEAD

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    if (ownership != AccessControlManagerHelper::IdentityDoesNotHaveOwner) {
        //Identity has an owner
        if (ownership == AccessControlManagerHelper::ApplicationIsNotOwner &&
            !AccessControlManagerHelper::instance()->isPeerKeychainWidget(
                                             parentDBusContext().message())) {

<<<<<<< HEAD
<<<<<<< HEAD
=======
    bool SignonIdentityAdaptor::signOut(const QVariant &userdata)
=======
        m_parent->remove(userdata);
    }

    bool SignonIdentityAdaptor::signOut(const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
        m_parent->remove(applicationContext);
    }

<<<<<<< HEAD
    bool SignonIdentityAdaptor::signOut(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
    bool SignonIdentityAdaptor::signOut(const QVariant &userdata)
>>>>>>> Add user data parameter to server side interfaces
=======
        m_parent->remove(userdata);
    }

    bool SignonIdentityAdaptor::signOut(const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
        m_parent->remove(applicationContext);
    }

    bool SignonIdentityAdaptor::signOut(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        /* Access Control */
        if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                        parentDBusContext().message(), m_parent->id())) {
>>>>>>> Add user data parameter to server side interfaces
            securityErrorReply(__func__);
            return;
        }
<<<<<<< HEAD
=======

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        return m_parent->signOut(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->signOut(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        return m_parent->signOut(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->signOut(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
    }

<<<<<<< HEAD
<<<<<<< HEAD
    m_parent->remove();
}
=======
    quint32 SignonIdentityAdaptor::store(const QVariantMap &info,
                                         const QDBusVariant &applicationContext)
<<<<<<< HEAD
=======
    quint32 SignonIdentityAdaptor::store(const QVariantMap &info,
<<<<<<< HEAD
                                         const QVariant &userdata)
>>>>>>> Add user data parameter to server side interfaces
=======
                                         const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
>>>>>>> Rename 'userdata' to 'applicationContext'
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
>>>>>>> Add user data parameter to server side interfaces

<<<<<<< HEAD
bool SignonIdentityAdaptor::signOut()
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                             parentDBusContext().message(), m_parent->id())) {
        securityErrorReply(__func__);
        return false;
=======
                    securityErrorReply(__func__);
                    return 0;
                }
            }
        }
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        return m_parent->store(info, userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->store(info, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
    }

<<<<<<< HEAD
    return m_parent->signOut();
}
=======
    quint32 SignonIdentityAdaptor::storeCredentials(const quint32 id,
                                                    const QString &userName,
                                                    const QString &secret,
                                                    const bool storeSecret,
                                                    const QMap<QString, QVariant> &methods,
                                                    const QString &caption,
                                                    const QStringList &realms,
                                                    const QStringList &accessControlList,
                                                    const int type,
                                                    const QDBusVariant &applicationContext)
    {
        /* Access Control */
        if (id != SIGNOND_NEW_IDENTITY) {
        AccessControlManagerHelper::IdentityOwnership ownership =
                AccessControlManagerHelper::instance()->isPeerOwnerOfIdentity(
                            parentDBusContext().message(), m_parent->id());
>>>>>>> Add user data parameter to server side interfaces

quint32 SignonIdentityAdaptor::store(const QVariantMap &info)
{
    quint32 id = info.value(QLatin1String("Id"), SIGNOND_NEW_IDENTITY).toInt();
    /* Access Control */
    if (id != SIGNOND_NEW_IDENTITY) {
    AccessControlManagerHelper::IdentityOwnership ownership =
            AccessControlManagerHelper::instance()->isPeerOwnerOfIdentity(
                        parentDBusContext().message(), m_parent->id());

        if (ownership != AccessControlManagerHelper::IdentityDoesNotHaveOwner) {
            //Identity has an owner
            if (ownership == AccessControlManagerHelper::ApplicationIsNotOwner &&
                !AccessControlManagerHelper::instance()->isPeerKeychainWidget(
                                              parentDBusContext().message())) {

                securityErrorReply(__func__);
                return 0;
            }
        }
<<<<<<< HEAD
=======

        return m_parent->storeCredentials(id,
                                          userName,
                                          secret,
                                          storeSecret,
                                          methods,
                                          caption,
                                          realms,
                                          accessControlList,
                                          type,
<<<<<<< HEAD
                                          userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                          applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        return m_parent->store(info, userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return m_parent->store(info, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
    }
    return m_parent->store(info);
=======

    if (ownership != AccessControlManagerHelper::IdentityDoesNotHaveOwner) {
        //Identity has an owner
        if (ownership == AccessControlManagerHelper::ApplicationIsNotOwner &&
            !AccessControlManagerHelper::instance()->isPeerKeychainWidget(
                                             parentDBusContext().message())) {

            securityErrorReply(__func__);
            return;
        }
    }

    m_parent->remove(applicationContext);
}

bool SignonIdentityAdaptor::signOut(const QDBusVariant &applicationContext)
{
    /* Access Control */
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                             parentDBusContext().message(), m_parent->id())) {
        securityErrorReply(__func__);
        return false;
    }

    return m_parent->signOut(applicationContext);
}

quint32 SignonIdentityAdaptor::store(const QVariantMap &info,
                                     const QDBusVariant &applicationContext)
{
    quint32 id = info.value(QLatin1String("Id"), SIGNOND_NEW_IDENTITY).toInt();
    /* Access Control */
    if (id != SIGNOND_NEW_IDENTITY) {
    AccessControlManagerHelper::IdentityOwnership ownership =
            AccessControlManagerHelper::instance()->isPeerOwnerOfIdentity(
                        parentDBusContext().message(), m_parent->id());

        if (ownership != AccessControlManagerHelper::IdentityDoesNotHaveOwner) {
            //Identity has an owner
            if (ownership == AccessControlManagerHelper::ApplicationIsNotOwner &&
                !AccessControlManagerHelper::instance()->isPeerKeychainWidget(
                                              parentDBusContext().message())) {

                securityErrorReply(__func__);
                return 0;
            }
        }
    }
    return m_parent->store(info, applicationContext);
>>>>>>> Merge & cleanup from master
}

} //namespace SignonDaemonNS
