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

#include "ssotestclient.h"

#include <QtTest/QtTest>
#include <QtCore>

class LibsigonQtTest : public QObject
{
     Q_OBJECT

 private Q_SLOTS:

    void runAuthSessionTests();
     void runAuthServiceTests();
     void runIdentityTests();


 private:
     SsoTestClient testClient;
};

void LibsigonQtTest::runAuthServiceTests()
{
    testClient.runAuthServiceTests();
}

void LibsigonQtTest::runIdentityTests()
{
    testClient.runIdentityTests();
}

void LibsigonQtTest::runAuthSessionTests()
{
    testClient.runAuthSessionTests();
}

QTEST_MAIN(LibsigonQtTest);
#include "libsignon-qt-tests.moc"

