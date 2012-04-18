/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2011-2012 Intel Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
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

#include <iostream>
#include <QVariantMap>

#include "signond-common.h"
#include "signonidentity.h"
#include "signonui_interface.h"
#include "SignOn/uisessiondata.h"
#include "SignOn/uisessiondata_priv.h"
#include "signoncommon.h"

#include "accesscontrolmanagerhelper.h"
#include "signonidentityadaptor.h"

#define SIGNON_RETURN_IF_CAM_UNAVAILABLE(_ret_arg_) do {                          \
        if (!(CredentialsAccessManager::instance()->credentialsSystemOpened())) { \
            sendErrorReply(internalServerErrName, \
                           internalServerErrStr + \
                           QLatin1String("Could not access Signon Database."));\
            return _ret_arg_;           \
        }                               \
    } while(0)

namespace SignonDaemonNS {

const QString internalServerErrName = SIGNOND_INTERNAL_SERVER_ERR_NAME;
const QString internalServerErrStr = SIGNOND_INTERNAL_SERVER_ERR_STR;

SignonIdentity::SignonIdentity(quint32 id, int timeout,
                               SignonDaemon *parent):
    SignonDisposable(timeout, parent),
    m_pInfo(NULL),
    m_pSignonDaemon(parent),
    m_registered(false)
{
    m_id = id;

    /*
     * creation of unique name for the given identity
     * */
    static quint32 incr = 0;
    QString objectName = SIGNOND_DAEMON_OBJECTPATH + QLatin1String("/Identity_")
                         + QString::number(incr++, 16);
    setObjectName(objectName);

    m_signonui = new SignonUiAdaptor(
                                    SIGNON_UI_SERVICE,
                                    SIGNON_UI_DAEMON_OBJECTPATH,
                                    SIGNOND_BUS,
                                    this);
}

SignonIdentity::~SignonIdentity()
{
    if (m_registered)
    {
        emit unregistered();
        QDBusConnection connection = SIGNOND_BUS;
        connection.unregisterObject(objectName());
    }

    if (credentialsStored())
        m_pSignonDaemon->m_storedIdentities.remove(m_id);
    else
        m_pSignonDaemon->m_unstoredIdentities.remove(objectName());

    delete m_signonui;
}

bool SignonIdentity::init()
{
    QDBusConnection connection = SIGNOND_BUS;

    if (!connection.isConnected()) {
        QDBusError err = connection.lastError();
        TRACE() << "Connection cannot be established:" <<
            err.errorString(err.type()) ;
        return false;
    }

    QDBusConnection::RegisterOptions registerOptions =
        QDBusConnection::ExportAllContents;

#ifndef SIGNON_DISABLE_ACCESS_CONTROL
    (void)new SignonIdentityAdaptor(this);
    registerOptions = QDBusConnection::ExportAdaptors;
#endif

    if (!connection.registerObject(objectName(), this, registerOptions)) {
        TRACE() << "Object cannot be registered: " << objectName();
        return false;
    }

    return (m_registered = true);
}

SignonIdentity *SignonIdentity::createIdentity(quint32 id, SignonDaemon *parent)
{
    SignonIdentity *identity =
        new SignonIdentity(id, parent->identityTimeout(), parent);

    if (!identity->init()) {
        TRACE() << "The created identity is invalid and will be deleted.\n";
        delete identity;
        return NULL;
    }

    return identity;
}

void SignonIdentity::destroy()
{
    if (m_registered)
    {
        emit unregistered();
        QDBusConnection connection = SIGNOND_BUS;
        connection.unregisterObject(objectName());
        m_registered = false;
    }

    deleteLater();
}

SignonIdentityInfo SignonIdentity::queryInfo(bool &ok, bool queryPassword)
{
    ok = true;

    bool needLoadFromDB = true;
    if (m_pInfo) {
        needLoadFromDB = false;
        if (queryPassword && m_pInfo->password().isEmpty()) {
            needLoadFromDB = true;
        }
    }

<<<<<<< HEAD
<<<<<<< HEAD
    if (needLoadFromDB) {
        if (m_pInfo != 0) {
            delete m_pInfo;
        }
=======
    SignonIdentityInfo SignonIdentity::queryInfo(bool &ok,
                                                 const QDBusVariant &applicationContext,
                                                 bool queryPassword)
    {
        Q_UNUSED(applicationContext);
=======
    SignonIdentityInfo SignonIdentity::queryInfo(bool &ok,
                                                 const QDBusVariant &applicationContext,
                                                 bool queryPassword)
    {
<<<<<<< HEAD
        Q_UNUSED(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        Q_UNUSED(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

        ok = true;
>>>>>>> Use QDBusVariant instead of QVariant

        CredentialsDB *db =
            CredentialsAccessManager::instance()->credentialsDB();
        m_pInfo = new SignonIdentityInfo(db->credentials(m_id, queryPassword));

        if (db->lastError().isValid()) {
            ok = false;
            delete m_pInfo;
            m_pInfo = NULL;
            return SignonIdentityInfo();
        }
    }

<<<<<<< HEAD
<<<<<<< HEAD
    /* Make sure that we clear the password, if the caller doesn't need it */
    SignonIdentityInfo info = *m_pInfo;
    if (!queryPassword) {
        info.setPassword(QString());
    }
    return info;
}
=======
    bool SignonIdentity::addReference(const QString &reference,
                                      const QDBusVariant &applicationContext)
    {
        Q_UNUSED(applicationContext);
=======
    bool SignonIdentity::addReference(const QString &reference,
                                      const QDBusVariant &applicationContext)
    {
<<<<<<< HEAD
        Q_UNUSED(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        Q_UNUSED(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

        TRACE() << "addReference: " << reference;
>>>>>>> Use QDBusVariant instead of QVariant

bool SignonIdentity::addReference(const QString &reference)
{
    TRACE() << "addReference: " << reference;

    SIGNON_RETURN_IF_CAM_UNAVAILABLE(false);

    CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
    if (db == NULL) {
        BLAME() << "NULL database handler object.";
        return false;
    }
    QString appId =
        AccessControlManagerHelper::instance()->appIdOfPeer(
                                 (static_cast<QDBusContext>(*this)).message());
    keepInUse();
    return db->addReference(m_id, appId, reference);
}

<<<<<<< HEAD
<<<<<<< HEAD
bool SignonIdentity::removeReference(const QString &reference)
{
    TRACE() << "removeReference: " << reference;
=======
    bool SignonIdentity::removeReference(const QString &reference,
                                         const QDBusVariant &applicationContext)
    {
        Q_UNUSED(applicationContext);
=======
    bool SignonIdentity::removeReference(const QString &reference,
                                         const QDBusVariant &applicationContext)
    {
<<<<<<< HEAD
        Q_UNUSED(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        Q_UNUSED(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

        TRACE() << "removeReference: " << reference;
>>>>>>> Use QDBusVariant instead of QVariant

    SIGNON_RETURN_IF_CAM_UNAVAILABLE(false);

    CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
    if (db == NULL) {
        BLAME() << "NULL database handler object.";
        return false;
    }
    QString appId =
        AccessControlManagerHelper::instance()->appIdOfPeer(
                                  (static_cast<QDBusContext>(*this)).message());
    keepInUse();
    return db->removeReference(m_id, appId, reference);
}

<<<<<<< HEAD
<<<<<<< HEAD
quint32 SignonIdentity::requestCredentialsUpdate(const QString &displayMessage)
{
    SIGNON_RETURN_IF_CAM_UNAVAILABLE(SIGNOND_NEW_IDENTITY);

    bool ok;
    SignonIdentityInfo info = queryInfo(ok, false);
=======
    quint32 SignonIdentity::requestCredentialsUpdate(const QString &displayMessage,
                                                     const QDBusVariant &applicationContext)
=======
    quint32 SignonIdentity::requestCredentialsUpdate(const QString &displayMessage,
<<<<<<< HEAD
                                                     const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                                     const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        SIGNON_RETURN_IF_CAM_UNAVAILABLE(SIGNOND_NEW_IDENTITY);

        bool ok;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        SignonIdentityInfo info = queryInfo(ok, userdata, false);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        SignonIdentityInfo info = queryInfo(ok, applicationContext, false);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        SignonIdentityInfo info = queryInfo(ok, userdata, false);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        SignonIdentityInfo info = queryInfo(ok, applicationContext, false);
>>>>>>> Rename 'userdata' to 'applicationContext'

    if (!ok) {
        BLAME() << "Identity not found.";
        sendErrorReply(SIGNOND_IDENTITY_NOT_FOUND_ERR_NAME,
                       SIGNOND_IDENTITY_NOT_FOUND_ERR_STR);
        return SIGNOND_NEW_IDENTITY;
    }
    if (!info.storePassword()) {
        BLAME() << "Password cannot be stored.";
        sendErrorReply(SIGNOND_STORE_FAILED_ERR_NAME,
                       SIGNOND_STORE_FAILED_ERR_STR);
        return SIGNOND_NEW_IDENTITY;
    }

    //delay dbus reply, ui interaction might take long time to complete
    setDelayedReply(true);
    m_message = message();

    //create ui request to ask password
    QVariantMap uiRequest;
    uiRequest.insert(SSOUI_KEY_QUERYPASSWORD, true);
    uiRequest.insert(SSOUI_KEY_USERNAME, info.userName());
    uiRequest.insert(SSOUI_KEY_MESSAGE, displayMessage);
    uiRequest.insert(SSOUI_KEY_CAPTION, info.caption());

    TRACE() << "Waiting for reply from signon-ui";
    QDBusPendingCallWatcher *watcher =
        new QDBusPendingCallWatcher(m_signonui->queryDialog(uiRequest), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(queryUiSlot(QDBusPendingCallWatcher*)));

    setAutoDestruct(false);
    return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
QVariantMap SignonIdentity::getInfo()
{
    TRACE() << "QUERYING INFO";
=======
    QList<QVariant> SignonIdentity::queryInfo(const QDBusVariant &userdata)
=======
    QList<QVariant> SignonIdentity::queryInfo(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
    QVariantMap SignonIdentity::getInfo(const QDBusVariant &applicationContext)
>>>>>>> Use QDBusVariant instead of QVariant
    {
        TRACE() << "QUERYING INFO";
>>>>>>> Use QDBusVariant instead of QVariant

    SIGNON_RETURN_IF_CAM_UNAVAILABLE(QVariantMap());

<<<<<<< HEAD
    bool ok;
    SignonIdentityInfo info = queryInfo(ok, false);
=======
        bool ok;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        SignonIdentityInfo info = queryInfo(ok, userdata, false);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        SignonIdentityInfo info = queryInfo(ok, applicationContext, false);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        SignonIdentityInfo info = queryInfo(ok, userdata, false);
=======
        SignonIdentityInfo info = queryInfo(ok, applicationContext, false);
>>>>>>> Rename 'userdata' to 'applicationContext'

        if (!ok) {
            TRACE();
            sendErrorReply(SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_NAME,
                           SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_STR +
                           QLatin1String("Database querying error occurred."));
            return QVariantMap();
        }
>>>>>>> Use QDBusVariant instead of QVariant

    if (!ok) {
        TRACE();
        sendErrorReply(SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_NAME,
                       SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_STR +
                       QLatin1String("Database querying error occurred."));
        return QVariantMap();
    }

    if (info.isNew()) {
        TRACE();
        sendErrorReply(SIGNOND_IDENTITY_NOT_FOUND_ERR_NAME,
                       SIGNOND_IDENTITY_NOT_FOUND_ERR_STR);
        return QVariantMap();
    }

    keepInUse();
    return info.toMap();
}

void SignonIdentity::queryUserPassword(const QVariantMap &params) {
    TRACE() << "Waiting for reply from signon-ui";
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
            m_signonui->queryDialog(params), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this,
            SLOT(verifyUiSlot(QDBusPendingCallWatcher*)));

<<<<<<< HEAD
<<<<<<< HEAD
    setAutoDestruct(false);
}

bool SignonIdentity::verifyUser(const QVariantMap &params)
{
    SIGNON_RETURN_IF_CAM_UNAVAILABLE(false);

    bool ok;
    SignonIdentityInfo info = queryInfo(ok, true);
=======
    bool SignonIdentity::verifyUser(const QVariantMap &params,
                                    const QDBusVariant &applicationContext)
=======
    bool SignonIdentity::verifyUser(const QVariantMap &params,
<<<<<<< HEAD
                                    const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                    const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        SIGNON_RETURN_IF_CAM_UNAVAILABLE(false);

        bool ok;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        SignonIdentityInfo info = queryInfo(ok, userdata, true);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        SignonIdentityInfo info = queryInfo(ok, applicationContext, true);
>>>>>>> Rename 'userdata' to 'applicationContext'
=======
        SignonIdentityInfo info = queryInfo(ok, userdata, true);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        SignonIdentityInfo info = queryInfo(ok, applicationContext, true);
>>>>>>> Rename 'userdata' to 'applicationContext'

    if (!ok) {
        BLAME() << "Identity not found.";
        sendErrorReply(SIGNOND_IDENTITY_NOT_FOUND_ERR_NAME,
                       SIGNOND_IDENTITY_NOT_FOUND_ERR_STR);
        return false;
    }
    if (!info.storePassword() || info.password().isEmpty()) {
        BLAME() << "Password is not stored.";
        sendErrorReply(SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_NAME,
                       SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_STR);
        return false;
    }

    //delay dbus reply, ui interaction might take long time to complete
    setDelayedReply(true);
    m_message = message();

    //create ui request to ask password
    QVariantMap uiRequest;
    uiRequest.unite(params);
    uiRequest.insert(SSOUI_KEY_QUERYPASSWORD, true);
    uiRequest.insert(SSOUI_KEY_USERNAME, info.userName());
    uiRequest.insert(SSOUI_KEY_CAPTION, info.caption());

    queryUserPassword(uiRequest);
    return false;
}

<<<<<<< HEAD
bool SignonIdentity::verifySecret(const QString &secret)
{
    SIGNON_RETURN_IF_CAM_UNAVAILABLE(false);

<<<<<<< HEAD
    bool ok;
    queryInfo(ok);
    if (!ok) {
        TRACE();
        sendErrorReply(SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_NAME,
=======
    bool SignonIdentity::verifySecret(const QString &secret,
                                      const QDBusVariant &applicationContext)
=======
    bool SignonIdentity::verifySecret(const QString &secret,
<<<<<<< HEAD
                                      const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                      const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        SIGNON_RETURN_IF_CAM_UNAVAILABLE(false);

        bool ok;
<<<<<<< HEAD
<<<<<<< HEAD
        queryInfo(ok, applicationContext);
=======
        queryInfo(ok, userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        queryInfo(ok, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'
        if (!ok) {
            TRACE();
            replyError(SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_NAME,
>>>>>>> Use QDBusVariant instead of QVariant
                       SIGNOND_CREDENTIALS_NOT_AVAILABLE_ERR_STR +
                       QLatin1String("Database querying error occurred."));
        return false;
    }

    CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
    bool ret = db->checkPassword(m_pInfo->id(), m_pInfo->userName(), secret);

    keepInUse();
    return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
void SignonIdentity::remove()
{
    SIGNON_RETURN_IF_CAM_UNAVAILABLE();
=======
    void SignonIdentity::remove(const QDBusVariant &userdata)
=======
    void SignonIdentity::remove(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        Q_UNUSED(applicationContext);
=======
    void SignonIdentity::remove(const QDBusVariant &userdata)
    {
        Q_UNUSED(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
    void SignonIdentity::remove(const QDBusVariant &applicationContext)
    {
        Q_UNUSED(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

        SIGNON_RETURN_IF_CAM_UNAVAILABLE();
>>>>>>> Use QDBusVariant instead of QVariant

    CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
    if ((db == 0) || !db->removeCredentials(m_id)) {
        TRACE() << "Error occurred while inserting/updating credentials.";
        sendErrorReply(SIGNOND_REMOVE_FAILED_ERR_NAME,
                       SIGNOND_REMOVE_FAILED_ERR_STR +
                       QLatin1String("Database error occurred."));
        return;
    }
    emit infoUpdated((int)SignOn::IdentityRemoved);
    keepInUse();
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
bool SignonIdentity::signOut()
{
    TRACE() << "Signout request. Identity ID: " << id();
    /*
     * - If the identity is stored (thus registered here)
     * signal 'sign out' to all identities subsribed to this object,
     * otherwise the only identity subscribed to this is the newly
     * created client side identity, which called this method.
     * - This is just a safety check, as the client identity - if it is a new
     * one - should not inform server side to sign out.
     */
    if (id() != SIGNOND_NEW_IDENTITY) {
        //clear stored sessiondata
        CredentialsDB *db =
            CredentialsAccessManager::instance()->credentialsDB();
        if ((db == 0) || !db->removeData(m_id)) {
            TRACE() << "clear data failed";
=======
    bool SignonIdentity::signOut(const QDBusVariant &userdata)
=======
    bool SignonIdentity::signOut(const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        Q_UNUSED(applicationContext);
=======
    bool SignonIdentity::signOut(const QDBusVariant &userdata)
    {
        Q_UNUSED(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
    bool SignonIdentity::signOut(const QDBusVariant &applicationContext)
    {
        Q_UNUSED(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

        TRACE() << "Signout request. Identity ID: " << id();
        /*
           - If the identity is stored (thus registered here)
           signal 'sign out' to all identities subsribed to this object,
           otherwise the only identity subscribed to this is the newly
           created client side identity, which called this method.
           - This is just a safety check, as the client identity - if it is a new one -
           should not inform server side to sign out.
        */
        if (id() != SIGNOND_NEW_IDENTITY) {
            //clear stored sessiondata
            CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
            if ((db == 0) || !db->removeData(m_id)) {
                TRACE() << "clear data failed";
            }

            emit infoUpdated((int)SignOn::IdentitySignedOut);
>>>>>>> Use QDBusVariant instead of QVariant
        }

        emit infoUpdated((int)SignOn::IdentitySignedOut);
    }
    keepInUse();
    return true;
}

<<<<<<< HEAD
quint32 SignonIdentity::store(const QVariantMap &info)
{
    keepInUse();
    SIGNON_RETURN_IF_CAM_UNAVAILABLE(SIGNOND_NEW_IDENTITY);

<<<<<<< HEAD
    QString secret = info.value(SIGNOND_IDENTITY_INFO_SECRET).toString();
    QString appId =
        AccessControlManagerHelper::instance()->appIdOfPeer(
                                 (static_cast<QDBusContext>(*this)).message());
=======
    quint32 SignonIdentity::store(const QVariantMap &info,
                                  const QDBusVariant &applicationContext)
=======
    quint32 SignonIdentity::store(const QVariantMap &info,
<<<<<<< HEAD
                                  const QDBusVariant &userdata)
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                  const QDBusVariant &applicationContext)
>>>>>>> Rename 'userdata' to 'applicationContext'
    {
        keepInUse();
        SIGNON_RETURN_IF_CAM_UNAVAILABLE(SIGNOND_NEW_IDENTITY);
>>>>>>> Use QDBusVariant instead of QVariant

    bool storeSecret = info.value(SIGNOND_IDENTITY_INFO_STORESECRET).toBool();
    QVariant container = info.value(SIGNOND_IDENTITY_INFO_AUTHMETHODS);
    MethodMap methods =
        qdbus_cast<MethodMap>(container.value<QDBusArgument>());

<<<<<<< HEAD
    //Add creator to owner list if it has AID
    QStringList ownerList =
        info.value(SIGNOND_IDENTITY_INFO_OWNER).toStringList();
    if (!appId.isNull())
        ownerList.append(appId);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    if (m_pInfo == 0) {
        m_pInfo = new SignonIdentityInfo(info);
        m_pInfo->setMethods(methods);
        m_pInfo->setOwnerList(ownerList);
    } else {
        QString userName =
            info.value(SIGNOND_IDENTITY_INFO_USERNAME).toString();
        QString caption =
            info.value(SIGNOND_IDENTITY_INFO_CAPTION).toString();
        QStringList realms =
            info.value(SIGNOND_IDENTITY_INFO_REALMS).toStringList();
        QStringList accessControlList =
            info.value(SIGNOND_IDENTITY_INFO_ACL).toStringList();
        int type = info.value(SIGNOND_IDENTITY_INFO_TYPE).toInt();
=======
        //Add creator to owner list if it has AID
=======
>>>>>>> returning error codes when setting isn't allowed
=======
>>>>>>> adding ac fixes
        QStringList ownerList = info.value(SIGNOND_IDENTITY_INFO_OWNER).toStringList();
        if (appId.isEmpty() && ownerList.isEmpty()) {
<<<<<<< HEAD
=======
        QStringList ownerList = info.value(SIGNOND_IDENTITY_INFO_OWNER).toStringList();
        if (appId.isEmpty() && ownerList.isEmpty()) {
>>>>>>> returning error codes when setting isn't allowed
            /* send an error reply, because otherwise we may end up with empty owner */
            sendErrorReply(SIGNOND_INVALID_QUERY_ERR_NAME,
                           SIGNOND_INVALID_QUERY_ERR_STR);
            return 0;
<<<<<<< HEAD
=======
            //blame again and don't allow such thing to happen, because otherwise we may end up with empty owner
>>>>>>> signonidentity additional checks
=======
>>>>>>> returning error codes when setting isn't allowed
        }
        /* if owner list is empty, add the appId of application to it by default */
        if (ownerList.isEmpty())
            ownerList.append(appId);
        else {
<<<<<<< HEAD
<<<<<<< HEAD
            /* check that application is allowed to set the specified list of owners */
            bool allowed = AccessControlManagerHelper::instance()->isACLValid(
                            (static_cast<QDBusContext>(*this)).message(),ownerList);
            if (!allowed) {
                /* send an error reply, because otherwise uncontrolled sharing might happen */
                replyError(SIGNOND_PERMISSION_DENIED_ERR_NAME,
                           SIGNOND_PERMISSION_DENIED_ERR_STR); 
                return 0;
=======
            // check that application is allowed to set the list of owners in this way
            // let's use the same function as for acl since rules are the same
            bool allowed = AccessControlManagerHelper::instance()->isPeerAllowedToSetACL((static_cast<QDBusContext>(*this)).message(),ownerList);
            if (!allowed) {
                // blame and don't allow this to happen. 
>>>>>>> signonidentity additional checks
=======
            /* check that application is allowed to set the specified list of owners */
            bool allowed = AccessControlManagerHelper::instance()->isACLValid(
                            (static_cast<QDBusContext>(*this)).message(),ownerList);
            if (!allowed) {
                /* send an error reply, because otherwise uncontrolled sharing might happen */
                sendErrorReply(SIGNOND_PERMISSION_DENIED_ERR_NAME,
                               SIGNOND_PERMISSION_DENIED_ERR_STR); 
                return 0;
>>>>>>> returning error codes when setting isn't allowed
            }
        }

        if (m_pInfo == 0) {
            m_pInfo = new SignonIdentityInfo(info);
            m_pInfo->setMethods(SignonIdentityInfo::mapVariantToMapList(methods));
            m_pInfo->setOwnerList(ownerList);
        } else {
            QString userName = info.value(SIGNOND_IDENTITY_INFO_USERNAME).toString();
            QString caption = info.value(SIGNOND_IDENTITY_INFO_CAPTION).toString();
            QStringList realms = info.value(SIGNOND_IDENTITY_INFO_REALMS).toStringList();
            QStringList accessControlList = info.value(SIGNOND_IDENTITY_INFO_ACL).toStringList();
            /* before setting this ACL value to the new identity, 
               we need to make sure that it isn't unconrolled sharing attempt.*/
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
            bool allowed = AccessControlManagerHelper::instance()->isACLValid(
                            (static_cast<QDBusContext>(*this)).message(),accessControlList);
            if (!allowed) {
                /* send an error reply, because otherwise uncontrolled sharing might happen */
                replyError(SIGNOND_PERMISSION_DENIED_ERR_NAME,
                           SIGNOND_PERMISSION_DENIED_ERR_STR);
                return 0;
=======
            bool allowed = AccessControlManagerHelper::instance()->isPeerAllowedToSetACL((static_cast<QDBusContext>(*this)).message(),accessControlList);
            if (!allowed) {
                // blame and don't allow this to happen.
>>>>>>> signonidentity additional checks
=======
            bool allowed = AccessControlManagerHelper::instance()->isPeerAllowedToSetACL(
=======
            bool allowed = AccessControlManagerHelper::instance()->isACLValid(
>>>>>>> changing ACL function name
                            (static_cast<QDBusContext>(*this)).message(),accessControlList);
            if (!allowed) {
                /* send an error reply, because otherwise uncontrolled sharing might happen */
                sendErrorReply(SIGNOND_PERMISSION_DENIED_ERR_NAME,
                               SIGNOND_PERMISSION_DENIED_ERR_STR);
                return 0;
>>>>>>> returning error codes when setting isn't allowed
            }
            int type = info.value(SIGNOND_IDENTITY_INFO_TYPE).toInt();
>>>>>>> signonidentity additional checks

        m_pInfo->setUserName(userName);
        m_pInfo->setCaption(caption);
        m_pInfo->setMethods(methods);
        m_pInfo->setRealms(realms);
        m_pInfo->setAccessControlList(accessControlList);
        m_pInfo->setOwnerList(ownerList);
        m_pInfo->setType(type);
    }

<<<<<<< HEAD
    if (storeSecret) {
        m_pInfo->setPassword(secret);
    } else {
        m_pInfo->setPassword(QString());
    }
    m_id = storeCredentials(*m_pInfo, storeSecret);
=======
        if (storeSecret) {
            m_pInfo->setPassword(secret);
        } else {
            m_pInfo->setPassword(QString());
        }
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        m_id = storeCredentials(*m_pInfo, storeSecret, userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        m_id = storeCredentials(*m_pInfo, storeSecret, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

    if (m_id == SIGNOND_NEW_IDENTITY) {
        sendErrorReply(SIGNOND_STORE_FAILED_ERR_NAME,
                       SIGNOND_STORE_FAILED_ERR_STR);
    }
=======
        m_id = storeCredentials(*m_pInfo, storeSecret, userdata);
>>>>>>> Use QDBusVariant instead of QVariant

<<<<<<< HEAD
    return m_id;
}
=======
    quint32 SignonIdentity::storeCredentials(const quint32 id,
                                             const QString &userName,
                                             const QString &secret,
                                             const bool storeSecret,
                                             const QMap<QString, QVariant> &methods,
                                             const QString &caption,
                                             const QStringList &realms,
                                             const QStringList &accessControlList,
                                             const int type,
                                             const QDBusVariant &applicationContext)
    {
        keepInUse();
        SIGNON_RETURN_IF_CAM_UNAVAILABLE(SIGNOND_NEW_IDENTITY);
>>>>>>> Use QDBusVariant instead of QVariant

quint32 SignonIdentity::storeCredentials(const SignonIdentityInfo &info,
                                         bool storeSecret)
{
    CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
    if (db == NULL) {
        BLAME() << "NULL database handler object.";
        return SIGNOND_NEW_IDENTITY;
    }

    bool newIdentity = info.isNew();
=======
        m_id = storeCredentials(*m_pInfo, storeSecret, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

    if (newIdentity)
        m_id = db->insertCredentials(info, storeSecret);
    else
        db->updateCredentials(info, storeSecret);

<<<<<<< HEAD
<<<<<<< HEAD
    if (db->errorOccurred()) {
        if (newIdentity)
            m_id = SIGNOND_NEW_IDENTITY;
=======
        storeCredentials(*m_pInfo, storeSecret, userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
        storeCredentials(*m_pInfo, storeSecret, applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

        TRACE() << "Error occurred while inserting/updating credentials.";
    } else {
        if (m_pInfo) {
            delete m_pInfo;
            m_pInfo = NULL;
        }
        m_pSignonDaemon->identityStored(this);

<<<<<<< HEAD
        //If secrets db is not available cache auth. data.
        if (!db->isSecretsDBOpen()) {
            AuthCache *cache = new AuthCache;
            cache->setUsername(info.userName());
            cache->setPassword(info.password());
            AuthCoreCache::instance()->insert(
                AuthCoreCache::CacheId(m_id, AuthCoreCache::AuthMethod()),
                cache);
=======
        return m_id;
    }

    quint32 SignonIdentity::storeCredentials(const SignonIdentityInfo &info,
                                             bool storeSecret,
<<<<<<< HEAD
<<<<<<< HEAD
                                             const QDBusVariant &applicationContext)
    {
        Q_UNUSED(applicationContext);
=======
                                             const QDBusVariant &userdata)
    {
        Q_UNUSED(userdata);
>>>>>>> Use QDBusVariant instead of QVariant
=======
                                             const QDBusVariant &applicationContext)
    {
        Q_UNUSED(applicationContext);
>>>>>>> Rename 'userdata' to 'applicationContext'

        CredentialsDB *db = CredentialsAccessManager::instance()->credentialsDB();
        if (db == NULL) {
            BLAME() << "NULL database handler object.";
            return SIGNOND_NEW_IDENTITY;
>>>>>>> Use QDBusVariant instead of QVariant
        }
        TRACE() << "FRESH, JUST STORED CREDENTIALS ID:" << m_id;
        emit infoUpdated((int)SignOn::IdentityDataUpdated);
    }
    return m_id;
}

void SignonIdentity::queryUiSlot(QDBusPendingCallWatcher *call)
{
    TRACE();
    setAutoDestruct(true);

    QDBusMessage errReply;
    QDBusPendingReply<QVariantMap> reply;
    if (call != NULL) {
        reply = *call;
        call->deleteLater();
    }
    QVariantMap resultParameters;
    if (!reply.isError() && reply.count()) {
        resultParameters = reply.argumentAt<0>();
    } else {
        errReply =
            m_message.createErrorReply(
                                  SIGNOND_IDENTITY_OPERATION_CANCELED_ERR_NAME,
                                  SIGNOND_IDENTITY_OPERATION_CANCELED_ERR_STR);
        SIGNOND_BUS.send(errReply);
        return;
    }

    if (!resultParameters.contains(SSOUI_KEY_ERROR)) {
        //no reply code
        errReply = m_message.createErrorReply(SIGNOND_INTERNAL_SERVER_ERR_NAME,
                SIGNOND_INTERNAL_SERVER_ERR_STR);
        SIGNOND_BUS.send(errReply);
        return;
    }

    int errorCode = resultParameters.value(SSOUI_KEY_ERROR).toInt();
    TRACE() << "error: " << errorCode;
    if (errorCode != QUERY_ERROR_NONE) {
        if (errorCode == QUERY_ERROR_CANCELED)
            errReply =
                m_message.createErrorReply(
                                  SIGNOND_IDENTITY_OPERATION_CANCELED_ERR_NAME,
                                  SIGNOND_IDENTITY_OPERATION_CANCELED_ERR_STR);
        else
            errReply =
                m_message.createErrorReply(SIGNOND_INTERNAL_SERVER_ERR_NAME,
                    QString(QLatin1String("signon-ui call returned error %1")).
                    arg(errorCode));

        SIGNOND_BUS.send(errReply);
        return;
    }

    if (resultParameters.contains(SSOUI_KEY_PASSWORD)) {
        CredentialsDB *db =
            CredentialsAccessManager::instance()->credentialsDB();
        if (db == NULL) {
            BLAME() << "NULL database handler object.";
            errReply = m_message.createErrorReply(SIGNOND_STORE_FAILED_ERR_NAME,
                    SIGNOND_STORE_FAILED_ERR_STR);
            SIGNOND_BUS.send(errReply);
            return;
        }

        //store new password
        if (m_pInfo) {
            m_pInfo->setPassword(resultParameters[SSOUI_KEY_PASSWORD].toString());

            quint32 ret = db->updateCredentials(*m_pInfo, true);
            delete m_pInfo;
            m_pInfo = NULL;
            if (ret != SIGNOND_NEW_IDENTITY) {
                QDBusMessage dbusreply = m_message.createReply();
                dbusreply << quint32(m_id);
                SIGNOND_BUS.send(dbusreply);
                return;
            } else{
                BLAME() << "Error during update";
            }
        }
    }

    //this should not happen, return error
    errReply = m_message.createErrorReply(SIGNOND_INTERNAL_SERVER_ERR_NAME,
            SIGNOND_INTERNAL_SERVER_ERR_STR);
    SIGNOND_BUS.send(errReply);
    return;
}

void SignonIdentity::verifyUiSlot(QDBusPendingCallWatcher *call)
{
    TRACE();
    setAutoDestruct(true);

    QDBusMessage errReply;
    QDBusPendingReply<QVariantMap> reply;
    if (call != NULL) {
        reply = *call;
        call->deleteLater();
    }
    QVariantMap resultParameters;
    if (!reply.isError() && reply.count()) {
        resultParameters = reply.argumentAt<0>();
    } else {
        errReply =
            m_message.createErrorReply(
                                 SIGNOND_IDENTITY_OPERATION_CANCELED_ERR_NAME,
                                 SIGNOND_IDENTITY_OPERATION_CANCELED_ERR_STR);
        SIGNOND_BUS.send(errReply);
        return;
    }

    if (!resultParameters.contains(SSOUI_KEY_ERROR)) {
        //no reply code
        errReply = m_message.createErrorReply(SIGNOND_INTERNAL_SERVER_ERR_NAME,
                SIGNOND_INTERNAL_SERVER_ERR_STR);
        SIGNOND_BUS.send(errReply);
        return;
    }

    int errorCode = resultParameters.value(SSOUI_KEY_ERROR).toInt();
    TRACE() << "error: " << errorCode;
    if (errorCode != QUERY_ERROR_NONE) {
        if (errorCode == QUERY_ERROR_CANCELED)
            errReply = m_message.createErrorReply(
                                  SIGNOND_IDENTITY_OPERATION_CANCELED_ERR_NAME,
                                  SIGNOND_IDENTITY_OPERATION_CANCELED_ERR_STR);
        else if (errorCode == QUERY_ERROR_FORGOT_PASSWORD)
            errReply = m_message.createErrorReply(
                                  SIGNOND_FORGOT_PASSWORD_ERR_NAME,
                                  SIGNOND_FORGOT_PASSWORD_ERR_STR);
        else
            errReply = m_message.createErrorReply(
                                  SIGNOND_INTERNAL_SERVER_ERR_NAME,
                                  QString(QLatin1String("signon-ui call "
                                                        "returned error %1")).
                                  arg(errorCode));

        SIGNOND_BUS.send(errReply);
        return;
    }

    if (resultParameters.contains(SSOUI_KEY_PASSWORD)) {
        CredentialsDB *db =
            CredentialsAccessManager::instance()->credentialsDB();
        if (db == NULL) {
            BLAME() << "NULL database handler object.";
            errReply = m_message.createErrorReply(SIGNOND_STORE_FAILED_ERR_NAME,
                    SIGNOND_STORE_FAILED_ERR_STR);
            SIGNOND_BUS.send(errReply);
            return;
        }

        //compare passwords
        if (m_pInfo) {
            bool ret =
                m_pInfo->password() == resultParameters[SSOUI_KEY_PASSWORD].
                toString();

            if (!ret && resultParameters.contains(SSOUI_KEY_CONFIRMCOUNT)) {
                int count = resultParameters[SSOUI_KEY_CONFIRMCOUNT].toInt();
                TRACE() << "retry count:" << count;
                if (count > 0) { //retry
                    resultParameters[SSOUI_KEY_CONFIRMCOUNT] = (count-1);
                    resultParameters[SSOUI_KEY_MESSAGEID] =
                        QUERY_MESSAGE_NOT_AUTHORIZED;
                    queryUserPassword(resultParameters);
                    return;
                } else {
                    //TODO show error note here if needed
                }
            }
            delete m_pInfo;
            m_pInfo = NULL;
            QDBusMessage dbusreply = m_message.createReply();
            dbusreply << ret;
            SIGNOND_BUS.send(dbusreply);
            return;
        }
    }
    //this should not happen, return error
    errReply = m_message.createErrorReply(SIGNOND_INTERNAL_SERVER_ERR_NAME,
            SIGNOND_INTERNAL_SERVER_ERR_STR);
    SIGNOND_BUS.send(errReply);
    return;
}

} //namespace SignonDaemonNS
