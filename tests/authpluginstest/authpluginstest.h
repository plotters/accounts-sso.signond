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

#ifndef AUTH_PLUGINS_TEST
#define AUTH_PLUGINS_TEST

#include "authpluginspool.h"


using namespace SignonDaemonNS;

class AuthPluginsTest : public QObject
{
    Q_OBJECT

public slots:

private slots:
    void initTestCase();
    void cleanupTestCase();

    //test cases
    void pluginsPool();
    void loadPlugins();
    void unloadPlugins();
    void checkPlugins();
    //test cases

    //end test cases

private:
    SPPluginInterface configureAndLoadPlugin(const QString& name);

protected Q_SLOTS:
    //void asyncChallengeDone(const QByteArray &, const QVariantMap &, const PluginError);
Q_SIGNALS:
    void asynChallengeCompleted();

private:
    AuthPluginsPool *m_pPluginsPool;
    SPPluginInterface m_testPlugin;

    QByteArray m_replyToken;
    QVariantMap m_replyInfo;
//    PluginError m_pluginError;
};

#endif //AUTH_PLUGINS_TEST
