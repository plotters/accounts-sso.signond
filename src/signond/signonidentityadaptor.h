/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
<<<<<<< HEAD
 * Copyright (C) 2012 Canonical Ltd.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
=======
 * Copyright (C) 2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
 * Contact: Alberto Mardegan <alberto.mardegan@nokia.com>
 * Contact: Jussi Laako <jussi.laako@linux.intel.com>
>>>>>>> Add user data parameter to server side interfaces
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

#ifndef SIGNONIDENTITYADAPTOR_H
#define SIGNONIDENTITYADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QDBusContext>

#include "signond-common.h"
#include "signonidentity.h"

namespace SignonDaemonNS {

class SignonIdentityAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface",
                "com.google.code.AccountsSSO.SingleSignOn.Identity")

public:
    SignonIdentityAdaptor(SignonIdentity *parent);
    virtual ~SignonIdentityAdaptor();

    inline const QDBusContext &parentDBusContext() const
        { return *static_cast<QDBusContext *>(m_parent); }

<<<<<<< HEAD
public Q_SLOTS:
    quint32 requestCredentialsUpdate(const QString &message);
    QVariantMap getInfo();
    void addReference(const QString &reference);
    void removeReference(const QString &reference);

    bool verifyUser(const QVariantMap &params);
    bool verifySecret(const QString &secret);
    void remove();
    bool signOut();
    quint32 store(const QVariantMap &info);

Q_SIGNALS:
    void unregistered();
    void infoUpdated(int);
=======
    public Q_SLOTS:
        quint32 requestCredentialsUpdate(const QString &message,
                                         const QVariant &userdata);
        QList<QVariant> queryInfo(const QVariant &userdata);
        void addReference(const QString &reference,
                          const QVariant &userdata);
        void removeReference(const QString &reference,
                             const QVariant &userdata);

        bool verifyUser(const QVariantMap &params,
                        const QVariant &userdata);
        bool verifySecret(const QString &secret,
                          const QVariant &userdata);
        void remove(const QVariant &userdata);
        bool signOut(const QVariant &userdata);
        quint32 store(const QVariantMap &info,
                      const QVariant &userdata);

        quint32 storeCredentials(const quint32 id,
                                 const QString &userName,
                                 const QString &secret,
                                 const bool storeSecret,
                                 const QMap<QString, QVariant> &methods,
                                 const QString &caption,
                                 const QStringList &realms,
                                 const QStringList &accessControlList,
                                 const int type,
                                 const QVariant &userdata);
    Q_SIGNALS:
        void unregistered();
        void infoUpdated(int);
>>>>>>> Add user data parameter to server side interfaces

private:
    void securityErrorReply(const char *failedMethodName);
    void errorReply(const QString &name, const QString &message);

private:
    SignonIdentity *m_parent;
};

} //namespace SignonDaemonNS

#endif // SIGNONIDENTITYADAPTOR_H
