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

#include <QtTest/QtTest>
#include <QtCore>

#include <sys/types.h>
#include <errno.h>

#include <signal.h>
#include "signoncommon.h"

#include "identityinfo.h"
#include "authservice.h"
#include "identity.h"

using namespace SignOn;

/*
 * test timeout 10 seconds
 * */
#define test_timeout 10000

class testSingleSignonStatus: public QObject
{
    Q_OBJECT

public :
    /*
     * The variable which keep the principal result of the test
     * */
     bool is_test_succeed;

     SignOn::Identity::IdentityError m_ierr;
     SignOn::AuthService::ServiceError m_aerr;

     QString m_errMsg;

public Q_SLOTS:

    void queryMechanismsError(AuthService::ServiceError err, const QString& message);
    void queryMechanismsSlot(const QStringList &mechs);

    void responseSlot(const QByteArray& token, const QVariantMap& param);
    void responseError(Identity::IdentityError err, const QString& message);

    void storeCredentialsSlot(const QVariantMap& info);
    void storeCredentialsError(Identity::IdentityError err, const QString& message);

    void identitiesError(AuthService::ServiceError err, const QString& message);
    void identitiesSlot(const QList<SignOn::IdentityInfo>& identities);


Q_SIGNALS:
    void testCompleted();
};


 class testSingleSignon: public QObject
 {
     Q_OBJECT

 private Q_SLOTS:

     /*
      * Start the signon daemon
      * */
     void initTestCase();

     /*
      * End the signon daemon
      * */
     void cleanupTestCase();

     /*
      * AuthService API related test cases
      * */

     void queryMechanisms_existing_service();
     void queryMechanisms_nonexisting_service();

     void queryIdentities_existing_service();
     void queryIdentities_nonexisting_service();
     void queryIdentities_nonexisting_mechanism();

     /*
      * Identity API related test cases
      * */

     void storeCredentials_with_new_identity();
     void storeCredentials_with_existing_identity();
     void storeCredentials_with_nonexisting_identity();
     void storeCredentials_with_nonexisting_type();
     void storeCredentials_with_not_allowed_mechanism();
     void storeCredentials_with_wrong_parameters();

     void response_with_new_identity();
     void response_with_existing_identity();
     void response_with_nonexisting_identity();
     void response_with_nonexisting_type();
     void response_with_wrong_type();
     void response_with_not_allowed_mechanism();
     /*
      * TODO: fulfill the unit tests with wrong credentials tests
      * */



private:
    QProcess* daemonProcess;
    testSingleSignonStatus m_teststatus;
 };

 void testSingleSignon::initTestCase()
 {
     /*
      * The forced start in advance is still required as
      * start-on-demand takes too much time and makes first tests failor
      * */
     daemonProcess = new QProcess();
     daemonProcess->start("../../src/signond/signond");
     daemonProcess->waitForStarted(10 * 1000);
     /*
      * 1 second is still required as the signon daemon needs time to be started
      * */
     sleep(1);
 }

 void testSingleSignon::cleanupTestCase()
 {
     daemonProcess->kill();
     daemonProcess->waitForFinished();

     sleep(1);

     delete daemonProcess;
 }

 void testSingleSignonStatus::queryMechanismsError(AuthService::ServiceError err, const QString &msg)
 {
     m_aerr = err;
     m_errMsg = QString(msg);

     is_test_succeed = true;
     emit testCompleted();
 }

 void testSingleSignonStatus::identitiesError(AuthService::ServiceError err, const QString& msg)
 {
     m_aerr = err;
     m_errMsg = QString(msg);

     is_test_succeed = true;
     emit testCompleted();
 }

 void testSingleSignonStatus::responseError(Identity::IdentityError err, const QString& msg)
 {
     m_ierr = err;
     m_errMsg = QString(msg);

     is_test_succeed = true;
     emit testCompleted();
 }

 void testSingleSignonStatus::storeCredentialsError(Identity::IdentityError err, const QString& msg)
 {
     m_ierr = err;
     m_errMsg = QString(msg);

     //TRACE() << msg;

     is_test_succeed = true;
     emit testCompleted();
 }

 void testSingleSignonStatus::queryMechanismsSlot(const QStringList &mechs)
 {
     int it = 1;
     char str[2];
     str[1] ='\0';

     foreach (QString mech, mechs)
     {
         QString pattern = QString("mech") + QString::number(it);

         if(mech.compare(pattern))
             break;

         it++;
     }

     if( it == 4 )
         is_test_succeed = true;

     emit testCompleted();
 }

 void testSingleSignonStatus::responseSlot(const QByteArray& token, const QMap<QString, QVariant>& param)
 {

     QCOMPARE(token, QByteArray("test_token_after_processing"));
     QCOMPARE(param.contains("test_param"), true);
     QCOMPARE(param["test_param"], QVariant(QString("test_param_after_processing")));

     is_test_succeed = true;
     emit testCompleted();
 }
//
 void testSingleSignon::queryMechanisms_existing_service()
 {
     SignOn::AuthService* service = new SignOn::AuthService("dummy",
                                                             QStringList(QString("")));

     m_teststatus.is_test_succeed = false;

     QEventLoop loop;

     QObject::connect(service, SIGNAL(mechanismsAvailable(const QStringList&)), &m_teststatus, SLOT(queryMechanismsSlot(const QStringList&)));
     QObject::connect(service, SIGNAL(error(AuthService::ServiceError, const QString&)), &loop, SLOT(quit()));
     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     service->queryMechanisms();

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     QVERIFY(m_teststatus.is_test_succeed);

     delete service;
 }

 void testSingleSignon::queryMechanisms_nonexisting_service()
 {
     //TRACE();
     SignOn::AuthService* service = new SignOn::AuthService("nonexisting",
                                                            QStringList(QString("")));

     m_teststatus.is_test_succeed = false;

     QEventLoop loop;

     QObject::connect(service, SIGNAL(mechanismsAvailable(const QStringList&)), &m_teststatus, SLOT(queryMechanismsSlot(const QStringList&)));
     QObject::connect(service, SIGNAL(error(AuthService::ServiceError, const QString&)), &m_teststatus, SLOT(queryMechanismsError(AuthService::ServiceError, const QString&)));

     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     service->queryMechanisms();

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     QVERIFY(m_teststatus.is_test_succeed);
     QCOMPARE(m_teststatus.m_aerr, AuthService::ServiceNotKnownError);
     QCOMPARE(m_teststatus.m_errMsg, serviceNotKnownErrorStr);

     delete service;
 }

 void testSingleSignonStatus::identitiesSlot(const QList<SignOn::AuthInfo>& identities)
 {
     Q_UNUSED(identities);
     is_test_succeed = true;

     emit testCompleted();
 }

 void testSingleSignon::queryIdentities_existing_service()
 {
     //TRACE();

     SignOn::AuthService* service = new SignOn::AuthService("dummy");

     QStringList wantedMech = QStringList(QString("mech1"));
     service->setMechanisms(wantedMech);

     m_teststatus.is_test_succeed = false;

     QEventLoop loop;

     QObject::connect(service, SIGNAL(identities(const QList<SignOn::AuthInfo>&)), &m_teststatus, SLOT(identitiesSlot(const QList<SignOn::AuthInfo>&)));
     QObject::connect(service, SIGNAL(error(AuthService::ServiceError, const QString&)), &loop, SLOT(quit()));
     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     service->queryIdentities();

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);

     delete service;
 }

 void testSingleSignon::queryIdentities_nonexisting_service()
 {
     //TRACE();

     SignOn::AuthService* service = new SignOn::AuthService("nonexisting");

     QStringList wantedMech = QStringList(QString("mech1"));
     service->setMechanisms(wantedMech);

     m_teststatus.is_test_succeed = false;

     QEventLoop loop;

     QObject::connect(service, SIGNAL(identities(const QList<SignOn::AuthInfo>&)), &m_teststatus, SLOT(identitiesSlot(const QList<SignOn::AuthInfo>&)));
     QObject::connect(service, SIGNAL(error(AuthService::ServiceError, const QString&)), &loop, SLOT(quit()));
     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     service->queryIdentities();

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);

     delete service;
 }

 void testSingleSignon::queryIdentities_nonexisting_mechanism()
 {
     //TRACE();

     SignOn::AuthService* service = new SignOn::AuthService("nonexisting");

     QStringList wantedMech = QStringList(QString("nonexisting"));
     service->setMechanisms(wantedMech);

     m_teststatus.is_test_succeed = false;

     QEventLoop loop;

     QObject::connect(service, SIGNAL(identities(const QList<SignOn::AuthInfo>&)), &m_teststatus, SLOT(identitiesSlot(const QList<SignOn::AuthInfo>&)));
     QObject::connect(service, SIGNAL(error(AuthService::ServiceError, const QString&)), &loop, SLOT(quit()));
     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     service->queryIdentities();

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);

     delete service;
 }

 /*
  * The unit tests for Identity::response functionality
  *
  * */


 void testSingleSignon::response_with_new_identity()
 {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     QStringList wantMechs = QStringList(QString("mech1"));

     SignOn::AuthService* service = new SignOn::AuthService("dummy", wantMechs);
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 0);

     QObject::connect(idty, SIGNAL(response(const QByteArray&, const QVariantMap&)), &m_teststatus, SLOT(responseSlot(const QByteArray&, const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(quit()));
     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QByteArray token = QByteArray("test_token_before_processing");
     QMap<QString, QVariant> param;

     param.insert(QString("user"), QVariant(QString("test_user")));
     param.insert(QString("caption"), QVariant(QString("test_caption")));
     param.insert(QString("test_param"), QVariant(QString("test_param_before_processing")));

     idty->challenge(token, param);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);

     delete service;

 }

 void testSingleSignon::response_with_existing_identity()
 {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     QStringList wantMechs = QStringList(QString("mech1"));
     SignOn::AuthService* service = new SignOn::AuthService("dummy", wantMechs);

     /*
      * There should be the existing account
      * */
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 1);

     QObject::connect(idty, SIGNAL(response(const QByteArray&, const QVariantMap&)), &m_teststatus, SLOT(responseSlot(const QByteArray&, const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(testCompleted()));
     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QByteArray token = QByteArray("test_token_before_processing");
     QMap<QString, QVariant> param;

     param.insert(QString("user"), QVariant(QString("test_user")));
     param.insert(QString("caption"), QVariant(QString("test_caption")));
     param.insert(QString("test_param"), QVariant(QString("test_param_before_processing")));

     idty->challenge(token, param);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);

     delete service;

 }

 /*
  * The expected result is getting an error signal NotFoundErr
  * */

 void testSingleSignon::response_with_nonexisting_identity()
  {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     QStringList wantMechs = QStringList(QString("mech1"));
     SignOn::AuthService* service = new SignOn::AuthService("dummy", wantMechs);

     /*
      * There should be the existing account
      * */
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 10000);

     QObject::connect(idty, SIGNAL(response(const QByteArray&, const QVariantMap&)), &m_teststatus, SLOT(responseSlot(const QByteArray&, const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(responseError(Identity::IdentityError, const QString&)));

     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QByteArray token = QByteArray("test_token_before_processing");
     QMap<QString, QVariant> param;

     param.insert(QString("user"), QVariant(QString("test_user")));
     param.insert(QString("caption"), QVariant(QString("test_caption")));
     param.insert(QString("test_param"), QVariant(QString("test_param_before_processing")));

     idty->challenge(token, param);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);
     QCOMPARE(m_teststatus.m_ierr, Identity::NotFoundError);
     QCOMPARE(m_teststatus.m_errMsg, notFoundErrorStr);

     delete idty;
     delete service;
  }


 void testSingleSignon::response_with_nonexisting_type()
  {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     QStringList wantMechs = QStringList(QString("mech1"));
     SignOn::AuthService* service = new SignOn::AuthService("nonexisting", wantMechs);

     /*
      * There should be the existing account
      * */
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 0);

     QObject::connect(idty, SIGNAL(response(const QByteArray&, const QVariantMap&)), &m_teststatus, SLOT(responseSlot(const QByteArray&, const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(responseError(Identity::IdentityError, const QString&)));

     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QByteArray token = QByteArray("test_token_before_processing");
     QMap<QString, QVariant> param;

     param.insert(QString("user"), QVariant(QString("test_user")));
     param.insert(QString("caption"), QVariant(QString("test_caption")));
     param.insert(QString("test_param"), QVariant(QString("test_param_before_processing")));

     idty->challenge(token, param);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);
     QCOMPARE(m_teststatus.m_ierr, Identity::NotFoundError);
     QCOMPARE(m_teststatus.m_errMsg, notFoundErrorStr);

     delete idty;
     delete service;
  }

  void testSingleSignon::response_with_wrong_type()
   {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     QStringList wantMechs = QStringList(QString("mech1"));
     SignOn::AuthService* service = new SignOn::AuthService("wrong", wantMechs);

 /*
      * There should be the existing account
  * */
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 1);

     QObject::connect(idty, SIGNAL(response(const QByteArray&, const QVariantMap&)), &m_teststatus, SLOT(responseSlot(const QByteArray&, const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(responseError(Identity::IdentityError, const QString&)));

     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

      QByteArray token = QByteArray("test_token_before_processing");
      QMap<QString, QVariant> param;

      param.insert(QString("user"), QVariant(QString("test_user")));
      param.insert(QString("caption"), QVariant(QString("test_caption")));
      param.insert(QString("test_param"), QVariant(QString("test_param_before_processing")));

      idty->challenge(token, param);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);
     QCOMPARE(m_teststatus.m_ierr, Identity::NotFoundError);
     QCOMPARE(m_teststatus.m_errMsg, notFoundErrorStr);

     delete idty;
     delete service;
   }


 /*
  * TODO: Currently we agreed that the AuthService::setMechanisms
  * does not have an effect on previously created Identities objects and makes effect
  * on the newly created. Otherwise we need to think about the changing the parenthood
  * relation between AuthService and Identities objects
  * */
 void testSingleSignon::response_with_not_allowed_mechanism()
 {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     QStringList wantMechs = QStringList(QString("mech1"));
     SignOn::AuthService* service = new SignOn::AuthService("wrong", wantMechs);

     /*
      * There should be the existing account
      * */
     SignOn::Identity *idty = new SignOn::Identity(service, "notallowed", 1);

    QObject::connect(idty, SIGNAL(response(const QByteArray&, const QVariantMap&)), &m_teststatus, SLOT(responseSlot(const QByteArray&, const QVariantMap&)));
    QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(responseError(Identity::IdentityError, const QString&)));

    QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

    QByteArray token = QByteArray("test_token_before_processing");
    QMap<QString, QVariant> param;

    param.insert(QString("user"), QVariant(QString("test_user")));
    param.insert(QString("caption"), QVariant(QString("test_caption")));
    param.insert(QString("test_param"), QVariant(QString("test_param_before_processing")));

    idty->challenge(token, param);

    if(!m_teststatus.is_test_succeed)
    {
        QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
        loop.exec();
    }

    //TRACE() << "wait loop is finished";
    QVERIFY(m_teststatus.is_test_succeed);
    QCOMPARE(m_teststatus.m_ierr, Identity::MechanismNotAvailableError);
    QCOMPARE(m_teststatus.m_errMsg, mechanismNotAvailableErrorStr);

     delete idty;
    delete service;

 }

 /*
  *
  * Unit tests for Identity::storeCredentials functionality
  * */

 void testSingleSignonStatus::storeCredentialsSlot(const QVariantMap& info)
  {
     //TRACE() << info;
     Q_UNUSED(info);
     is_test_succeed = true;
     emit testCompleted();
  }

  void testSingleSignon::storeCredentials_with_new_identity()
  {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     SignOn::AuthService* service = new SignOn::AuthService("dummy", QStringList(QString("mech1")));
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 0);

     QObject::connect(idty, SIGNAL(credentialsStored(const QVariantMap&)), &m_teststatus, SLOT(storeCredentialsSlot(const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &loop, SLOT(quit()));
     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QVariantMap info;

     info.insert(QString(SSO_KEY_USERNAME), QVariant(QString("test_user")));
     info.insert(QString(SSO_KEY_PASSWORD), QVariant(QString("test_password")));

     idty->storeCredentials(info);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);

     delete idty;
     delete service;

  }

  void testSingleSignon::storeCredentials_with_existing_identity()
  {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     SignOn::AuthService* service = new SignOn::AuthService("dummy", QStringList(QString("mech1")));
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 1);

     QObject::connect(idty, SIGNAL(credentialsStored(const QVariantMap&)), &m_teststatus, SLOT(storeCredentialsSlot(const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &loop, SLOT(quit()));
     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QVariantMap info;

     info.insert(QString(SSO_KEY_USERNAME), QVariant(QString("test_user")));
     info.insert(QString(SSO_KEY_PASSWORD), QVariant(QString("test_password")));

     idty->storeCredentials(info);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);

     delete service;
  }

  void testSingleSignon::storeCredentials_with_nonexisting_identity()
  {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     SignOn::AuthService* service = new SignOn::AuthService("dummy", QStringList(QString("mech1")));
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 10000);

     QObject::connect(idty, SIGNAL(credentialsStored(const QVariantMap&)), &m_teststatus, SLOT(storeCredentialsSlot(const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(storeCredentialsError(Identity::IdentityError, const QString&)));

     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QVariantMap info;

     info.insert(QString(SSO_KEY_USERNAME), QVariant(QString("test_user")));
     info.insert(QString(SSO_KEY_PASSWORD), QVariant(QString("test_password")));

     idty->storeCredentials(info);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

    //TRACE() << "wait loop is finished";
    QVERIFY(m_teststatus.is_test_succeed);
    QCOMPARE(m_teststatus.m_ierr, Identity::NotFoundError);
    QCOMPARE(m_teststatus.m_errMsg, notFoundErrorStr);

    delete idty;
    delete service;
  }

  void testSingleSignon::storeCredentials_with_nonexisting_type()
  {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     SignOn::AuthService* service = new SignOn::AuthService("nonexisting", QStringList(QString("mech1")));
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 1);

     QObject::connect(idty, SIGNAL(credentialsStored(const QVariantMap&)), &m_teststatus, SLOT(storeCredentialsSlot(const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(storeCredentialsError(Identity::IdentityError, const QString&)));

     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QVariantMap info;

     info.insert(QString(SSO_KEY_USERNAME), QVariant(QString("test_user")));
     info.insert(QString(SSO_KEY_PASSWORD), QVariant(QString("test_password")));

     idty->storeCredentials(info);

     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

    //TRACE() << "wait loop is finished";
    QVERIFY(m_teststatus.is_test_succeed);
    QCOMPARE(m_teststatus.m_ierr, Identity::NotFoundError);
    QCOMPARE(m_teststatus.m_errMsg, notFoundErrorStr);

    delete idty;
    delete service;
  }

  void testSingleSignon::storeCredentials_with_not_allowed_mechanism()
  {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     SignOn::AuthService* service = new SignOn::AuthService("dummy", QStringList(QString("mech1")));
     SignOn::Identity *idty = new SignOn::Identity(service, "notallowed", 1);

     QObject::connect(idty, SIGNAL(credentialsStored(const QVariantMap&)), &m_teststatus, SLOT(storeCredentialsSlot(const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(storeCredentialsError(Identity::IdentityError, const QString&)));

     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QVariantMap info;

     info.insert(QString(SSO_KEY_USERNAME), QVariant(QString("test_user")));
     info.insert(QString(SSO_KEY_PASSWORD), QVariant(QString("test_password")));

     idty->storeCredentials(info);


     QTimer::singleShot(test_timeout, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);
     QCOMPARE(m_teststatus.m_ierr, Identity::NotFoundError);
     QCOMPARE(m_teststatus.m_errMsg, notFoundErrorStr);

     delete idty;
     delete service;
  }

  void testSingleSignon::storeCredentials_with_wrong_parameters()
    {
     //TRACE();
     m_teststatus.is_test_succeed = false;
     QEventLoop loop;

     SignOn::AuthService* service = new SignOn::AuthService("dummy", QStringList(QString("mech1")));
     SignOn::Identity *idty = new SignOn::Identity(service, "mech1", 1);

     QObject::connect(idty, SIGNAL(credentialsStored(const QVariantMap&)), &m_teststatus, SLOT(storeCredentialsSlot(const QVariantMap&)));
     QObject::connect(idty, SIGNAL(error(Identity::IdentityError, const QString&)), &m_teststatus, SLOT(storeCredentialsError(Identity::IdentityError, const QString&)));

     QObject::connect(&m_teststatus, SIGNAL(testCompleted()), &loop, SLOT(quit()));

     QVariantMap info;

     idty->storeCredentials(info);

     QTimer::singleShot(10*1000, &loop, SLOT(quit()));
     loop.exec();

     //TRACE() << "wait loop is finished";
     QVERIFY(m_teststatus.is_test_succeed);
     QCOMPARE(m_teststatus.m_ierr, Identity::InvalidCredentialsError);
     QCOMPARE(m_teststatus.m_errMsg, invalidCredentialsErrorStr);

     delete idty;
     delete service;
  }

 QTEST_MAIN(testSingleSignon)
 #include "testsinglesignon.moc"

