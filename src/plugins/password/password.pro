include( ../../../common-project-config.pri )
include( ../../../common-vars.pri )

TEMPLATE = lib
TARGET = passwordplugin
DESTDIR = ../../lib/signon
QT += core
QT -= gui

CONFIG += plugin \
    debug_and_release \
    build_all \
    warn_on \
    link_pkgconfig

HEADERS += passwordplugin.h

SOURCES += passwordplugin.cpp

INCLUDEPATH += . \
    ../ \
    $$TOP_SRC_DIR/lib/plugins

QMAKE_CXXFLAGS += -fno-exceptions \
    -fno-rtti

headers.files = $$HEADERS
include( ../../../common-installs-config.pri )
target.path = $${INSTALL_PREFIX}/lib/signon
INSTALLS = target
headers.path = $${INSTALL_PREFIX}/include/signon-plugins
INSTALLS += headers
