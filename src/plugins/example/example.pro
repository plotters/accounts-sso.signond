include( ../../../common-project-config.pri )
include( ../../../common-vars.pri )

TEMPLATE = lib
TARGET = exampleplugin
DESTDIR = ../../lib/signon
QT += core
QT -= gui

CONFIG += plugin \
    debug_and_release \
    build_all \
    warn_on \
    link_pkgconfig

HEADERS += exampleplugin.h \
    exampledata.h

SOURCES += exampleplugin.cpp

INCLUDEPATH += . \
    ../ \
    $$TOP_SRC_DIR/lib/plugins

QMAKE_CXXFLAGS += -fno-exceptions \
    -fno-rtti

QMAKE_CLEAN += libexample.so
headers.files = $$HEADERS
include( ../../../common-installs-config.pri )
target.path = $${INSTALL_PREFIX}/lib/signon
INSTALLS = target
headers.path = $${INSTALL_PREFIX}/include/signon-plugins
INSTALLS += headers
example.path = $${INSTALL_PREFIX}/share/doc/signon-plugins-dev/example
example.files = exampleplugin.h \
    exampleplugin.cpp \
    exampleplugin.pro \
    exampledata.h
INSTALLS += example
