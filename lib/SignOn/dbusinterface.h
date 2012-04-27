/*
 * This file is part of signon
 *
 * Copyright (C) 2011 Nokia Corporation.
 *
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

#ifndef SIGNON_DBUSINTERFACE_H
#define SIGNON_DBUSINTERFACE_H

#include <QDBusAbstractInterface>

namespace SignOn {

/* This is a version of QDBusInterface which doesn't do the blocking
 * introspection.
 */
class DBusInterface: public QDBusAbstractInterface
{
public:
    DBusInterface(const QString &service,
                  const QString &path,
                  const char *interface,
                  const QDBusConnection &connection,
                  QObject *parent = 0);
    virtual ~DBusInterface();

    bool connect(const char *name, QObject *receiver, const char *slot);
};

}

#endif // SIGNON_DBUSINTERFACE_H

