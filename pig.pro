TEMPLATE = app
TARGET = pig 

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

unix {
    config.files = .pig/db.sqlite
    config.path  = $$(HOME)/.pig
    config.extra = mkdir $$(HOME)/.pig -m 744 -p && chown $$(USER).users $$(HOME)/.pig -R
    update.files = bin/PXupdate
    update.path  = /usr/bin/
    target.path  = /usr/bin/
    INSTALLS += target update config
    QMAKE_INSTALL_PROGRAM = install -m 755 -p -o $$(USER) -g users
}
win32 {

}


