TEMPLATE = lib
TARGET =

include( ../../common-installs-config.pri )

headers.files = \
    SignOn/authpluginif.h \
    SignOn/signonplugin.h \
    SignOn/UiSessionData \
    SignOn/uisessiondata.h \
    SignOn/uisessiondata_priv.h
headers.path = $${INSTALL_PREFIX}/include/signon-plugins/SignOn
INSTALLS += headers

pkgconfig.files = signon-plugins.pc
pkgconfig.path = $${INSTALL_PREFIX}/lib/pkgconfig
INSTALLS += pkgconfig

