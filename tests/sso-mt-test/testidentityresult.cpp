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

#include "testidentityresult.h"


#define IT_IS_HAPPENING qDebug() << "Reply from SIGNON DAEMON---------------------------------" << __FUNCTION__;

TestIdentityResult::TestIdentityResult()
{
    reset();
}

void TestIdentityResult::reset()
{
    m_responseReceived = Inexistent;
    m_err = Identity::UnknownError;
    m_errMsg = "";

    m_methods.clear();
    m_id = 0;
    m_idInfo = IdentityInfo();
    m_userVerified = false;
    m_secretVerified = false;
    m_signedOut = false;
    m_removed = false;

}
bool TestIdentityResult::compareIdentityInfos(
        const IdentityInfo &info1,
        const IdentityInfo &info2, bool checkId, bool checlACL)
{
    TRACE() << QString("\nComparing identities %1 & %2.\n").arg(info1.id()).arg(info2.id());

    if(checkId && (info1.id() != info2.id()))
    {
        TRACE() << "IDs:" << info1.id() << " " << info2.id();
        return false;
    }

    if(info1.caption() != info2.caption())
    {
        TRACE() << "Captions:" << info1.caption() << " " << info2.caption();
        return false;
    }

    if(info1.methods() != info2.methods())
    {
        TRACE() << "Methods:" << info1.methods() << " " << info2.methods();
        return false;
    }

    if(info1.realms() != info2.realms())
    {
        TRACE() << "Realms:" << info1.realms() << " " << info2.realms();
        return false;
    }

    if(checlACL && (info1.accessControlList() != info2.accessControlList()))
    {
        TRACE() << "ACLs:" << info1.accessControlList() << " " << info2.accessControlList();
        return false;
    }

    if(info1.userName() != info2.userName())
    {
        TRACE() << "Usernames:" << info1.userName() << " " << info2.userName();
        return false;
    }

    foreach(QString method, info1.methods())
        if(info1.mechanisms(method) != info2.mechanisms(method))
        {

            TRACE() << QString("Mechanisms for method %1:").arg(method)
                    << info1.mechanisms(method) << " " << info2.mechanisms(method);
            return false;
        }


    return true;
}

void TestIdentityResult::error(Identity::IdentityError code, const QString& message)
{
    IT_IS_HAPPENING
    m_responseReceived = Error;
    m_err = code;
    m_errMsg = message;

    qDebug() << "Error:" << m_err << ", Message:" << m_errMsg;

    emit testCompleted();
}

void TestIdentityResult::methodsAvailable(const QStringList& methods)
{
    IT_IS_HAPPENING
    m_responseReceived = Normal;
    m_methods = methods;

    emit testCompleted();
}

void TestIdentityResult::credentialsStored(const quint32 id)
{
    IT_IS_HAPPENING
    m_responseReceived = Normal;
    m_id = id;

    emit testCompleted();
}

void TestIdentityResult::info(const IdentityInfo &info)
{
    IT_IS_HAPPENING
    m_responseReceived = Normal;
    m_idInfo = info;

    emit testCompleted();
}

void TestIdentityResult::userVerified(const bool valid)
{
    IT_IS_HAPPENING
    m_responseReceived = Normal;
    m_userVerified = valid;

    emit testCompleted();
}

void TestIdentityResult::secretVerified(const bool valid)
{
    IT_IS_HAPPENING
    m_responseReceived = Normal;
    m_secretVerified = valid;

    emit testCompleted();
}

void TestIdentityResult::removed()
{
    IT_IS_HAPPENING
    m_responseReceived = Normal;
    m_removed = true;

    emit testCompleted();
}

void TestIdentityResult::signedOut()
{
    IT_IS_HAPPENING
    m_responseReceived = Normal;
    m_signedOut = true;

    emit testCompleted();
}
