include( ../../../common-project-config.pri )
include( ../../../common-vars.pri )

TEMPLATE = lib
TARGET = saslplugin
DESTDIR = ../../lib/signon
QT += core
QT -= gui

CONFIG += plugin \
        debug_and_release \
        build_all \
        warn_on \
        link_pkgconfig

HEADERS += saslplugin.h \
        sasldata.h

SOURCES += saslplugin.cpp

INCLUDEPATH += . \
    ../ \
    $$TOP_SRC_DIR/lib/plugins

QMAKE_CXXFLAGS += -fno-exceptions \
    -fno-rtti

PKGCONFIG += sasl2
LIBS += -lsasl2

QMAKE_CLEAN += libsasl.so
headers.files = $$HEADERS
include( ../../../common-installs-config.pri )
target.path = $${INSTALL_PREFIX}/lib/signon
INSTALLS = target
headers.path = $${INSTALL_PREFIX}/include/signon-plugins
INSTALLS += headers
pkgconfig.files = signon-saslplugin.pc
pkgconfig.path = $${INSTALL_PREFIX}/lib/pkgconfig
INSTALLS += pkgconfig
