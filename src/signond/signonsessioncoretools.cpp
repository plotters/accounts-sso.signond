/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2011 Nokia Corporation.
 *
 * Contact: Aurel Popirtac <ext-aurel.popirtac@nokia.com>
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

#include "signonsessioncoretools.h"

#include <QDebug>
#include "signond-common.h"

using namespace SignonDaemonNS;

QVariantMap SignonDaemonNS::mergeVariantMaps(const QVariantMap &map1,
                                             const QVariantMap &map2)
{
    if (map1.isEmpty()) return map2;
    if (map2.isEmpty()) return map1;

    QVariantMap map = map1;
    //map2 values will overwrite map1 values for the same keys.
    QMapIterator<QString, QVariant> it(map2);
    while (it.hasNext()) {
        it.next();
        if (map.contains(it.key()))
            map.remove(it.key());
    }
    return map.unite(map2);
}

/* --------------------- StoreOperation ---------------------- */

StoreOperation::StoreOperation(const StoreType type):
    m_storeType(type)
{
}

StoreOperation::StoreOperation(const StoreOperation &src):
    m_storeType(src.m_storeType),
    m_info(src.m_info),
    m_authMethod(src.m_authMethod),
    m_blobData(src.m_blobData)
{
}

StoreOperation::~StoreOperation()
{
}

/* --------------------- RequestData ---------------------- */

RequestData::RequestData(const QDBusConnection &conn,
                         const QDBusMessage &msg,
                         const QVariantMap &params,
                         const QString &mechanism,
                         const QString &cancelKey):
    m_conn(conn),
    m_msg(msg),
    m_params(params),
    m_mechanism(mechanism),
    m_cancelKey(cancelKey)
{
}

RequestData::RequestData(const RequestData &other):
    m_conn(other.m_conn),
    m_msg(other.m_msg),
    m_params(other.m_params),
    m_mechanism(other.m_mechanism),
    m_cancelKey(other.m_cancelKey)
{
}

RequestData::~RequestData()
{
}

/* --------------------- AuthCoreCache ---------------------- */

AuthCoreCache *AuthCoreCache::m_instance = 0;

AuthCoreCache::AuthCache::AuthCache()
{
}

AuthCoreCache::AuthCache::~AuthCache()
{
}

bool AuthCoreCache::AuthCache::isEmpty() const
{
    return (m_password.isEmpty() && m_blobData.isEmpty());
}

AuthCoreCache::AuthCoreCache(QObject *parent):
    QObject(parent)
{
}

AuthCoreCache::~AuthCoreCache()
{
    clear();
    m_instance = 0;
}

AuthCoreCache *AuthCoreCache::instance(QObject *parent)
{
    if (m_instance == 0)
        m_instance = new AuthCoreCache(parent);

    return m_instance;
}

AuthCache *AuthCoreCache::data(const IdentityId id) const
{
    return m_cache.value(id, 0);
}

void AuthCoreCache::insert(const CacheId &id, AuthCache *cache)
{
    if (cache == 0) return;

    AuthCache *data = m_cache.take(id.first);

    if ((data != 0) && data->isEmpty()) {
        delete data;
        data = 0;
    }

    if (data == 0) {
        m_cache.insert(id.first, cache);
        m_cachingSessionsMethods[id.first] = AuthMethods() << id.second;
    } else {
        if (cache->m_username.isEmpty())
            cache->m_username = data->m_username;
        if (cache->m_password.isEmpty())
            cache->m_password = data->m_password;

        cache->m_blobData =
            mergeVariantMaps(data->m_blobData, cache->m_blobData);

        delete data;
        m_cache.insert(id.first, cache);

        AuthMethods cachingSessionsMethods = m_cachingSessionsMethods[id.first];
        if (!cachingSessionsMethods.contains(id.second))
            cachingSessionsMethods.append(id.second);
    }
}

void AuthCoreCache::authSessionDestroyed(const CacheId &id)
{
    AuthCache *data = m_cache.value(id.first, 0);
    if (data != 0) {
        AuthMethods authMethods = m_cachingSessionsMethods[id.first];
        authMethods.removeOne(id.second);
        if (authMethods.isEmpty()) {
            delete m_cache.take(id.first);
            (void)m_cachingSessionsMethods.take(id.first);
        }
    }
}

void AuthCoreCache::clear()
{
    QList<IdentityId> keys = m_cache.keys();
    foreach (IdentityId key, keys)
        delete m_cache.take(key);

    m_cachingSessionsMethods.clear();
}
