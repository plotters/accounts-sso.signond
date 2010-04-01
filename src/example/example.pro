include( ../../common-project-config.pri )
include( ../../common-vars.pri )

TEMPLATE = app
TARGET = signonclient
DEPENDPATH += .
INCLUDEPATH += . \
    $$TOP_SRC_DIR/src/plugins/example

CONFIG += uitools \
    debug \
    link_pkgconfig
CONFIG += qdbus

# Input
FORMS += signonclient.ui
HEADERS += signonclient.h \
    $$TOP_SRC_DIR/src/plugins/example/exampledata.h
SOURCES += main.cpp \
    signonclient.cpp

#LIBS += -L../libsignon-qt -lsignon-qt
LIBS += $${TOP_BUILD_DIR}/lib/SignOn/libsignon-qt.a

# install
include( ../../common-installs-config.pri )
example.path = $${INSTALL_PREFIX}/share/doc/libsignon-qt-dev/example
example.files = $${FORMS} $${HEADERS} $${SOURCES}
INSTALLS += example
