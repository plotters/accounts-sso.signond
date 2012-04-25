/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 *
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
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
#include <QVariant>
#include "testpluginproxy.h"

#include <sys/types.h>
#include <pwd.h>

void TestPluginProxy::initTestCase()
{
    m_proxy = NULL;

#ifndef NO_SIGNON_USER
    QVERIFY2(!::getuid(), "test must be run as root");
    //check signon user

    struct passwd *signonUser = getpwnam("signon");
    QVERIFY2(signonUser,
             "signon user do not exist, add with 'useradd --system signon'");
#endif

    qRegisterMetaType<QVariantMap>("QVariantMap");
}

void TestPluginProxy::cleanupTestCase()
{
    delete m_proxy;

}

void TestPluginProxy::create_nonexisting()
{
    PluginProxy *pp = PluginProxy::createNewPluginProxy("nonexisting");
    QVERIFY(pp == NULL);
}

void TestPluginProxy::create_dummy()
{

    PluginProxy *pp = PluginProxy::createNewPluginProxy("ssotest");
    QVERIFY(pp != NULL);

    m_proxy = pp;
}

void TestPluginProxy::type_for_dummy()
{
    QString type =  m_proxy->type();
    QVERIFY(type == "ssotest");
}

void TestPluginProxy::mechanisms_for_dummy()
{
    QStringList mechs =  m_proxy->mechanisms();
    QStringList pattern;

    pattern << "mech1";
    pattern << "mech2";
    pattern << "mech3";
    pattern << "BLOB";

    QVERIFY(mechs == pattern);
}

void TestPluginProxy::process_for_dummy()
{
    SessionData inData;

    inData.setRealm("testRealm");
    inData.setUserName("testUsername");

    QVariantMap inDataV;

    foreach(QString key, inData.propertyNames())
        inDataV[key] = inData.getProperty(key);

    QSignalSpy spyResult(m_proxy,
               SIGNAL(processResultReply(const QString&, const QVariantMap&)));
    QSignalSpy spyState(m_proxy,
                    SIGNAL(stateChanged(const QString&, int, const QString&)));
    QEventLoop loop;

    QObject::connect(m_proxy,
                 SIGNAL(processResultReply(const QString&, const QVariantMap&)),
                 &loop,
                 SLOT(quit()));

    QTimer::singleShot(10*1000, &loop, SLOT(quit()));

    QString cancelKey = QUuid::createUuid().toString();
    bool res = m_proxy->process(cancelKey, inDataV, "mech1");
    QVERIFY(res);

    loop.exec();

    QCOMPARE(spyResult.count(), 1);
    QCOMPARE(spyState.count(), 10);

    QVariantMap outData = spyResult.at(0).at(1).toMap();

    qDebug() << outData;

    QVERIFY(outData.contains("UserName") &&
            outData["UserName"] == "testUsername");
    QVERIFY(outData.contains("Realm") &&
            outData["Realm"] == "testRealm_after_test");
}

void TestPluginProxy::processUi_for_dummy()
{
    SessionData inData;

    inData.setRealm("testRealm");
    inData.setUserName("testUsername");
    inData.setUiPolicy(NoUserInteractionPolicy);

    QVariantMap inDataV;

    foreach(QString key, inData.propertyNames())
        inDataV[key] = inData.getProperty(key);

    QSignalSpy spyResult(m_proxy,
              SIGNAL(processResultReply(const QString&, const QVariantMap&)));
    QSignalSpy spyError(m_proxy,
                   SIGNAL(processError(const QString&, int, const QString&)));
    QSignalSpy spyUi(m_proxy,
                SIGNAL(processUiRequest(const QString&, const QVariantMap&)));
    QEventLoop loop;

    QObject::connect(m_proxy,
                 SIGNAL(processResultReply(const QString&, const QVariantMap&)),
                 &loop,
                 SLOT(quit()));

    QObject::connect(m_proxy,
                     SIGNAL(processError(const QString&, int, const QString&)),
                     &loop,
                     SLOT(quit()));


    QObject::connect(m_proxy,
                     SIGNAL(processUiRequest(const QString&,const QVariantMap&)),
                     &loop,
                     SLOT(quit()));

    QTimer::singleShot(10*1000, &loop, SLOT(quit()));

    QString cancelKey = QUuid::createUuid().toString();
    bool res = m_proxy->process(cancelKey, inDataV, "mech2");
    QVERIFY(res);

    loop.exec();

    QCOMPARE(spyUi.count(), 0);
    QCOMPARE(spyError.count(), 1);

    QString key = spyError.at(0).at(0).toString();
    int err = spyError.at(0).at(1).toInt();
    QString errMsg = spyError.at(0).at(2).toString();

    qDebug() << err;
    qDebug() << errMsg;

    QVERIFY(err == Error::NotAuthorized);

    inDataV["UiPolicy"] = 0;
    res = m_proxy->process(cancelKey, inDataV, "mech2");
    QVERIFY(res);

    loop.exec();

    QCOMPARE(spyUi.count(), 1);

}

void TestPluginProxy::process_and_cancel_for_dummy()
{
    SessionData inData;

    inData.setSecret("testSecret");
    inData.setUserName("testUsername");

    QVariantMap inDataV;

    foreach(QString key, inData.propertyNames())
        inDataV[key] = inData.getProperty(key);

    QSignalSpy spyResult(m_proxy, SIGNAL(processResultReply(const QString&, const QVariantMap&)));
    QSignalSpy spyError(m_proxy, SIGNAL(processError(const QString&, int, const QString&)));

    QEventLoop loop;

    QObject::connect(m_proxy,
                     SIGNAL(processResultReply(const QString&, const QVariantMap&)),
                     &loop,
                     SLOT(quit()));

    QObject::connect(m_proxy,
                     SIGNAL(processError(const QString&, int, const QString&)),
                     &loop,
                     SLOT(quit()));

    QTimer::singleShot(0.2*1000, m_proxy, SLOT(cancel()));
    QTimer::singleShot(10*1000, &loop, SLOT(quit()));

    QString cancelKey = QUuid::createUuid().toString();
    bool res = m_proxy->process(cancelKey, inDataV, "mech1");
    QVERIFY(res);
    loop.exec();

    QCOMPARE(spyResult.count(), 0);
    QCOMPARE(spyError.count(), 1);

    QString key = spyError.at(0).at(0).toString();
    int err = spyError.at(0).at(1).toInt();
    QString errMsg = spyError.at(0).at(2).toString();

    qDebug() << err;
    qDebug() << errMsg;

    QVERIFY(key == cancelKey);
    QVERIFY(err == Error::SessionCanceled);
    QVERIFY(errMsg == QString("The operation is canceled"));
}

void TestPluginProxy::process_wrong_mech_for_dummy()
{
    SessionData inData;

    inData.setSecret("testSecret");
    inData.setUserName("testUsername");

    QVariantMap inDataV;

    foreach(QString key, inData.propertyNames())
        inDataV[key] = inData.getProperty(key);

    QSignalSpy spyResult(m_proxy, SIGNAL(processResultReply(const QString&, const QVariantMap&)));
    QSignalSpy spyError(m_proxy, SIGNAL(processError(const QString&, int, const QString&)));

    QEventLoop loop;

    QObject::connect(m_proxy,
                     SIGNAL(processResultReply(const QString&, const QVariantMap&)),
                     &loop,
                     SLOT(quit()));

    QObject::connect(m_proxy,
                     SIGNAL(processError(const QString&, int, const QString&)),
                     &loop,
                     SLOT(quit()));

    QString cancelKey = QUuid::createUuid().toString();
    bool res = m_proxy->process(cancelKey, inDataV, "wrong");
    QVERIFY(res);

    loop.exec();

    QCOMPARE(spyResult.count(), 0);
    QCOMPARE(spyError.count(), 1);

    QString key = spyError.at(0).at(0).toString();
    int err = spyError.at(0).at(1).toInt();
    QString errMsg = spyError.at(0).at(2).toString();

    qDebug() << err << " " << errMsg;

    QVERIFY(key == cancelKey);
    QVERIFY(err == Error::MechanismNotAvailable);
    QVERIFY(errMsg == QString("The given mechanism is unavailable"));
}

void TestPluginProxy::wrong_user_for_dummy()
{
    if (::getuid()) {
        QSKIP("This test need to be run as root", SkipSingle);
    }

#ifndef NO_SIGNON_USER
    QProcess *pluginProcess = new QProcess(this);
    QVERIFY(pluginProcess->execute(QString("/usr/bin/signonpluginprocess"))==2);
#endif
}

#if defined(SSO_CI_TESTMANAGEMENT)
void TestPluginProxy::runAllTests()
{
    initTestCase();
    create_nonexisting();
    create_dummy();
    type_for_dummy();
    mechanisms_for_dummy();
    process_for_dummy();
    process_wrong_mech_for_dummy();
    process_and_cancel_for_dummy();
    cleanupTestCase();
}
#else
QTEST_MAIN(TestPluginProxy)
#endif
