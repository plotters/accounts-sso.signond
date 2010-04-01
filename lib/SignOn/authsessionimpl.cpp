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

#include <QUuid>

#include "authsessionimpl.h"
#include "libsignoncommon.h"

#define SSO_AUTHSESSION_CONNECTION_PROBLEM \
    QLatin1String("Cannot create remote AuthSession object: " \
                  "check the signon daemon and authentication plugin.")

#define SSO_SESSION_PROCESS_METHOD \
    SSO_NORMALIZE_METHOD_SIGNATURE("process(const SessionData &, const QString &)")
#define SSO_SESSION_SET_ID_METHOD \
    SSO_NORMALIZE_METHOD_SIGNATURE("setId(quint32)")
#define SSO_SESSION_QUERY_AVAILABLE_MECHANISMS_METHOD \
    SSO_NORMALIZE_METHOD_SIGNATURE("queryAvailableMechanisms(const QStringList &)")

#ifndef SSO_NEW_IDENTITY
    #define SSO_NEW_IDENTITY 0
#endif

namespace SignOn {

    static QVariantMap sessionData2VariantMap(const SessionData &data)
    {
        QVariantMap result;

        foreach(QString key, data.propertyNames()) {
            if (!data.getProperty(key).isNull() && data.getProperty(key).isValid())
                result[key] = data.getProperty(key);
        }

        return result;
    }

    AuthSessionImpl::AuthSessionImpl(AuthSession *parent, quint32 id, const QString &methodName)
        : QObject(parent),
          m_parent(parent),
          m_operationQueueHandler(this),
          m_methodName(methodName)
    {
        m_id = id;
        m_DBusInterface = 0;
        m_isValid = true;
        m_isAuthInProcessing = false;
        m_isBusy = false;
        initInterface();
    }

    AuthSessionImpl::~AuthSessionImpl()
    {
        if (m_DBusInterface) {
            m_DBusInterface->call(QLatin1String("objectUnref"));
            delete m_DBusInterface;
        }
    }

    void AuthSessionImpl::send2interface(const QString &operation, const char *slot, const QVariantList &arguments)
    {
        if (!m_DBusInterface || !m_DBusInterface->isValid()) {
            emit m_parent->error(AuthSession::UnknownError,
                                 QLatin1String("general error in AuthSession: "
                                               "cannot register interface"));
            return;
        }

        bool res = true;

        if (slot) {
            /*
             * TODO: Invent something more attractive for the case
             * of operation call with big timeout
             * */
            if (QLatin1String("process") != operation) {
                res = m_DBusInterface->callWithCallback(operation,
                                                    arguments,
                                                    this,
                                                    slot,
                                                    SLOT(errorSlot(const QDBusError&)));
            } else {
                QDBusMessage msg = QDBusMessage::createMethodCall(m_DBusInterface->service(),
                                                                  m_DBusInterface->path(),
                                                                  m_DBusInterface->interface(),
                                                                  operation);
                msg.setArguments(arguments);
                res = m_DBusInterface->connection().callWithCallback(msg,
                                                                 this,
                                                                 slot,
                                                                 SLOT(errorSlot(const QDBusError&)),
                                                                 SIGNON_MAX_TIMEOUT);
            }
        } else {
            m_DBusInterface->callWithArgumentList(QDBus::NoBlock, operation, arguments);
        }

        if (!res)
            emit m_parent->error(AuthSession::UnknownError, m_DBusInterface->lastError().message());
        else
            emit m_parent->stateChanged(AuthSession::ProcessPending,
                                        QLatin1String("The request is added to queue."));
    }

    void AuthSessionImpl::setId(quint32 id)
    {
        if (id == SSO_NEW_IDENTITY) {
            qCritical() << "wrong value for credentials id";
            return;
        }

        if (!m_isValid) {
            emit m_parent->error(AuthSession::UnknownError,
                QString(QLatin1String("AuthSession(%1) cannot perform operation"))
                .arg(m_methodName));
            return;
        }

        m_id = id;

        QLatin1String remoteFunctionName("setId");

        QVariantList arguments;
        arguments += id;

        if (m_DBusInterface)
            send2interface(remoteFunctionName, 0, arguments);
        else
            m_operationQueueHandler.enqueueOperation(
                                        SSO_SESSION_SET_ID_METHOD,
                                        QList<QGenericArgument *>() << (new Q_ARG(quint32, id)));
    }

    bool AuthSessionImpl::checkConnection()
    {
        /*
         * if the AuthSession is unable to connect to SignonDaemon
         * or was refused to create a remote AuthSession then
         * there is no sense to try again (????)
         *
         * */
        if (!m_isValid)
            return false;

        if (m_isAuthInProcessing)
            return true;

        if (!m_DBusInterface || m_DBusInterface->lastError().isValid())
            return initInterface();

        TRACE();
        return true;
    }

    bool AuthSessionImpl::initInterface()
    {
        TRACE();
        m_isAuthInProcessing = true;

        delete m_DBusInterface;
        m_DBusInterface = 0;

        QVariantList arguments;
        QLatin1String operation("getAuthSessionObjectPath");

        arguments += m_id;
        arguments += m_methodName;

        QDBusInterface iface(SIGNON_SERVICE,
                             SIGNON_DAEMON_OBJECTPATH,
                             SIGNON_DAEMON_INTERFACE);

        if (iface.lastError().isValid()) {
            qCritical() << "cannot initialize interface: " << iface.lastError();
            m_isValid = false;
            return false;
        } else {
            return iface.callWithCallback(operation,
                                          arguments,
                                          this,
                                          SLOT(authenticationSlot(const QString&)),
                                          SLOT(errorSlot(const QDBusError&)));
        }
    }

    QString AuthSessionImpl::name()
    {
        return m_methodName;
    }

    void AuthSessionImpl::queryAvailableMechanisms(const QStringList &wantedMechanisms)
    {
        if (!checkConnection()) {
            qCritical() << SSO_AUTHSESSION_CONNECTION_PROBLEM;
            emit m_parent->error(AuthSession::InternalCommunicationError,
                                 SSO_AUTHSESSION_CONNECTION_PROBLEM);
            return;
        }

        QLatin1String remoteFunctionName("queryAvailableMechanisms");

        QVariantList arguments;
        arguments += wantedMechanisms;

        if (m_DBusInterface)
            send2interface(remoteFunctionName, SLOT(mechanismsAvailableSlot(const QStringList&)), arguments);
        else
            m_operationQueueHandler.enqueueOperation(
                            SSO_SESSION_QUERY_AVAILABLE_MECHANISMS_METHOD,
                            QList<QGenericArgument *>() << (new Q_ARG(QStringList, wantedMechanisms)));
    }

    void AuthSessionImpl::process(const SessionData &sessionData, const QString &mechanism)
    {
        if (!checkConnection()) {
            qCritical() << SSO_AUTHSESSION_CONNECTION_PROBLEM;
            emit m_parent->error(AuthSession::InternalCommunicationError, SSO_AUTHSESSION_CONNECTION_PROBLEM);
            return;
        }

        if (m_isBusy) {
            qCritical() << "AuthSession: client is busy";
            emit m_parent->error(AuthSession::UnknownError,
                QString(QLatin1String("AuthSession(%1) is busy"))
                .arg(m_methodName));
            return;
        }

        QVariantMap sessionDataVa = sessionData2VariantMap(sessionData);
        QString remoteFunctionName;
        QVariantList arguments;

        arguments += sessionDataVa;
        arguments += mechanism;

        remoteFunctionName = QLatin1String("process");

        if (m_DBusInterface) {
            TRACE() << "sending to daemon";
            send2interface(remoteFunctionName, SLOT(responseSlot(const QVariantMap&)), arguments);
            m_isBusy = true;
        } else {
            TRACE() << "sending to queue";
            QList<QGenericArgument *> args;
            args << (new Q_ARG(QVariantMap, sessionDataVa))
                 << (new Q_ARG(QString, mechanism));

            m_operationQueueHandler.enqueueOperation(SSO_SESSION_PROCESS_METHOD,
                                                     args);
        }

    }

    void AuthSessionImpl::cancel()
    {
        if (!checkConnection()) {
            qCritical() << SSO_AUTHSESSION_CONNECTION_PROBLEM;
            emit m_parent->error(AuthSession::InternalCommunicationError, SSO_AUTHSESSION_CONNECTION_PROBLEM);
            return;
        }

        if (!m_isBusy && (!m_operationQueueHandler.queueContainsOperation(SSO_SESSION_PROCESS_METHOD))) {
            qCritical() << "no requests to be cancelled";
            return;
        }

        if (!m_DBusInterface) {
            m_operationQueueHandler.removeOperation(SSO_SESSION_PROCESS_METHOD);
            emit m_parent->error(AuthSession::CanceledError,
                                 QLatin1String("process is cancelled"));
        } else {
            TRACE() << "Sending cancel-request";
            m_DBusInterface->call(QDBus::NoBlock,
                    QLatin1String("cancel"));
        }

        m_isBusy = false;
    }

    void AuthSessionImpl::errorSlot(const QDBusError &err)
    {
        TRACE() << err;
        /*
         * If DBus was unable to start daemon or to recognize interface
         * then this case is definitely a problematic:
         * we disable the whole object and reply errors on each delayed operation
         * */
        if (m_isAuthInProcessing) {
            qCritical() << "Cannot connect to SignonDaemon: " << err;

            m_isValid = false;
            m_isAuthInProcessing = false;

            int numberOfErrorReplies = m_operationQueueHandler.queuedOperationsCount();
            for (int i = 0; i < numberOfErrorReplies; i++)
                emit m_parent->error(
                        AuthSession::InternalCommunicationError,
                        SSO_AUTHSESSION_CONNECTION_PROBLEM);

            return;
        }

        m_isBusy = false;
        AuthSession::AuthSessionError errCode = AuthSession::UnknownError;

        if (err.type() != QDBusError::Other) {
            qCritical() << err.type();
            qCritical() << err.name();
            qCritical() << err.message();

            /*
             * if we got another error code then we have some sort of
             * problems with DBus: try to reset DBus interface
             * */
            if (err.type() == QDBusError::UnknownObject) {
                delete m_DBusInterface;
                m_DBusInterface = NULL;
            }

            errCode = AuthSession::UnknownError;
        } else {
            if (err.name() == canceledErrorName)
                errCode = AuthSession::CanceledError;
            else if (err.name() == timedOutErrorName)
                errCode = AuthSession::TimedOutError;
            else if (err.name() == invalidCredentialsErrorName)
                errCode = AuthSession::InvalidCredentialsError;
            else if (err.name() == noConnectionErrorName)
                errCode = AuthSession::NoConnectionError;
            else if (err.name() == operationNotSupportedErrorName)
                errCode = AuthSession::OperationNotSupportedError;
            else if (err.name() == permissionDeniedErrorName)
                errCode = AuthSession::PermissionDeniedError;
            else if (err.name() == wrongStateErrorName)
                errCode = AuthSession::WrongStateError;
            else if (err.name() == mechanismNotAvailableErrorName)
                errCode = AuthSession::PermissionDeniedError;
            else if (err.name() == missingDataErrorName)
                 errCode = AuthSession::MissingDataError;
            else if (err.name() == runtimeErrorName)
                 errCode = AuthSession::RuntimeError;
            else if (err.name() == noConnectionErrorName)
                 errCode = AuthSession::NoConnectionError;
            else if (err.name() == networkErrorName)
                 errCode = AuthSession::NetworkError;
            else if (err.name() == sslErrorName)
                 errCode = AuthSession::SslError;
            else if (err.name() == userInteractionErrorName)
                 errCode = AuthSession::UserInteractionError;
            else
                errCode = AuthSession::UnknownError;
        }

        /*
         * * TODO: find a normal way how to wrap message into AuthSessionError
         * */
        emit m_parent->error(errCode, err.message());
    }

    void AuthSessionImpl::authenticationSlot(const QString &path)
    {
        if (QString() != path) {
            m_DBusInterface = new QDBusInterface(SIGNON_SERVICE,
                                                 path,
                                                 QString());
            connect(m_DBusInterface, SIGNAL(stateChanged(int, const QString&)),
                    this, SLOT(stateSlot(int, const QString&)));

            if (m_operationQueueHandler.queuedOperationsCount() > 0)
                m_operationQueueHandler.execQueuedOperations();
        } else {
            int numberOfErrorReplies = m_operationQueueHandler.queuedOperationsCount();
            for (int i = 0; i < numberOfErrorReplies; i++)
                emit m_parent->error(AuthSession::UnknownError,
                                     QLatin1String("The given session cannot be accessed"));

            m_isValid = false;
        }

        m_isAuthInProcessing = false;
        m_operationQueueHandler.clearOperationsQueue();
    }

    void AuthSessionImpl::mechanismsAvailableSlot(const QStringList& mechanisms)
    {
        emit m_parent->mechanismsAvailable(mechanisms);
    }

    void AuthSessionImpl::responseSlot(const QVariantMap &sessionDataVa)
    {
        SessionData data(sessionDataVa);
        m_isBusy = false;
        emit m_parent->response(data);
    }

    void AuthSessionImpl::stateSlot(int state, const QString &message)
    {
        emit m_parent->stateChanged((AuthSession::AuthSessionState)state, message);
    }

} //namespace SignOn
