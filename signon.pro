include( common-vars.pri )
include( common-project-config.pri )

TEMPLATE  = subdirs
CONFIG   += ordered
SUBDIRS   = lib src server tests

include( common-installs-config.pri )
include( doc/doc.pri )

exists(/etc/aegisfs.conf) {
    
    PKGCONFIG += aegisfs
    DEFINES += SIGNON_AEGISFS
    
# Install AEGIS_fond file
	aegisfs_config.files = user-sso.conf
	aegisfs_config.path = /etc/aegisfs.d

	INSTALLS += aegisfs_config
}

# End of File
