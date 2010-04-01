/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 *
 * Contact: Aurel Popirtac <ext-Aurel.Popirtac@nokia.com>
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

/*!
  @file simdbusadaptor.h
  Definition of the Sim DBUS Adaptor object.
  @ingroup Accounts_and_SSO_Framework
 */

#ifndef SIMDBUSADAPTOR_H
#define SIMDBUSADAPTOR_H

#include <QtDBus>

namespace SignonDaemonNS {

    /*!
        @class SimDBusAdaptor
        Handles getting the SIM pin via DBUS.
        @ingroup Accounts_and_SSO_Framework
    */
    class SimDBusAdaptor : public QDBusAbstractAdaptor
    {
        Q_OBJECT

    public:
        /*!
            Constructs a SimDBusAdaptor object with the given parent.
            @param parent
        */
        SimDBusAdaptor(QObject *parent = 0);

        /*!
            Destroys a SimDBusAdaptor object.
        */
        virtual ~SimDBusAdaptor();

        /*!
            Initializes the SimDBusAdaptor object.
            @returns true, upon success, false otherwise.
        */
        bool initialize();

        /*!
            Does a check for the SIM pin via DBUS.
            @todo this should be changed on libsim lib --> cellmo, so that signal is emitted on the cellmo side not on the client side
        */
        void checkSim();

    Q_SIGNALS:
        /*!
            Is emitted when a new SIM pin is available.
            @param newSimPint, the new SIM pin.
        */
        void simChanged(const QString &newSimPin);

    private:
        void displayError(const QDBusError &error);
        void checkDBusInterfaces();

    private:
        QDBusConnection m_DBusConnection;
    };

} // namespace SignonDaemonNS

#endif // SIMDBUSADAPTOR_H
