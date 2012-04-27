/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
<<<<<<< HEAD
<<<<<<< HEAD
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
=======
 * Contact: Alberto Mardegan <alberto.mardegan@nokia.com>
=======
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
>>>>>>> Merge & cleanup from master
 * Contact: Jussi Laako <jussi.laako@linux.intel.com>
<<<<<<< HEAD
>>>>>>> Start adding userdata to the client side implementation
=======
>>>>>>> Start adding userdata to the client side implementation
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

#ifndef AUTHSESSIONIMPL_H
#define AUTHSESSIONIMPL_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QQueue>
#include <QDateTime>

#include "authsession.h"
#include "dbusinterface.h"
#include "dbusoperationqueuehandler.h"

namespace SignOn {

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Merge & cleanup from master
/*!
 * @class AuthSessionImpl
 * AuthSession class implementation.
 * @sa AuthSession
 */
class AuthSessionImpl: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AuthSessionImpl)
<<<<<<< HEAD
=======
    /*!
     * @class AuthSessionImpl
     * AuthSession class implementation.
     * @sa AuthSession
     */
    class AuthSessionImpl : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(AuthSessionImpl)
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        Q_PROPERTY(QVariant userdata READ userdata WRITE setUserdata);
>>>>>>> Start adding userdata to the client side implementation
=======
        Q_PROPERTY(QVariant applicationContext READ applicationContext WRITE setApplicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        Q_PROPERTY(QVariant userdata READ userdata WRITE setUserdata);
>>>>>>> Start adding userdata to the client side implementation
=======
        Q_PROPERTY(QVariant applicationContext READ applicationContext WRITE setApplicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
    Q_PROPERTY(QVariant applicationContext READ applicationContext WRITE setApplicationContext);
>>>>>>> Merge & cleanup from master

    friend class AuthSession;
    friend class IdentityImpl;

<<<<<<< HEAD
<<<<<<< HEAD
public:
    AuthSessionImpl(AuthSession *parent, quint32 id,
                    const QString &methodName);
    ~AuthSessionImpl();
=======
    public:
        AuthSessionImpl(AuthSession *parent,
                        quint32 id,
                        const QString &methodName,
<<<<<<< HEAD
<<<<<<< HEAD
                        const QVariant &applicationContextP);
=======
                        const QVariant &userdataP);
>>>>>>> Finalize API changes and implementation for user data
        ~AuthSessionImpl();
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Start adding userdata to the client side implementation
        QVariant userdata() const
        { return m_userdata; }
        void setUserdata (const QVariant &newUserdata)
        { m_userdata = newUserdata; }
<<<<<<< HEAD
>>>>>>> Start adding userdata to the client side implementation
=======
=======
                        const QVariant &applicationContextP);
        ~AuthSessionImpl();
>>>>>>> Rename 'userdata' to 'applicationContext'
        QVariant applicationContext() const
=======
public:
    AuthSessionImpl(AuthSession *parent,
                    quint32 id,
                    const QString &methodName,
                    const QVariant &applicationContextP);
    ~AuthSessionImpl();
    QVariant applicationContext() const
>>>>>>> Merge & cleanup from master
        { return m_applicationContext; }
    void setApplicationContext (const QVariant &newApplicationContext)
        { m_applicationContext = newApplicationContext; }
<<<<<<< HEAD
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
>>>>>>> Start adding userdata to the client side implementation
=======
>>>>>>> Rename 'userdata' to 'applicationContext'

public Q_SLOTS:
    QString name();
    void queryAvailableMechanisms(const QStringList &wantedMechanisms);
    void process(const SessionData &sessionData, const QString &mechanism);
    void cancel();

private Q_SLOTS:
    void errorSlot(const QDBusError &err);
    void authenticationSlot(const QString &path);
    void mechanismsAvailableSlot(const QStringList &mechanisms);
    void responseSlot(const QVariantMap &sessionDataVa);
    void stateSlot(int state, const QString &message);
    void unregisteredSlot();

private:
    void send2interface(const QString &operation,
                        const char *slot, const QVariantList &arguments);
    void setId(quint32 id);
    bool checkConnection();
    bool initInterface();

private:
    AuthSession *m_parent;
    DBusOperationQueueHandler m_operationQueueHandler;
    quint32 m_id;
    QString m_methodName;
    DBusInterface *m_DBusInterface;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Merge & cleanup from master
    /*
     * flag to prevent multiple authentication requests
     */
    bool m_isAuthInProcessing;
    /*
     * busy flag for process operation
     */
    bool m_isBusy;
    /*
     * valid flag for authentication: if
     * authentication failed once we do not try anymore
     */
    bool m_isValid;
<<<<<<< HEAD
};
=======
        /*
         * flag to prevent multiple authentication requests
         * */
        bool m_isAuthInProcessing;
        /*
         * busy flag for process operation
         * */
        bool m_isBusy;
        /*
         * valid flag for authentication: if
         * authentication failed once we do not try anymore
         * */
        bool m_isValid;

<<<<<<< HEAD
<<<<<<< HEAD
        QVariant m_applicationContext;
=======
        QVariant m_userdata;
>>>>>>> Start adding userdata to the client side implementation
=======
        QVariant m_applicationContext;
>>>>>>> Rename 'userdata' to 'applicationContext'
    };
>>>>>>> Start adding userdata to the client side implementation
=======
    /*
     * application level context data
     */
    QVariant m_applicationContext;
};
>>>>>>> Merge & cleanup from master

} //namespace SignOn

#endif //AUTHSESSIONIMPL_H

