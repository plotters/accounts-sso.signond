include( ../../../common-project-config.pri )
include( ../../../common-vars.pri )

TEMPLATE = lib
TARGET = ssotest2plugin
DESTDIR = ../../lib/signon
QT += core

CONFIG += plugin \
        debug_and_release \
        build_all \
        warn_on \
        link_pkgconfig \
        thread

HEADERS += ssotest2plugin.h \
           ssotest2data.h

SOURCES += ssotest2plugin.cpp

INCLUDEPATH += . \
    $$TOP_SRC_DIR/lib/plugins

QMAKE_CXXFLAGS += -fno-exceptions \
    -fno-rtti

headers.files = $$HEADERS
include( ../../../common-installs-config.pri )
target.path  = $${INSTALL_PREFIX}/lib/signon

captcha_images.path = $${INSTALL_PREFIX}/lib/signon/captcha_images
captcha_images.files = Captcha1.jpg \
                       Captcha2.jpg \
                       Captcha3.jpg \
                       Captcha4.jpg

INSTALLS = target captcha_images

