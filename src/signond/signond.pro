include( ../../common-project-config.pri )
include( ../../common-vars.pri )
TEMPLATE = app
TARGET = signond
QT += core \
    sql \
    xml \
    network \
    dbus
QT -= gui

#generate adaptor for backup
system(qdbusxml2cpp -c BackupIfAdaptor -a backupifadaptor.h:backupifadaptor.cpp \
    ../../lib/signond/com.nokia.SingleSignOn.Backup.xml)

HEADERS += \
    accesscontrolmanager.h \
    credentialsaccessmanager.h \
    credentialsdb.h \
    default-crypto-manager.h \
    default-key-authorizer.h \
    signonsessioncore.h \
    signonauthsessionadaptor.h \
    signonauthsession.h \
    signonidentity.h \
    signond-common.h \
    signondaemonadaptor.h \
    signondaemon.h \
    signondisposable.h \
    signontrace.h \
    pluginproxy.h \
    signonidentityinfo.h \
    signonui_interface.h \
    signonidentityadaptor.h \
    backupifadaptor.h \
    signonsessioncoretools.h
SOURCES += \
    accesscontrolmanager.cpp \
    credentialsaccessmanager.cpp \
    credentialsdb.cpp \
    default-crypto-manager.cpp \
    default-key-authorizer.cpp \
    signonsessioncore.cpp \
    signonauthsessionadaptor.cpp \
    signonauthsession.cpp \
    signonidentity.cpp \
    signondaemonadaptor.cpp \
    signondisposable.cpp \
    signonui_interface.cpp \
    pluginproxy.cpp \
    main.cpp \
    signondaemon.cpp \
    signonidentityinfo.cpp \
    signonidentityadaptor.cpp \
    backupifadaptor.cpp \
    signonsessioncoretools.cpp
INCLUDEPATH += . \
    $${TOP_SRC_DIR}/lib/plugins \
    $${TOP_SRC_DIR}/lib/plugins/signon-plugins-common \
    $${TOP_SRC_DIR}/lib/signond \
    $${TOP_SRC_DIR}/lib/sim-dlc

CONFIG += build_all \
    link_pkgconfig

PKGCONFIG += \
    signon-plugins-common

QMAKE_LIBDIR += \
    $${TOP_BUILD_DIR}/lib/plugins/signon-plugins-common \
    $${TOP_BUILD_DIR}/lib/signond/SignOn

QMAKE_CXXFLAGS += -fno-exceptions \
    -fno-rtti
DEFINES += QT_NO_CAST_TO_ASCII \
    QT_NO_CAST_FROM_ASCII
DEFINES += "SIGNOND_PLUGINS_DIR=$${SIGNOND_PLUGINS_DIR_QUOTED}"
DEFINES += "SIGNOND_EXTENSIONS_DIR=$${SIGNOND_EXTENSIONS_DIR_QUOTED}"

#Trace defines can be overruled by signond's configuration file `LoggingLevel`
DEFINES += SIGNOND_TRACE
LIBS += \
    -lsignon-plugins-common \
    -lsignon-extension

QMAKE_CLEAN += backupifadaptor.cpp \
               backupifadaptor.h

exists(/usr/include/sys/creds.h) {
    DEFINES += HAVE_LIBCREDS=1
    LIBS += -lcreds
} else {
    DEFINES += HAVE_LIBCREDS=0
    DEFINES += SIGNON_DISABLE_ACCESS_CONTROL
}

headers.files = $$HEADERS
include( ../../common-installs-config.pri )

# Disabling access control if platform is not arm
BUILD_ARCH = $$QMAKE_HOST.arch
contains(BUILD_ARCH, i686):DEFINES += SIGNON_DISABLE_ACCESS_CONTROL

OTHER_FILES += \
    signond.conf \
    setupstorage.sh

conf_file.files = $$OTHER_FILES
conf_file.path = /etc/

INSTALLS += conf_file
