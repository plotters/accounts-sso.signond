/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
<<<<<<< HEAD
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
=======
 * Contact: Alberto Mardegan <alberto.mardegan@nokia.com>
 * Contact: Jussi Laako <jussi.laako@linux.intel.com>
>>>>>>> Finalize API changes and implementation for user data
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

#include <QObject>
#include <QMetaType>

#include "libsignoncommon.h"
#include "authsession.h"
#include "authsessionimpl.h"


namespace SignOn {

<<<<<<< HEAD
AuthSession::AuthSession(quint32 id, const QString &methodName,
                         QObject *parent):
    QObject(parent),
    impl(new AuthSessionImpl(this, id, methodName))
{
    qRegisterMetaType<SessionData>("SessionData");
    qRegisterMetaType<AuthSessionState>("AuthSession::AuthSessionState");
=======
    AuthSession::AuthSession(quint32 id,
                             const QString &methodName,
                             const QVariant &applicationContextP,
                             QObject *parent)
            :  QObject(parent),
               impl(new AuthSessionImpl(this, id, methodName, applicationContextP))
    {
        qRegisterMetaType<SessionData>("SessionData");
        qRegisterMetaType<AuthSessionState>("AuthSession::AuthSessionState");
>>>>>>> Finalize API changes and implementation for user data

    if (qMetaTypeId<SessionData>() < QMetaType::User)
        BLAME() << "AuthSession::AuthSession() - "
            "SessionData meta type not registered.";

<<<<<<< HEAD
    if (qMetaTypeId<AuthSessionState>() < QMetaType::User)
        BLAME() << "AuthSession::AuthSession() - "
            "AuthSessionState meta type not registered.";

}
=======
        if (qMetaTypeId<AuthSessionState>() < QMetaType::User)
            BLAME() << "AuthSession::AuthSession() - AuthSessionState meta type not registered.";
    }
>>>>>>> Finalize API changes and implementation for user data

AuthSession::~AuthSession()
{
    delete impl;
}

const QString AuthSession::name() const
{
    return impl->name();
}

void AuthSession::queryAvailableMechanisms(const QStringList &wantedMechanisms)
{
    impl->queryAvailableMechanisms(wantedMechanisms);
}

void AuthSession::process(const SessionData& sessionData,
                          const QString &mechanism)
{
    impl->process(sessionData, mechanism);
}

void AuthSession::cancel()
{
    impl->cancel();
}

} //namespace SignOn
