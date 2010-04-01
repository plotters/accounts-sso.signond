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


#include "cryptomanager.h"
#include "cryptohandlers.h"
#include "signond-common.h"

#include <QFile>
#include <QDir>
#include <QMetaEnum>

#define MOUNT_DIR "/home/user/"
#define DEVICE_MAPPER_DIR "/device/mapper/"
#define EXT2 "ext2"
#define EXT3 "ext3"
#define EXT4 "ext4"

namespace SignonDaemonNS {


    CryptoManager::CryptoManager(QObject *parent)
            : QObject(parent),
              m_accessCode(QByteArray()),
              m_fileSystemPath(QString()),
              m_fileSystemMapPath(QString()),
              m_fileSystemName(QString()),
              m_loopDeviceName(QString()),
              m_fileSystemType(Ext3),
              m_fileSystemSize(4)
    {
        updateMountState(Unmounted);
    }

    CryptoManager::CryptoManager(const QByteArray &accessCode,
                                 const QString &fileSystemPath,
                                 QObject *parent)
            : QObject(parent),
              m_accessCode(accessCode),
              m_loopDeviceName(QString()),
              m_fileSystemType(Ext3),
              m_fileSystemSize(4)
    {
        setFileSystemPath(fileSystemPath);
        updateMountState(Unmounted);
    }

    CryptoManager::~CryptoManager()
    {
        unmountFileSystem();
    }

    void CryptoManager::setFileSystemPath(const QString &path)
    {
        m_fileSystemPath = path;

        //Generated based on the file system path
        if (m_fileSystemPath.contains(QDir::separator()))
            m_fileSystemName = m_fileSystemPath.section(QDir::separator(), -1);
        else
            m_fileSystemName = m_fileSystemPath;

        m_fileSystemMapPath = QLatin1String(DEVICE_MAPPER_DIR) + m_fileSystemName;
    }

    bool CryptoManager::setFileSystemSize(const quint32 size)
    {
        if (size < MINUMUM_ENCRYPTED_FILE_SYSTEM_SIZE) {
            TRACE() << "Minumum encrypted file size is 4 Mb.";
            return false;
        }
        m_fileSystemSize = size;
        return true;
    }

    bool CryptoManager::setFileSystemType(const QString &type)
    {
        QString cmpBase = type.toLower();
        if (cmpBase == QLatin1String(EXT2)) {
            m_fileSystemType = Ext2;
            return true;
        } else if (cmpBase == QLatin1String(EXT3)) {
            m_fileSystemType = Ext3;
            return true;
        } else if (cmpBase == QLatin1String(EXT4)) {
            m_fileSystemType = Ext4;
            return true;
        }
        return false;
    }

    bool CryptoManager::setupFileSystem()
    {
        if (m_mountState == Mounted) {
            TRACE() << "Ecrypyted file system already mounted.";
            return false;
        }

        m_loopDeviceName = LosetupHandler::findAvailableDevice();
        if (m_loopDeviceName.isNull()) {
            BLAME() << "No free loop device available!";
            return false;
        }

        if (!PartitionHandler::createPartitionFile(m_fileSystemPath, m_fileSystemSize)) {
            BLAME() << "Could not create partition file.";
            unmountFileSystem();
            return false;
        }

        if (!LosetupHandler::setupDevice(m_loopDeviceName,
                                         m_fileSystemPath)) {
            BLAME() << "Failed to setup loop device:" << m_loopDeviceName;
            unmountFileSystem();
            return false;
        }
        updateMountState(LoopSet);

        if (!CryptsetupHandler::formatFile(m_accessCode, m_loopDeviceName)) {
            BLAME() << "Failed to LUKS format.";
            unmountFileSystem();
            return false;
        }
        updateMountState(LoopLuksFormatted);

        if (!CryptsetupHandler::openFile(m_accessCode,
                                         m_loopDeviceName,
                                         m_fileSystemName)) {
            BLAME() << "Failed to LUKS open";
            unmountFileSystem();
            return false;
        }
        updateMountState(LoopLuksOpened);

        if (!PartitionHandler::formatPartitionFile(m_fileSystemMapPath,
                                                   m_fileSystemType)) {
            BLAME() << "Could not format mapped partition.";
            unmountFileSystem();
            return false;
        }

        if (!mountMappedDevice(m_fileSystemName)) {
            BLAME() << "Failed to mount ecrypted file system.";
            unmountFileSystem();
            return false;
        }
        updateMountState(Mounted);

        return true;
    }

    //TODO - add checking for LUKS header in case of preavious app run formatting failure
    bool CryptoManager::mountFileSystem()
    {
        if (m_mountState == Mounted) {
            TRACE() << "Ecrypyted file system already mounted.";
            return false;
        }

        m_loopDeviceName = LosetupHandler::findAvailableDevice();
        if (m_loopDeviceName.isNull()) {
            BLAME() << "No free loop device available!";
            return false;
        }

        if (!LosetupHandler::setupDevice(m_loopDeviceName, m_fileSystemPath)) {
            BLAME() << "Failed to setup loop device:" << m_loopDeviceName;
            unmountFileSystem();
            return false;
        }
        updateMountState(LoopSet);

        if (!CryptsetupHandler::openFile(m_accessCode, m_loopDeviceName, m_fileSystemName)) {
            BLAME() << "Failed to LUKS open.";
            unmountFileSystem();
            return false;
        }
        updateMountState(LoopLuksOpened);

        if (!mountMappedDevice(m_fileSystemName)) {
            TRACE() << "Failed to mount ecrypted file system.";
            unmountFileSystem();
            return false;
        }
        updateMountState(Mounted);
        return true;
    }

    bool CryptoManager::unmountFileSystem()
    {
        if (m_mountState == Unmounted) {
            TRACE() << "Ecrypyted file system not mounted.";
            return true;
        }

        bool isOk = true;

        if ((m_mountState >= Mounted)
            && !unmountMappedDevice()) {
            TRACE() << "Failed to unmount mapped loop device.";
            isOk = false;
        } else {
            TRACE() << "Mapped loop device unmounted.";
        }

        if ((m_mountState >= LoopLuksOpened)
            && (!CryptsetupHandler::closeFile(m_fileSystemName))) {
            TRACE() << "Failed to LUKS close.";
            isOk = false;
        } else {
            TRACE() << "Luks close succeeded.";
        }

        if ((m_mountState >= LoopSet)
            && (!LosetupHandler::releaseDevice(m_loopDeviceName))) {
            TRACE() << "Failed to release loop device.";
            isOk = false;
        } else {
            TRACE() << "Loop device released.";
        }

        updateMountState(Unmounted);
        return isOk;
    }

    bool CryptoManager::deleteFileSystem()
    {
        if (m_mountState > Unmounted) {
            if (!unmountFileSystem())
                return false;
        }

        //TODO - implement effective deletion in specific handler object
        return false;
    }

    //TODO - debug this method. Current test scenarios do no cover this one
    bool CryptoManager::fileSystemContainsFile(const QString &filePath)
    {
        if (m_mountState != Mounted) {
            TRACE() << "Ecrypyted file system not mounted.";
            return false;
        }

        QString filePathL = QDir::toNativeSeparators(filePath);

        if (filePathL.startsWith(QDir::separator()))
            filePathL = QLatin1String(MOUNT_DIR) + m_fileSystemPath + filePath;
        else
            filePathL = QLatin1String(MOUNT_DIR) + m_fileSystemPath + QDir::separator() + filePath;

        return QFile::exists(filePathL);
    }

    QString CryptoManager::fileSystemMountPath() const
    {
        return QLatin1String(MOUNT_DIR) + m_fileSystemName;
    }

    void CryptoManager::updateMountState(const FileSystemMountState state)
    {
        TRACE() << "Updating mount state:" << state;
        m_mountState = state;
    }

    bool CryptoManager::mountMappedDevice(const QString &mountName)
    {
        //create mnt dir if not existant
        QString mountNameAbs = QLatin1String(MOUNT_DIR) + mountName;

        if (!QFile::exists(mountNameAbs)) {
            QDir dir;
            dir.mkdir(mountNameAbs);
        }

        MountHandler::mount(m_fileSystemMapPath, mountNameAbs);
        return true;
    }

    bool CryptoManager::unmountMappedDevice()
    {
        return MountHandler::unmount(QLatin1String(MOUNT_DIR) + m_fileSystemName);
    }

    bool CryptoManager::addEncryptionKey(const QByteArray &key, const QByteArray &existingKey)
    {
        if (!CryptsetupHandler::addKeySlot(m_fileSystemPath, key, existingKey)) {
            TRACE() << "FAILED to add new key slot to the encrypted file system.";
            return false;
        }
        return true;
    }

    bool CryptoManager::removeEncryptionKey(const QByteArray &key, const QByteArray &remainingKey)
    {
        if (!CryptsetupHandler::removeKeySlot(m_fileSystemPath, key, remainingKey)) {
            TRACE() << "FAILED to add new key slot to the encrypted file system.";
            return false;
        }
        return false;
    }

    //TODO - remove this after stable version is achieved.
    void CryptoManager::serializeData()
    {
        TRACE() << "m_accessCode" << m_accessCode;
        TRACE() << "m_fileSystemPath" << m_fileSystemPath;
        TRACE() << "m_fileSystemMapPath" << m_fileSystemMapPath;
        TRACE() << "m_fileSystemName" << m_fileSystemName;
        TRACE() << "m_loopDeviceName" << m_loopDeviceName;
        TRACE() << "m_fileSystemType" << m_fileSystemType;
        TRACE() << "m_fileSystemSize" << m_fileSystemSize;
    }
} //namespace SignonDaemonNS
