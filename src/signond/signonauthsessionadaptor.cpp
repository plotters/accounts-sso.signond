/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 *
 * Contact: Alberto Mardegan <alberto.mardegan@nokia.com>
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

namespace SignonDaemonNS {

    SignonAuthSessionAdaptor::SignonAuthSessionAdaptor(SignonAuthSession *parent) : QDBusAbstractAdaptor(parent)
    {
        setAutoRelaySignals(true);
    }

    SignonAuthSessionAdaptor::~SignonAuthSessionAdaptor()
    {
    }

    QStringList SignonAuthSessionAdaptor::queryAvailableMechanisms(const QStringList &wantedMechanisms)
    {
        TRACE();
        return parent()->queryAvailableMechanisms(wantedMechanisms);
    }

    QVariantMap SignonAuthSessionAdaptor::process(const QVariantMap &sessionDataVa, const QString &mechanism)
    {
        TRACE();
        return parent()->process(sessionDataVa, mechanism);
    }

    void SignonAuthSessionAdaptor::cancel()
    {
        TRACE();
        parent()->cancel();
    }

    void SignonAuthSessionAdaptor::setId(quint32 id)
    {
        TRACE();
        parent()->setId(id);
    }

    void SignonAuthSessionAdaptor::objectUnref()
    {
        TRACE();
        parent()->objectUnref();
    }

} //namespace SignonDaemonNS
