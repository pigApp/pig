TARGET = pig
TEMPLATE = app

CONFIG += link_pkgconfig#w
PKGCONFIG += libtorrent-rasterbar#w
DEFINES += BOOST_ASIO_DYN_LINK#w

QT += qml quick sql network multimediawidgets

SOURCES += src/main.cpp\
	   src/pig.cpp\
	   src/password.cpp\
	   src/update.cpp\
	   src/tcpSocket.cpp\
           src/torrent.cpp\#w
           src/videoplayer.cpp

HEADERS += src/pig.h\
	   src/password.h\
	   src/update.h\
	   src/tcpSocket.h\
           src/torrent.h\#w
	   src/videoplayer.h\
           /usr/include/libtorrent/entry.hpp\
           /usr/include/libtorrent/bencode.hpp\
           /usr/include/libtorrent/session.hpp\
           /usr/include/libtorrent/torrent_info.hpp\
           /usr/include/libtorrent/torrent_handle.hpp\

RESOURCES += resources.qrc

RC_FILE += resources/images/pig/icon.rc

# CUSTOM PATH SHARED LIBS
unix {
#QMAKE_HOST.arch = x86                                     #TODO: Determinar arquitectura.
    #...
#QMAKE_HOST.arch = x86_64
    QMAKE_LFLAGS += -Wl,--rpath=/usr/lib64/pig             #comentar
}
win32 {
    QMAKE_LFLAGS += -Wl,--rpath=C:/pig/lib                 #comentar
}

unix {                                                     #TODO: Cambiar el usuario de .pig y .pig/db.sqlite.
    target.path = /usr/bin/
    config.path = $$(HOME)/.pig
    config.files = .pig/*
    #config.extra = mkdir $$(HOME)/.pig -m 744 -p && chown $$(USER).users $$(HOME)/.pig -R
    INSTALLS += target config
}
win32 {
    target.path = C:/pig/
    config.path = C:/pig/.pig
    config.files += .pig/*
    update.path = C:/pig/bin/
    update.files = src/update_WIN32/bin/update.exe
    INSTALLS += target config update
    #QMAKE_INSTALL_PROGRAM = install -m 755 -p -o $$(USER) -g users
}


