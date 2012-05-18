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
#include <QPair>
#include <QList>
#include <QVariant>

namespace SignOn {

typedef QList<QVariant> SecurityList;

/*!
 * Security context descriptor.
 */
class SecurityContext : public QPair<QString, QString>
{
public:
    SecurityContext() :
        QPair<QString, QString>()
        { }
    SecurityContext(const QLatin1String &simple)
        { first = simple; }
    SecurityContext(const QString &simple)
        { first = simple; }
    SecurityContext(const QString &sysCtx, const QString &appCtx)
        {
            first = sysCtx;
            second = appCtx;
        }
    SecurityContext(const QStringList &list)
        { *this = list; }
    SecurityContext(const SecurityContext &src) :
        QPair<QString, QString>(src.first, src.second)
        { }
    operator const QString & () const
        { return first; }
    QStringList toStringList() const
        {
            QStringList list;
            list.append(first);
            list.append(second);
            return list;
        }
    SecurityContext & operator=(const SecurityContext &src)
        {
            QPair<QString, QString>::operator=(src);
            return (*this);
        }
    SecurityContext & operator=(const QStringList &list)
        {
            first = list.value(0);
            second = list.value(1);
            return (*this);
        }
    bool operator<(const SecurityContext &other)
        {
            if (first == other.first)
                return (second < other.second);
            return (first < other.first);
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
    operator QStringList () const
        {
            QStringList simpleACL;
            foreach (SecurityContext aclEntry, *this)
                simpleACL.append(aclEntry.first);
            return simpleACL;
        }
    SecurityList toSecurityList () const
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
    void sort ()
        { qSort(begin(), end()); }
};

}

Q_DECLARE_METATYPE(SignOn::SecurityList)

#endif  // SECURITYCONTEXT_H

