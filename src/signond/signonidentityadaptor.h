/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
<<<<<<< HEAD
 * Copyright (C) 2012 Canonical Ltd.
 * Copyright (C) 2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
<<<<<<< HEAD
=======
 * Copyright (C) 2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
 * Contact: Alberto Mardegan <alberto.mardegan@nokia.com>
 * Contact: Jussi Laako <jussi.laako@linux.intel.com>
>>>>>>> Add user data parameter to server side interfaces
=======
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
<<<<<<< HEAD

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
<<<<<<< HEAD
<<<<<<< HEAD
                                         const QDBusVariant &applicationContext);
        QList<QVariant> queryInfo(const QDBusVariant &applicationContext);
        void addReference(const QString &reference,
                          const QDBusVariant &applicationContext);
        void removeReference(const QString &reference,
                             const QDBusVariant &applicationContext);

        bool verifyUser(const QVariantMap &params,
                        const QDBusVariant &applicationContext);
        bool verifySecret(const QString &secret,
                          const QDBusVariant &applicationContext);
        void remove(const QDBusVariant &applicationContext);
        bool signOut(const QDBusVariant &applicationContext);
        quint32 store(const QVariantMap &info,
                      const QDBusVariant &applicationContext);
=======
                                         const QVariant &applicationContext);
        QVariantMap getInfo(const QVariant &applicationContext);
=======
                                         const QDBusVariant &applicationContext);
        QVariantMap getInfo(const QDBusVariant &applicationContext);
>>>>>>> Rebase
        void addReference(const QString &reference,
                          const QDBusVariant &applicationContext);
        void removeReference(const QString &reference,
                             const QDBusVariant &applicationContext);

        bool verifyUser(const QVariantMap &params,
                        const QDBusVariant &applicationContext);
        bool verifySecret(const QString &secret,
                          const QDBusVariant &applicationContext);
        void remove(const QDBusVariant &applicationContext);
        bool signOut(const QDBusVariant &applicationContext);
        quint32 store(const QVariantMap &info,
<<<<<<< HEAD
<<<<<<< HEAD
                      const QVariant &userdata);
>>>>>>> Add user data parameter to server side interfaces
=======
                      const QDBusVariant &userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
                      const QDBusVariant &applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

        quint32 storeCredentials(const quint32 id,
                                 const QString &userName,
                                 const QString &secret,
                                 const bool storeSecret,
                                 const QMap<QString, QVariant> &methods,
                                 const QString &caption,
                                 const QStringList &realms,
                                 const QStringList &accessControlList,
                                 const int type,
                                 const QDBusVariant &applicationContext);
    Q_SIGNALS:
        void unregistered();
        void infoUpdated(int);
>>>>>>> Add user data parameter to server side interfaces
=======

public:
    SignonIdentityAdaptor(SignonIdentity *parent);
    virtual ~SignonIdentityAdaptor();

    inline const QDBusContext &parentDBusContext() const
        { return *static_cast<QDBusContext *>(m_parent); }

public Q_SLOTS:
    quint32 requestCredentialsUpdate(const QString &message,
                                     const QDBusVariant &applicationContext);
    QVariantMap getInfo(const QDBusVariant &applicationContext);
    void addReference(const QString &reference,
                      const QDBusVariant &applicationContext);
    void removeReference(const QString &reference,
                         const QDBusVariant &applicationContext);

    bool verifyUser(const QVariantMap &params,
                    const QDBusVariant &applicationContext);
    bool verifySecret(const QString &secret,
                      const QDBusVariant &applicationContext);
    void remove(const QDBusVariant &applicationContext);
    bool signOut(const QDBusVariant &applicationContext);
    quint32 store(const QVariantMap &info,
                  const QDBusVariant &applicationContext);

Q_SIGNALS:
    void unregistered();
    void infoUpdated(int);
>>>>>>> Merge & cleanup from master

private:
    void securityErrorReply(const char *failedMethodName);
    void errorReply(const QString &name, const QString &message);

private:
    SignonIdentity *m_parent;
};

} //namespace SignonDaemonNS

#endif // SIGNONIDENTITYADAPTOR_H
