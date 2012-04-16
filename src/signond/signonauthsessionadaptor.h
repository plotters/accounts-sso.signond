/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
<<<<<<< HEAD
 * Copyright (C) 2012 Canonical Ltd.
 * Copyright (C) 2012 Intel Corporation.
 *
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
<<<<<<< HEAD
=======
 * Copyright (C) 2012 Intel Corporation.
 *
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
#ifndef SIGNONAUTHSESSIONADAPTOR_H_
#define SIGNONAUTHSESSIONADAPTOR_H_

#include <QtCore>
#include <QtDBus>

#include "signond-common.h"
#include "signonauthsession.h"

namespace SignonDaemonNS {

class SignonAuthSessionAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface",
                "com.google.code.AccountsSSO.SingleSignOn.AuthSession")

public:
    SignonAuthSessionAdaptor(SignonAuthSession *parent);
    virtual ~SignonAuthSessionAdaptor();

    inline SignonAuthSession *parent() const {
        return static_cast<SignonAuthSession *>(QObject::parent());
    }

private:
    void errorReply(const QString &name, const QString &message);

<<<<<<< HEAD
public Q_SLOTS:
    QStringList queryAvailableMechanisms(const QStringList &wantedMechanisms);
    QVariantMap process(const QVariantMap &sessionDataVa,
                        const QString &mechanism);

    Q_NOREPLY void cancel();
    Q_NOREPLY void setId(quint32 id);
    Q_NOREPLY void objectUnref();
=======
    public Q_SLOTS:
        QStringList queryAvailableMechanisms(const QStringList &wantedMechanisms,
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
                                             const QDBusVariant &applicationContext);
        QVariantMap process(const QVariantMap &sessionDataVa,
                            const QString &mechanism,
                            const QDBusVariant &applicationContext);

<<<<<<< HEAD
<<<<<<< HEAD
        Q_NOREPLY void cancel(const QVariant &userdata);
        Q_NOREPLY void setId(quint32 id, const QVariant &userdata);
        Q_NOREPLY void objectUnref(const QVariant &userdata);
>>>>>>> Add user data parameter to server side interfaces
=======
        Q_NOREPLY void cancel(const QDBusVariant &userdata);
        Q_NOREPLY void setId(quint32 id, const QDBusVariant &userdata);
        Q_NOREPLY void objectUnref(const QDBusVariant &userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        Q_NOREPLY void cancel(const QDBusVariant &applicationContext);
        Q_NOREPLY void setId(quint32 id, const QDBusVariant &applicationContext);
        Q_NOREPLY void objectUnref(const QDBusVariant &applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
                                             const QVariant &userdata);
=======
                                             const QDBusVariant &userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                             const QDBusVariant &applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
        QVariantMap process(const QVariantMap &sessionDataVa,
                            const QString &mechanism,
                            const QDBusVariant &applicationContext);

<<<<<<< HEAD
<<<<<<< HEAD
        Q_NOREPLY void cancel(const QVariant &userdata);
        Q_NOREPLY void setId(quint32 id, const QVariant &userdata);
        Q_NOREPLY void objectUnref(const QVariant &userdata);
>>>>>>> Add user data parameter to server side interfaces
=======
        Q_NOREPLY void cancel(const QDBusVariant &userdata);
        Q_NOREPLY void setId(quint32 id, const QDBusVariant &userdata);
        Q_NOREPLY void objectUnref(const QDBusVariant &userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        Q_NOREPLY void cancel(const QDBusVariant &applicationContext);
        Q_NOREPLY void setId(quint32 id, const QDBusVariant &applicationContext);
        Q_NOREPLY void objectUnref(const QDBusVariant &applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

Q_SIGNALS:
    void stateChanged(int state, const QString &message);
    void unregistered();
};

} //namespace SignonDaemonNS

#endif /* SIGNONAUTHSESSIONADAPTOR_H_ */
