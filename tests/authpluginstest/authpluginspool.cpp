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

#include "authpluginspool.h"
#include "signond-common.h"

#include <QPluginLoader>
#include <QStringList>
#include <QDir>


namespace SignonDaemonNS {

    AuthPluginsPool::AuthPluginsPool(const QString &pluginsDir)
    {
        setPluginsDir(pluginsDir);
    }

    AuthPluginsPool::~AuthPluginsPool()
    {
        clear();
    }

    SPPluginInterface AuthPluginsPool::loadPlugin(const PluginId &id, const QObject *challengeReplyHandler, const char *challengeReplySlot)
    {
        if(!validId(id))
        {
            tracePluginId(QLatin1String("Invalid plugin id."), id);
            return SPPluginInterface(NULL);
        }

        PluginProxy* proxy = PluginProxy::createNewPluginProxy(id.first);
        if(proxy)
        {
            if(pluginInterfaceValid(proxy))
            {
                if(challengeReplyHandler && challengeReplySlot)
                    if(!QObject::connect(
                                    proxy,
                                    SIGNAL(challengeReply(const QByteArray &, const QVariantMap &, const PluginError)),
                                    challengeReplyHandler,
                                    challengeReplySlot), Qt::DirectConnection)
                        TRACE() << "Could not create QObject::connect for given challenge reply handler";

                return storePlugin(id, proxy);
            }
            else
            {
                serializePluginInterface(proxy, NULL);
                delete proxy;
            }
        }
        return SPPluginInterface(NULL);
    }

    bool AuthPluginsPool::unloadPlugin(const PluginId &id)
    {
        SPPluginInterface interface = m_plugins.take(id);
        if(interface.isNull())
        {
            TRACE() << QString(QLatin1String("No plugin of type '%1' loaded for the Identity '%2'."))
                .arg(id.first).arg(id.second);
            return false;
        }

        interface.clear();

        return true;
    }

    bool AuthPluginsPool::unloadPlugin(const SPPluginInterface &interface)
    {
        if(interface.isNull())
            return false;

        PluginId id = m_plugins.key(interface, PluginId(QLatin1String(""),
                                                        QLatin1String("")));
        if(!validId(id))
            return false;

        return unloadPlugin(id);
    }

    SPPluginInterface AuthPluginsPool::plugin(const PluginId &id, bool forceLoading)
    {
        if(!validId(id))
            return SPPluginInterface(NULL);

        SPPluginInterface interface = m_plugins.value(id, SPPluginInterface(NULL));

        if(interface.isNull() && forceLoading)
            return loadPlugin(id);
        else
            return interface;
    }

    bool AuthPluginsPool::clear()
    {
        bool allOk = true;
        while(!m_plugins.empty())
        {
            PluginId id = m_plugins.begin().key();
            tracePluginId(QLatin1String("Plugin id to be unloaded"), id);

            if(!unloadPlugin(id))
            {
                tracePluginId(QLatin1String("Could not successfully unload plugin"), id);
                allOk = false;
            }
        }
        return allOk;
    }

    const QStringList AuthPluginsPool::pluginTypes()
    {
        QList<PluginId> ids = m_plugins.keys();
        QListIterator<PluginId> it(ids);
        QStringList types;
        while(it.hasNext())
        {
            PluginId id = it.next();
            if(!types.contains(id.first))
                types << id.first;
        }

        return types;
    }

    const QList<SPPluginInterface> AuthPluginsPool::plugins(const QString &type) const
    {
        if(type.isNull())
            return m_plugins.values();
        else
        {
            //TODO implement this
            return m_plugins.values();
        }
    }

    int AuthPluginsPool::pluginsCount(const QString &type) const
    {
        if(type.isNull())
            return m_plugins.count();
        else
        {
            //TODO implement this
            return m_plugins.count();
        }
    }

    const QString &AuthPluginsPool::pluginsDir() const
    {
        return m_pluginsDir;
    }

    void AuthPluginsPool::setPluginsDir(const QString &pluginsDir)
    {
        if(pluginsDir == QLatin1String("."))
        {
            m_pluginsDir = QLatin1String("");
            return;
        }

        if(pluginsDir == QLatin1String(""))
            m_pluginsDir = DEFAULT_PLUGINS_DIR;
        else
            m_pluginsDir = pluginsDir;

        QDir dir = QDir(m_pluginsDir);
        if(!dir.exists())
        {
            if(!dir.mkpath(m_pluginsDir))
            {
                TRACE() << "Plugin directory does not exist. Unable to create it:" << m_pluginsDir;
                if(m_pluginsDir != DEFAULT_PLUGINS_DIR)
                    if(dir.mkpath(DEFAULT_PLUGINS_DIR))
                    {
                        m_pluginsDir = DEFAULT_PLUGINS_DIR;
                        TRACE() << "Switched to default plugin directory:" << QString(DEFAULT_PLUGINS_DIR);
                    }
            }
        }

        if(!m_pluginsDir.endsWith(QDir::separator()))
            m_pluginsDir += QDir::separator();
    }

    const PluginId AuthPluginsPool::createId(const PluginType &type, const quint32 id)
    {
        if(!id)
            return qMakePair<QString, QString>(type, QUuid::createUuid().toString());

        return qMakePair<QString, QString>(type, QString::number(id));
    }

    QStringList AuthPluginsPool::availablePluginTypes() const
    {
        QDir pluginsDir(m_pluginsDir);
        //in the future remove the sym links comment
        QStringList fileNames = pluginsDir.entryList(
                QStringList() << QLatin1String("*.so*"), QDir::Files /*| QDir::NoSymLinks*/ | QDir::NoDotAndDotDot);

        QStringList ret;
        QString fileName;
        foreach(fileName, fileNames)
        {
            if(fileName.startsWith(QLatin1String("lib")))
            {
                fileName = fileName.mid(3, fileName.indexOf(QLatin1String("plugin")) -3);
                if((fileName.length() > 0) && !ret.contains(fileName))
                    ret << fileName;
            }
        }
        return ret;
    }

    bool AuthPluginsPool::validId(const PluginId &id)
    {
        if(id.first == QLatin1String("") || id.second == QLatin1String(""))
        {
            tracePluginId(QLatin1String("Invalid plugin id"), id);
            return false;
        }
        return true;
    }

    void AuthPluginsPool::tracePluginId(const QString &message, const PluginId &id)
    {
        TRACE() << QString(QLatin1String("%1. type: %2, unique id: %3"))
            .arg(message).arg(id.first).arg(id.second);
    }

    QString AuthPluginsPool::createFileName(const QString &type)
    {
        return QString(m_pluginsDir + QLatin1String("lib") + type + QLatin1String("plugin.so"));
    }

    SPPluginInterface AuthPluginsPool::storePlugin(const PluginId &id, PluginProxy *proxy)
    {
        SPPluginInterface interface(proxy);
        m_plugins.insert(id, interface);
        return interface;
    }

    bool AuthPluginsPool::pluginInterfaceValid(PluginProxy *interface)
    {
        // Could use improvement at a certain point

        bool isOk = true;
        if(interface->type() == QLatin1String(""))
        {
            TRACE() << "Plugin interface invalid. No type set.";
            isOk = false;
        }
        if(interface->mechanisms().count() <= 0)
        {
            TRACE() << "Plugin interface invalid. No authentication mechanism supported.";
            isOk = false;
        }

        return isOk;
    }

    void AuthPluginsPool::serializePluginInterface(const PluginProxy *interface, QIODevice *device)
    {
        if(!interface || !device)
            return;
        //TODO
    }

} // namespace SignonDaemonNS
