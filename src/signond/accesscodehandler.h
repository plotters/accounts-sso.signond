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

/*!
 * @file accesscodehandler.h
 * @brief Definition of the access code (sim, lock code) handler object.
 */

#ifndef ACCESSCODEHANDLER_H
#define ACCESSCODEHANDLER_H

#include <QRunnable>
#include <QMutex>
#include <QObject>

namespace SignonDaemonNS {

    class SimDBusAdaptor;

    /*!
     * @class AccessCodeHandler
     * AccessCodeHandler handles getting the access code (SIM pin or device lock code)
     * which is used by the CryptoManager as a key for the mounting of the encrypted file system.
     * @ingroup Accounts_and_SSO_Framework
     * AccessCodeHandler inherits QObject and QRunnable.
     *@sa SimDBusAdaptor
     * @sa CryptoManager
     */
    class AccessCodeHandler : public QObject, public QRunnable
    {
        Q_OBJECT

        friend class SimDBusAdaptor;

    public:
        /*!
         * @enum CodeType
         * @brief Describes the code type to be handled.
         */
        enum CodeType {
            SIM_PIN = 0,
            LOCK_CODE,
            UNKNOWN
        };

        /*!
          * Constructs an AcessCodeHandler object with the given parent.
          * @param parent the parent object
          * @sa AccessCodeHandler(const CodeType &type, QObject *parent = 0)
          */
        AccessCodeHandler(QObject *parent = 0);

        /*!
          * @brief Constructs an AcessCodeHandler object with the given parent and code type
          * @param type type of code expected to be handled
          * @param parent the parent object
          * @sa AccessCodeHandler(const CodeType &type, QObject *parent = 0)
          */
        AccessCodeHandler(const CodeType &type, QObject *parent = 0);

        /*!
          * Destructor, releases allocated resources.
          * AccessCodeHandler::finalize() must be called first.
          * Future versions will include auto finalize feature.
          */
        virtual ~AccessCodeHandler();

        /*!
          * Initializes the AccessCodeHandler object.
          * AccessCodeHandler::newSimAvailable(const QString &simPin) will be emitted upon successfull SIM interogation.
          * Device lock code not supported yet.
          * @retval true uppon success. Error handling not supported yet.
          */
        bool initialize();

        /*!
          *  Finalizes the AccessCodeHandler object, stopping all running tasks and the SimDBusAdaptor.
          */
        void finalize();

        /*!
         * @retval true, if an access code is available for using, false otherwise.
         */
        bool codeAvailable();

        /*!
          * @retval true, if the access code changed.
          * @deprecated this method is currently deprecated.
          */
        bool codeChanged();

        /*!
          * Loop method, checking for the access code. Uses thread prioritisation.
          */
        virtual void run();


        /*!
            @returns the currently available access code.
        */
        QString currentCode();

        /*!
            @returns the code type in use.
        */
        CodeType codeType() const { return m_codeType; }

        /*!
            Sets the code type in use.
            @param codeType the code type in use.
        */
        void setType(const CodeType &codeType);

    Q_SIGNALS:
        /*!
            Is emitted when a new SIM is available.
            @param simPin, the newly available SIM pin.
        */
        void newSimAvailable(const QString &simPin);

    private Q_SLOTS:
        void simChanged(const QString &newSimPin);

    private:
        SimDBusAdaptor *m_pSimDBusAdaptor;
        QString m_code;
        CodeType m_codeType;
        bool m_codeChanged;
        QMutex m_mutex;
    };

} // namespace SignonDaemonNS

#endif // ACCESSCODEHANDLER_H
