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
  @file authpluginspool.h
  Definition of the Authentication Plugins Pool object.
  @ingroup Accounts_and_SSO_Framework
 */

#ifndef AUTHPLUGINSPOOL_H
#define AUTHPLUGINSPOOL_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QIODevice>

#include "pluginproxy.h"


#define DEFAULT_PLUGINS_DIR QLatin1String("/usr/lib/signon/")
#define ALL_TYPES 0

namespace SignonDaemonNS {

    class PluginEntity;

    /*!
        @typedef SPPluginInterface Shared pointer access to the plugin proxy interface.
    */
    typedef QSharedPointer<PluginProxy> SPPluginInterface;

    /*!
        @typedef PluginId, unique ID a pair of the plugin type and the Unique Signon Identity Id
     */
    typedef QString PluginType, UniqueId;
    typedef QPair<PluginType, UniqueId> PluginId;

    /*!
        @class AuthPluginsPool
        Manages all authentication plugins access.
        @ingroup Accounts_and_SSO_Framework
     */
    class AuthPluginsPool
    {
    public:
        /*!
            Constructs an AccessControl object
            @param pluginsDir, the physical location of the plugin files.
        */
        AuthPluginsPool(const QString &pluginsDir = DEFAULT_PLUGINS_DIR);

        /*!
            Destroys the AuthPluginsPool object, clearing all resources.
            Unloads all loaded plugins.
            @sa clear()
        */
        ~AuthPluginsPool();

        /*!
            Constructs an AccessControl object
            @param type, the type of the authentication plugin.This string is part of the plugin library file name.
        */
        SPPluginInterface loadPlugin(const PluginId &id, const QObject *challengeReplyHandler = 0, const char *challengeReplySlot = 0);

        /*!
            Unloads the plugin having a specific type.
            @param type, the type of the authentication plugin.
            @sa unloadPlugin(const SPPluginInterface &plugin)
            @retval, true upon success, false otherwise
        */
        bool unloadPlugin(const PluginId &id);

        /*!
            Unloads a specific plugin. Provided for convenience.
            @param plugin, the authentication plugin to be unloaded. The callee must clear his reference of the shared pointer.
            @sa unloadPlugin(const QString &type)
            @retval, true upon success, false otherwise
        */
        bool unloadPlugin(const SPPluginInterface &plugin);

        /*!
            @param type, the type of the desired plugin.
            @param forceLoading, forces the loading of the plugin if it was not previously loaded.
            @returns a non null plugin interface upon success.
        */
        SPPluginInterface plugin(const PluginId &id, bool forceLoading = true);

        /*!
            @returns a list of all the loaded plugins' types.
        */
        const QStringList pluginTypes();

        /*!
            @param type, limits the result to a subset of type 'type'
            @returns a list of all the loaded plugins.
        */
        const QList<SPPluginInterface> plugins(const QString &type = ALL_TYPES) const;

        /*!
            @param type, limits the result to a subset of type 'type'
            @returns the number of loaded plugins.
        */
        int pluginsCount(const QString &type = ALL_TYPES) const;

        /*!
            Clears the plugin pool of all the loaded plugins.
            @returns true, upon unloading all the plugins.
            @sa unloadPlugin()
        */
        bool clear();

        /*!
            @returns the plugin directory, the physical location of the plugin library files.
        */
        const QString &pluginsDir() const;

        /*!
            Sets the plugin directory.
            @param pluginsDir.
        */
        void setPluginsDir(const QString &pluginsDir);

        /*!
            Creates a plugin id based on a plugin type and unique id (Identity id).
            @param type.
            @param id
            @returns The plugin id.
        */
        static const PluginId createId(const PluginType &type, const quint32 id);

        /*!
            Current implementation parses file names in the set plugins' directory.
            @returns list of available plugin types.
        */
        QStringList availablePluginTypes() const;

    private:
        static bool validId(const PluginId &id);
        static void tracePluginId(const QString &message, const PluginId &id);

        QString createFileName(const QString &pluginType);
        SPPluginInterface storePlugin(const PluginId &id, PluginProxy *interface);

        bool pluginInterfaceValid(PluginProxy *interface);
        void serializePluginInterface(const PluginProxy *interface, QIODevice *device);

    private:
        QString m_pluginsDir;
        QMap<PluginId, SPPluginInterface> m_plugins;
    };
} // namespace SignonDaemonNS

#endif // AUTHPLUGINSPOOL_H
