/*
 * This file is part of signon
 *
 * Copyright (C) 2009-2010 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation.
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

#include "identityinfoimpl.h"
#include "identityinfo.h"
#include "signond/signoncommon.h"

#include <QDBusMetaType>
#include <QVariant>
#include <QVariantMap>

namespace SignOn {

IdentityInfoImpl::IdentityInfoImpl(IdentityInfo *identityInfo):
    m_identityInfo(identityInfo),
    m_id(0),
    m_userName(QString()),
    m_secret(QString()),
    m_storeSecret(false),
    m_caption(QString()),
    m_authMethods(QMap<MethodName, MechanismsList>()),
    m_realms(QStringList()),
    m_accessControlList(SecurityContextList()),
    m_owner(SecurityContext()),
    m_type(IdentityInfo::Other),
    m_refCount(0),
    m_isEmpty(true)
{
    qDBusRegisterMetaType<SignOn::MethodMap>();
    qDBusRegisterMetaType<SignOn::SecurityList>();
}

IdentityInfoImpl::~IdentityInfoImpl()
{
}

void IdentityInfoImpl::addMethod(const MethodName &method,
                                 const MechanismsList &mechanismsList)
{
    m_authMethods.insert(method, mechanismsList);
}

void IdentityInfoImpl::updateMethod(const MethodName &method,
                                    const MechanismsList &mechanismsList)
{
    m_authMethods.remove(method);
    m_authMethods.insert(method, mechanismsList);
}

void IdentityInfoImpl::removeMethod(const MethodName &method)
{
    m_authMethods.remove(method);
}

bool IdentityInfoImpl::hasMethod(const MethodName &method) const
{
    return m_authMethods.contains(method);
}

void IdentityInfoImpl::setType(IdentityInfo::CredentialsType type)
{
    m_type = type;
}

IdentityInfo::CredentialsType IdentityInfoImpl::type() const
{
    return m_type;
}

void IdentityInfoImpl::setRefCount(qint32 refCount)
{
    m_refCount = refCount;
}

qint32 IdentityInfoImpl::refCount() const
{
    return m_refCount;
}

bool IdentityInfoImpl::isEmpty() const
{
    return m_isEmpty;
}

void IdentityInfoImpl::copy(const IdentityInfoImpl &other)
{
    m_id = other.m_id;
    m_userName = other.m_userName;
    m_secret = other.m_secret;
    m_storeSecret = other.m_storeSecret;
    m_caption =  other.m_caption;
    m_authMethods = other.m_authMethods;
    m_realms = other.m_realms;
    m_accessControlList = other.m_accessControlList;
    m_owner = other.m_owner;
    m_type = other.m_type;
    m_refCount = other.m_refCount;
    m_isEmpty = other.m_isEmpty;
}

void IdentityInfoImpl::clear()
{
    m_id = 0;
    m_userName = QString();
    m_secret = QString();
    m_storeSecret = false;
    m_caption =  QString();
    m_authMethods.clear();
    m_realms.clear();
    m_accessControlList.clear();
    m_owner = SecurityContext();
    m_type = IdentityInfo::Other;
    m_refCount = 0;
    m_isEmpty = true;
}

QVariantMap IdentityInfoImpl::toMap() const
{
    QVariantMap values;
    values.insert(SIGNOND_IDENTITY_INFO_ID, m_id);
    values.insert(SIGNOND_IDENTITY_INFO_USERNAME, m_userName);
    values.insert(SIGNOND_IDENTITY_INFO_SECRET, m_secret);
    values.insert(SIGNOND_IDENTITY_INFO_STORESECRET, m_storeSecret);
    values.insert(SIGNOND_IDENTITY_INFO_CAPTION, m_caption);
    values.insert(SIGNOND_IDENTITY_INFO_AUTHMETHODS,
                  QVariant::fromValue(m_authMethods));
    values.insert(SIGNOND_IDENTITY_INFO_REALMS, m_realms);

    values.insert(SIGNOND_IDENTITY_INFO_ACL,
                  QVariant::fromValue(m_accessControlList.toSecurityList()));
    values.insert(SIGNOND_IDENTITY_INFO_OWNER, m_owner.toStringList());

    values.insert(SIGNOND_IDENTITY_INFO_TYPE, m_type);
    values.insert(SIGNOND_IDENTITY_INFO_REFCOUNT, m_refCount);
    return values;
}

void IdentityInfoImpl::updateFromMap(const QVariantMap &map)
{
    if (map.contains(SIGNOND_IDENTITY_INFO_ID))
        m_id = map.value(SIGNOND_IDENTITY_INFO_ID).toUInt();

    if (map.contains(SIGNOND_IDENTITY_INFO_USERNAME))
        m_userName = map.value(SIGNOND_IDENTITY_INFO_USERNAME).toString();

    if (map.contains(SIGNOND_IDENTITY_INFO_SECRET))
        m_secret = map.value(SIGNOND_IDENTITY_INFO_SECRET).toString();

    if (map.contains(SIGNOND_IDENTITY_INFO_CAPTION))
        m_caption = map.value(SIGNOND_IDENTITY_INFO_CAPTION).toString();

    if (map.contains(SIGNOND_IDENTITY_INFO_AUTHMETHODS))
        m_authMethods =
            qdbus_cast<MethodMap>(map.value(SIGNOND_IDENTITY_INFO_AUTHMETHODS));

    if (map.contains(SIGNOND_IDENTITY_INFO_ACL))
        m_accessControlList =
            qdbus_cast<SecurityList>(map.value(SIGNOND_IDENTITY_INFO_ACL));
    if (map.contains(SIGNOND_IDENTITY_INFO_OWNER))
        m_owner = map.value(SIGNOND_IDENTITY_INFO_OWNER).toStringList();

    if (map.contains(SIGNOND_IDENTITY_INFO_REALMS))
        m_realms = map.value(SIGNOND_IDENTITY_INFO_REALMS).toStringList();

    if (map.contains(SIGNOND_IDENTITY_INFO_TYPE))
        m_type = IdentityInfo::CredentialsType(
            map.value(SIGNOND_IDENTITY_INFO_TYPE).toInt());

    if (map.contains(SIGNOND_IDENTITY_INFO_REFCOUNT))
        m_refCount = map.value(SIGNOND_IDENTITY_INFO_REFCOUNT).toInt();
}

} //namespace SignOn
