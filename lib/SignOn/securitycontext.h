/*
 * This file is part of signon
 *
 * Copyright (C) 2012 Intel Corporation.
 *
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
/*!
 * @copyright Copyright (C) 2012 Intel Corporation.
 * @license LGPL
 */

#ifndef SECURITYCONTEXT_H
#define SECURITYCONTEXT_H

#include <QString>
#include <QStringList>
#include <QLatin1String>
#include <QList>
#include <QVariant>
#include <QSet>
#include <QDebug>

namespace SignOn {

typedef QList<QVariant> SecurityList;

/*!
 * Security context descriptor.
 */
class SecurityContext
{
public:
    /*!
     * System context, such as SMACK-label, MSSF token or just binary path.
     */
    QString sysCtx;
    /*!
     * Application context. Specifies security context within client process.
     * Such as a script or a web page.
     */
    QString appCtx;

    SecurityContext()
        { }
    SecurityContext(const QLatin1String &simple)
        {
            sysCtx = simple;
            appCtx = QLatin1String("*");
        }
    SecurityContext(const QString &simple)
        {
            sysCtx = simple;
            appCtx = QLatin1String("*");
        }
    SecurityContext(const QString &sysCtxP, const QString &appCtxP)
        {
            sysCtx = sysCtxP;
            appCtx = appCtxP;
        }
    SecurityContext(const QStringList &list)
        { *this = list; }
    SecurityContext(const SecurityContext &src)
        { *this = src; }
    operator const QString & () const
        { return sysCtx; }
    QStringList toStringList() const
        {
            QStringList list;
            list.append(sysCtx);
            list.append(appCtx);
            return list;
        }
    SecurityContext & operator=(const SecurityContext &src)
        {
            sysCtx = src.sysCtx;
            appCtx = src.appCtx;
            return (*this);
        }
    SecurityContext & operator=(const QStringList &list)
        {
            sysCtx = list.value(0);
            appCtx = list.value(1);
            return (*this);
        }
    bool operator<(const SecurityContext &other) const
        {
            if (sysCtx == other.sysCtx)
                return (appCtx < other.appCtx);
            return (sysCtx < other.appCtx);
        }
    bool operator==(const SecurityContext &other) const
        {
            if (sysCtx != other.sysCtx)
                return false;
            if (appCtx.isEmpty() && other.appCtx.isEmpty())
                return true;
            return (appCtx == other.appCtx);
        }
    bool match(const SecurityContext &other) const
        {
            if (sysCtx == QString::fromLatin1("*"))
                return true;
            if (sysCtx != other.sysCtx)
                return false;
            if (appCtx == QString::fromLatin1("*"))
                return true;
            return (appCtx == other.appCtx);
        }
};

/*!
 * List of security context descriptors.
 */
class SecurityContextList : public QList<SecurityContext>
{
public:
    SecurityContextList() :
        QList<SecurityContext>()
        { }
    SecurityContextList(const QStringList &simpleACL)
        {
            foreach (QString aclEntry, simpleACL)
                append(SecurityContext(aclEntry));
        }
    SecurityContextList(const SecurityList &list)
        { *this = list; }
    SecurityContextList(const SecurityContext &ctx)
        { *this = ctx; }
    SecurityContextList(const SecurityContextList &src) :
        QList<SecurityContext>(src)
        { }
    operator QStringList() const
        {
            QStringList simpleACL;
            foreach (SecurityContext aclEntry, *this)
                simpleACL.append(aclEntry.sysCtx);
            return simpleACL;
        }
    SecurityList toSecurityList() const
        {
            SecurityList list;
            foreach (SecurityContext aclEntry, *this)
                list.append(QVariant::fromValue(aclEntry.toStringList()));
            return list;
        }
    SecurityContextList & operator=(const SecurityContextList &src)
        {
            QList<SecurityContext>::operator=(src);
            return (*this);
        }
    SecurityContextList & operator=(const SecurityList &list)
        {
            foreach (QVariant listEntry, list)
                append(SecurityContext(listEntry.toStringList()));
            return (*this);
        }
    SecurityContextList & operator=(const SecurityContext &ctx)
        {
            clear();
            append(ctx);
            return (*this);
        }
    bool operator==(const SecurityContextList &list) const
        {
            QSet<SecurityContext> thisSet(
                QSet<SecurityContext>::fromList(*this));
            QSet<SecurityContext> otherSet(
                QSet<SecurityContext>::fromList(list));
            return thisSet.contains(otherSet);
        }
    void sort()
        { qSort(begin(), end()); }
};

}

Q_DECLARE_METATYPE(SignOn::SecurityList)

#endif  // SECURITYCONTEXT_H

