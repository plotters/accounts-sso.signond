/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2011-2012 Intel Corporation.
 *
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

#include "signonauthsessionadaptor.h"
#include "accesscontrolmanagerhelper.h"
#include "credentialsaccessmanager.h"
#include "credentialsdb.h"

namespace SignonDaemonNS {

SignonAuthSessionAdaptor::SignonAuthSessionAdaptor(SignonAuthSession *parent):
    QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

SignonAuthSessionAdaptor::~SignonAuthSessionAdaptor()
{
}

<<<<<<< HEAD
void SignonAuthSessionAdaptor::errorReply(const QString &name,
                                          const QString &message)
{
    QDBusMessage errReply =
        static_cast<QDBusContext *>(parent())->message().
        createErrorReply(name, message);
    SIGNOND_BUS.send(errReply);
}
=======
    QStringList SignonAuthSessionAdaptor::queryAvailableMechanisms(const QStringList &wantedMechanisms,
                                                                   const QDBusVariant &applicationContext)
    {
        TRACE();
>>>>>>> Add user data parameter to server side interfaces

QStringList
SignonAuthSessionAdaptor::queryAvailableMechanisms(
                                           const QStringList &wantedMechanisms)
{
    TRACE();

<<<<<<< HEAD
<<<<<<< HEAD
    QDBusContext &dbusContext = *static_cast<QDBusContext *>(parent());
    if (AccessControlManagerHelper::pidOfPeer(dbusContext) !=
        parent()->ownerPid()) {
        TRACE() << "queryAvailableMechanisms called from peer that doesn't "
            "own the AuthSession object\n";
        QString errMsg;
        QTextStream(&errMsg) << SIGNOND_PERMISSION_DENIED_ERR_STR
                             << " Authentication session owned by other "
                             "process.";
        errorReply(SIGNOND_PERMISSION_DENIED_ERR_NAME, errMsg);
        return QStringList();
=======
        return parent()->queryAvailableMechanisms(wantedMechanisms, userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return parent()->queryAvailableMechanisms(wantedMechanisms, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
    }

<<<<<<< HEAD
    return parent()->queryAvailableMechanisms(wantedMechanisms);
}
=======
    QVariantMap SignonAuthSessionAdaptor::process(const QVariantMap &sessionDataVa,
                                                  const QString &mechanism,
                                                  const QDBusVariant &applicationContext)
    {
        TRACE();
>>>>>>> Add user data parameter to server side interfaces

QVariantMap SignonAuthSessionAdaptor::process(const QVariantMap &sessionDataVa,
                                              const QString &mechanism)
{
    TRACE();

    QString allowedMechanism(mechanism);

    if (parent()->id() != SIGNOND_NEW_IDENTITY) {
        CredentialsDB *db =
            CredentialsAccessManager::instance()->credentialsDB();
        if (db) {
            SignonIdentityInfo identityInfo = db->credentials(parent()->id(),
                                                              false);
            if (!identityInfo.checkMethodAndMechanism(parent()->method(),
                                                      mechanism,
                                                      allowedMechanism)) {
                QString errMsg;
                QTextStream(&errMsg) << SIGNOND_METHOD_OR_MECHANISM_NOT_ALLOWED_ERR_STR
                                     << " Method:"
                                     << parent()->method()
                                     << ", mechanism:"
                                     << mechanism
                                     << ", allowed:"
                                     << allowedMechanism;
                errorReply(SIGNOND_METHOD_OR_MECHANISM_NOT_ALLOWED_ERR_NAME,
                           errMsg);
                return QVariantMap();
            }
        } else {
            BLAME() << "Null database handler object.";
        }
    }

<<<<<<< HEAD
    QDBusContext &dbusContext = *static_cast<QDBusContext *>(parent());
    if (AccessControlManagerHelper::pidOfPeer(dbusContext) !=
        parent()->ownerPid()) {
        TRACE() << "process called from peer that doesn't own the AuthSession "
            "object";
        QString errMsg;
        QTextStream(&errMsg) << SIGNOND_PERMISSION_DENIED_ERR_STR
                             << " Authentication session owned by other "
                             "process.";
        errorReply(SIGNOND_PERMISSION_DENIED_ERR_NAME, errMsg);
        return QVariantMap();
    }

<<<<<<< HEAD
<<<<<<< HEAD
    return parent()->process(sessionDataVa, allowedMechanism);
}
=======
    void SignonAuthSessionAdaptor::cancel(const QVariant &userdata)
=======
        return parent()->process(sessionDataVa, allowedMechanism, userdata);
    }

    void SignonAuthSessionAdaptor::cancel(const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
        return parent()->process(sessionDataVa, allowedMechanism, applicationContext);
    }

    void SignonAuthSessionAdaptor::cancel(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        TRACE();
>>>>>>> Add user data parameter to server side interfaces

void SignonAuthSessionAdaptor::cancel()
{
    TRACE();

<<<<<<< HEAD
    QDBusContext &dbusContext = *static_cast<QDBusContext *>(parent());
    if (AccessControlManagerHelper::pidOfPeer(dbusContext) != parent()->ownerPid()) {
        TRACE() << "cancel called from peer that doesn't own the AuthSession "
            "object";
        return;
    }

<<<<<<< HEAD
<<<<<<< HEAD
    parent()->cancel();
}
=======
    void SignonAuthSessionAdaptor::setId(quint32 id, const QVariant &userdata)
=======
        parent()->cancel(userdata);
    }

    void SignonAuthSessionAdaptor::setId(quint32 id,
                                         const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
        parent()->cancel(applicationContext);
    }

    void SignonAuthSessionAdaptor::setId(quint32 id,
                                         const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        TRACE();
>>>>>>> Add user data parameter to server side interfaces

void SignonAuthSessionAdaptor::setId(quint32 id)
{
    TRACE();

<<<<<<< HEAD
<<<<<<< HEAD
    QDBusContext &dbusContext = *static_cast<QDBusContext *>(parent());
    if (AccessControlManagerHelper::pidOfPeer(dbusContext) !=
        parent()->ownerPid()) {
        TRACE() << "setId called from peer that doesn't own the AuthSession "
            "object";
        return;
    }
    if (!AccessControlManagerHelper::instance()->isPeerAllowedToUseIdentity(
                                    dbusContext.message(), id)) {
        TRACE() << "setId called with an identifier the peer is not allowed "
            "to use";
        return;
    }

<<<<<<< HEAD
    parent()->setId(id);
}
=======
    void SignonAuthSessionAdaptor::objectUnref(const QVariant &userdata)
=======
        parent()->setId(id, userdata);
    }

    void SignonAuthSessionAdaptor::objectUnref(const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
        parent()->setId(id, applicationContext);
    }

    void SignonAuthSessionAdaptor::objectUnref(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        TRACE();
>>>>>>> Add user data parameter to server side interfaces

void SignonAuthSessionAdaptor::objectUnref()
{
    TRACE();

<<<<<<< HEAD
<<<<<<< HEAD
    QDBusContext &dbusContext = *static_cast<QDBusContext *>(parent());
    if (AccessControlManagerHelper::pidOfPeer(dbusContext) !=
        parent()->ownerPid()) {
        TRACE() << "objectUnref called from peer that doesn't own the "
            "AuthSession object";
        return;
=======
        parent()->objectUnref(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        parent()->objectUnref(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
    }

    parent()->objectUnref();
}

} //namespace SignonDaemonNS
