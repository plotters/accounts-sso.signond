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

#include <QtTest/QtTest>
#include "authpluginstest.h"

using namespace SignonDaemonNS;

#define TEST_START qDebug("\n\n\n\n ----------------- %s ----------------\n\n",  __func__);
#define TEST_DONE  qDebug("\n\n ----------------- %s DONE ----------------\n\n",  __func__);

const QString config = QString::fromAscii("/usr/share/signon-plugin-tests/config.txt");

void AuthPluginsTest::initTestCase()
{
    TEST_START

    qDebug() << "Creating Authentication plugins pool.Checking default plugin strategy";

    m_pPluginsPool = new AuthPluginsPool;
    QVERIFY(m_pPluginsPool != NULL);

    qDebug() << "Checking if object is ok.";
    QVERIFY(m_pPluginsPool->pluginsCount() == 0);

    TEST_DONE
}

void AuthPluginsTest::cleanupTestCase()
{
    TEST_START

    qDebug() << "Clearing plugin pool.";
    m_pPluginsPool->clear();

    qDebug() << "Checking if clear succeeded.";
    QCOMPARE(m_pPluginsPool->pluginsCount(), (int)0);

    delete m_pPluginsPool;

    TEST_DONE
}

//test cases

void AuthPluginsTest::pluginsPool()
{
    TEST_START

    qDebug() << "Creating Authentication plugins pool.Checking default plugin strategy";

    delete m_pPluginsPool;
    m_pPluginsPool = new AuthPluginsPool;
    QVERIFY(m_pPluginsPool != NULL);
    QCOMPARE(m_pPluginsPool->pluginsDir(), QString(DEFAULT_PLUGINS_DIR));
    m_pPluginsPool->setPluginsDir("");
    QCOMPARE(m_pPluginsPool->pluginsDir(), QString(DEFAULT_PLUGINS_DIR));

    //testing removal of last separator
    QString testDir = "/tmp/dir";
    m_pPluginsPool->setPluginsDir(testDir);
    QCOMPARE(m_pPluginsPool->pluginsDir(), QString("/tmp/dir/"));
    delete m_pPluginsPool;

    qDebug() << "Creating Authentication plugins pool for the rest of the tests.";

    m_pPluginsPool = new AuthPluginsPool;
    QVERIFY(m_pPluginsPool != NULL);

    qDebug() << "Checking if object is ok.";
    QVERIFY(m_pPluginsPool->pluginsCount() == 0);

    TEST_DONE
}

void AuthPluginsTest::loadPlugins()
{
    TEST_START

    QSettings settings(config, QSettings::NativeFormat);

    QCOMPARE(m_pPluginsPool->pluginsDir(), settings.value(QString("plugindir"),QString("/usr/lib/signon/")).toString());

    QStringList plugins = settings.value("plugins").toStringList();

    int count = 0;
    for (int i = 0; i < plugins.size(); i++) {
        SPPluginInterface plugin = configureAndLoadPlugin(plugins.at(i));
        QVERIFY(!plugin.isNull());
        count++;
    }

    QStringList available = m_pPluginsPool->availablePluginTypes();
    if (available.size() != count) {
        QWARN("all available plugins were not loaded");
        qDebug() << "loaded: " << plugins;
        qDebug() << "available: " << available;
    }

    TEST_DONE
}

void AuthPluginsTest::unloadPlugins()
{
    TEST_START
    if(m_pPluginsPool->pluginsCount() < 1)
        qDebug() << "Must have at least one plugin loaded, in order for test to make sense.";

    m_pPluginsPool->clear();

    QVERIFY(m_pPluginsPool->pluginsCount() == 0);

    TEST_DONE
}

void AuthPluginsTest::checkPlugins()
{
    TEST_START

    QSettings settings(config, QSettings::NativeFormat);
    QStringList plugins = settings.value("plugins").toStringList();

    for (int i = 0; i < plugins.size(); i++) {
        QString name = plugins.at(i);
        SPPluginInterface plugin = configureAndLoadPlugin(name);
        QVERIFY(!plugin.isNull());
        //check that type match the plugin name
        QString type = plugin->type();
        QCOMPARE(type, name);
        //check mechanisms
        QStringList mechs = plugin->mechanisms();
        settings.beginGroup(name);
        QStringList mechsL = settings.value(QString("mechanisms")).toStringList();
        mechs.sort();
        mechsL.sort();
        QVERIFY(mechs == mechsL);
        settings.endGroup();
    }

    TEST_DONE
}

//end test cases

//private methods

SPPluginInterface AuthPluginsTest::configureAndLoadPlugin(const QString& name)
{
    PluginId pluginId = AuthPluginsPool::createId(name, 0);
    return m_pPluginsPool->loadPlugin(pluginId);
}

QTEST_MAIN(AuthPluginsTest)
