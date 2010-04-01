/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
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

#include "simdbusadaptor.h"
#include "signond-common.h"

#include <QCoreApplication>

namespace SignonDaemonNS {

    static const QString adaptorName = QLatin1String("SingleSignon.Signond.SimAdaptor");

    SimDBusAdaptor::SimDBusAdaptor(QObject *parent)
            : QDBusAbstractAdaptor(parent),
              m_DBusConnection(QDBusConnection::connectToBus(
                QDBusConnection::SessionBus, adaptorName))

    {
        setObjectName(QLatin1String("/Signond/SimAdaptor"));
    }

    SimDBusAdaptor::~SimDBusAdaptor()
    {
        //disconnect from DBUS here
        m_DBusConnection.unregisterObject(this->objectName());

        if (!m_DBusConnection.unregisterService(adaptorName)) {
            TRACE() << "Could not unregister DBUS service: " << adaptorName;
        }
        m_DBusConnection.disconnectFromBus(adaptorName);
    }

    bool SimDBusAdaptor::initialize()
    {
        //remove
        //checkDBusInterfaces();
        //
        if (!m_DBusConnection.isConnected()) {
            TRACE() << "SIM DBUS Adaptor connection FAILED";
            displayError(m_DBusConnection.lastError());
            return false;
        }

        if (!m_DBusConnection.registerService(adaptorName)) {
            TRACE() << "SIM DBUS Service cannot be registered";
            displayError(m_DBusConnection.lastError());
            return false;
        }

        if (!m_DBusConnection.registerObject(this->objectName(), this,
                                             QDBusConnection::ExportAllSlots)) {
            TRACE() << "SIM DBUS Object cannot be registered";
            displayError(m_DBusConnection.lastError());
            return false;
        }

        //display purposes method call
        checkDBusInterfaces();
        return true;
    }

    void SimDBusAdaptor::checkSim()
    {
        //"Phone.Sim.get_imsi" - todo, make the intefrace names and objects paths right, remove hardcodings

        static QString simPin = QLatin1String("");

        QDBusInterface simInterface(QLatin1String("Phone.Sim"),
                                    QLatin1String("/Phone/Sim"),
                                    QLatin1String(""),
                                    m_DBusConnection);
        QDBusReply<QVariant> reply = simInterface.callWithArgumentList(
                QDBus::Block,QLatin1String("get_imsi"), QList<QVariant>() << QLatin1String("arg0"));
        void displayError(const QDBusError &error);
        void checkDBusInterfaces();
        if (reply.isValid()) {
            QVariant value =  reply.value();
            if (simPin != value.toString()) {
                simPin = value.toString();
                emit simChanged(simPin);
            }
        }
        //else
            //this->displayError(reply.error());

        // --------------------- Dummy Demo only handling - remove when DBUS functionality is working
        QFile simFile(QCoreApplication::applicationDirPath() + QLatin1String("/SIM"));

        if (!simFile.open(QIODevice::ReadOnly)) {
            return;
        }

        QString sim = QString::fromLatin1(simFile.readAll().constData()).simplified();

        if (sim != simPin) {
            simPin = sim;
            emit simChanged(simPin);
        }
        // -------------------- Dummy Demo only handling END
    }

    void SimDBusAdaptor::checkDBusInterfaces()
    {
        QDBusConnectionInterface *interface = m_DBusConnection.interface();

        QDBusReply<QStringList> reply = interface->registeredServiceNames();

        QStringList list = reply.value();

        QString servicesList = QLatin1String("DBUS registered services: \n");
        servicesList += list.join(QLatin1String("\n"));

        TRACE() << "\n\n" << servicesList << "\n";
    }

    void SimDBusAdaptor::displayError(const QDBusError &error)
    {
        QString displayStr;
        QTextStream stream(&displayStr);
        stream << "\nError:\n Name:" << error.name();
        stream << "\n Type:";

        const char *typeStr;
        switch (error.type()) {
            case QDBusError::NoError: typeStr = "NoError"; break;
            case QDBusError::Other: typeStr = "Other"; break;
            case QDBusError::Failed: typeStr = "Failed"; break;
            case QDBusError::NoMemory: typeStr = "NoMemory"; break;
            case QDBusError::ServiceUnknown: typeStr = "ServiceUnknown"; break;
            case QDBusError::NoReply: typeStr = "NoReply"; break;
            case QDBusError::BadAddress: typeStr = "BadAddress"; break;
            case QDBusError::NotSupported: typeStr = "NotSupported"; break;
            case QDBusError::LimitsExceeded: typeStr = "LimitsExceeded"; break;
            case QDBusError::AccessDenied: typeStr = "AccessDenied"; break;
            case QDBusError::NoServer: typeStr = "NoServer"; break;
            case QDBusError::Timeout: typeStr = "Timeout"; break;
            case QDBusError::NoNetwork: typeStr = "NoNetwork"; break;
            case QDBusError::AddressInUse: typeStr = "AddressInUse"; break;
            case QDBusError::Disconnected: typeStr = "Disconnected"; break;
            case QDBusError::InvalidArgs: typeStr = "InvalidArgs"; break;
            case QDBusError::UnknownMethod: typeStr = "UnknownMethod"; break;
            case QDBusError::TimedOut: typeStr = "TimedOut"; break;
            case QDBusError::InvalidSignature: typeStr = "InvalidSignature"; break;
            case QDBusError::UnknownInterface: typeStr = "UnknownInterface"; break;
            case QDBusError::InternalError: typeStr = "InternalError"; break;
            case QDBusError::UnknownObject: typeStr = "UnknownObject"; break;
            default:  typeStr = "DEFAULT -- UKNOWN ERROR"; break;
        }
        stream << typeStr;
        stream << "\n Message:";
        stream << error.message();
        TRACE() << displayStr;
    }

} // namespace SignonDaemonNS
