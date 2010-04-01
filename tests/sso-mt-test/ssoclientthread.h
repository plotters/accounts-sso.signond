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

#ifndef SSOCLIENTTHREAD_H
#define SSOCLIENTTHREAD_H

#include <QThread>

#include "ssotestclient.h"

#define NUMBER_OF_TEST_CLIENTS 5

class SsoClientThread : public QThread
{
    Q_OBJECT

public:
    SsoClientThread(SsoTestClient::TestType type);
    ~SsoClientThread();

    bool isPrimary();
    void run();

private Q_SLOTS:
    void startTests();
    void stopAllExceptPrimary();

Q_SIGNALS:
    void reallyStarted();
    void runTests();
    void done();

private:
    SsoTestClient *m_pTestClient;
    SsoTestClient::TestType m_testType;

    int m_id;
};

#endif // SSOCLIENTTHREAD_H
