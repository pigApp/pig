TEMPLATE = app
TARGET = PIG

QT += qml quick sql multimediawidgets

SOURCES += src/main.cpp\
	         src/pig.cpp\
	         src/password/pass.cpp\
	         src/update/update.cpp\
	         src/video/videoplayer.cpp


HEADERS += src/pig.h\
	         src/password/pass.h\
	         src/update/update.h\
	         src/video/videoplayer.h

OTHER_FILES += src/qml/main.qml\
	             src/qml/core/AskPassword.qml\
	             src/qml/core/Update.qml\
               src/qml/core/Finder.qml\
               src/qml/core/Output.qml\
	             src/qml/core/SetPassword.qml\
	             src/qml/core/Filters.qml\
               src/qml/core/PreviewPlayer.qml\
	             src/qml/core/Button.qml\
	             src/qml/core/ButtonFilter.qml\
	             src/qml/core/ButtonScenne.qml\
	             src/qml/core/WaitMsg.qml\
	             src/qml/core/ErrorDbMsg.qml

RC_FILE += images/icon.rc

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


