/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of signon
 *
 * Copyright (C) 2010 Nokia Corporation.
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

#ifndef CRYPTSETUP_PLUGIN_H
#define CRYPTSETUP_PLUGIN_H

#include <QObject>
#include <SignOn/ExtensionInterface>

class CryptsetupPlugin: public QObject, public SignOn::ExtensionInterface3
{
    Q_OBJECT
    Q_INTERFACES(SignOn::ExtensionInterface3)

public:
    CryptsetupPlugin();

    // reimplemented methods
    SignOn::AbstractCryptoManager *cryptoManager(QObject *parent = 0) const;
    SignOn::AbstractKeyAuthorizer *keyAuthorizer(SignOn::KeyHandler *keyHandler,
                                                 QObject *parent = 0) const;
    SignOn::AbstractKeyManager *keyManager(QObject *parent = 0) const;
};

#endif // CRYPTSETUP_PLUGIN_H

