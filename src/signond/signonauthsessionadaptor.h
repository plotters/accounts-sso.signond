/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation.
 *
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
        Q_CLASSINFO("D-Bus Interface", "com.nokia.SingleSignOn.AuthSession")

    public:
            SignonAuthSessionAdaptor(SignonAuthSession *parent);
            virtual ~SignonAuthSessionAdaptor();

            inline SignonAuthSession *parent() const
            {
                return static_cast<SignonAuthSession *>(QObject::parent());
            }

    private:
        void errorReply(const QString &name, const QString &message);

    public Q_SLOTS:
        QStringList queryAvailableMechanisms(const QStringList &wantedMechanisms,
                                             const QVariant &userdata);
        QVariantMap process(const QVariantMap &sessionDataVa,
                            const QString &mechanism,
                            const QVariant &userdata);

        Q_NOREPLY void cancel(const QVariant &userdata);
        Q_NOREPLY void setId(quint32 id, const QVariant &userdata);
        Q_NOREPLY void objectUnref(const QVariant &userdata);

    Q_SIGNALS:
        void stateChanged(int state, const QString &message);
        void unregistered();
    };

} //namespace SignonDaemonNS

#endif /* SIGNONAUTHSESSIONADAPTOR_H_ */
