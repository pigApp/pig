TEMPLATE = app
TARGET = pig 

#CONFIG += link_pkgconfig
#PKGCONFIG += libtorrent-rasterbar
#DEFINES += BOOST_ASIO_DYN_LINK

QT += qml quick sql network multimedia multimediawidgets

SOURCES += src/main.cpp\
	   src/pig.cpp\
	   src/password.cpp\
	   src/update.cpp\
	   src/tcpSocket.cpp\
           #src/torrent.cpp\
	   src/videoplayer.cpp

HEADERS += src/pig.h\
	   src/password.h\
	   src/update.h\
	   src/tcpSocket.h\
           #src/torrent.h\
	   src/videoplayer.h\
           /usr/include/libtorrent/entry.hpp\
           /usr/include/libtorrent/bencode.hpp\
           /usr/include/libtorrent/session.hpp\
           /usr/include/libtorrent/torrent_info.hpp\
           /usr/include/libtorrent/torrent_handle.hpp\
           lib\libtorrent-rasterbar-1.0.1\include\entry.hpp\
           lib\libtorrent-rasterbar-1.0.1\include\bencode.hpp\
           lib\libtorrent-rasterbar-1.0.1\include\session.hpp\
           lib\libtorrent-rasterbar-1.0.1\include\torrent_info.hpp\
           lib\libtorrent-rasterbar-1.0.1\include\torrent_handle.hpp

OTHER_FILES += src/qml/main.qml\
	       src/qml/AskPassword.qml\
	       src/qml/Update.qml\
	       src/qml/Finder.qml\
	       src/qml/Output.qml\
	       src/qml/SetPassword.qml\
	       src/qml/Filters.qml\
               src/qml/Selectors.qml\
	       src/qml/Wait.qml\
	       src/qml/Button.qml\
	       src/qml/ButtonFilter.qml\
	       src/qml/ButtonScene.qml\
	       src/qml/ProgressBar.qml\
	       src/qml/News.qml\
	       src/qml/Help.qml\
	       src/qml/ErrorDb.qml

RC_FILE += images/pig/icon.rc

RESOURCES += qml.qrc

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


