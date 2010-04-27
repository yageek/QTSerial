# -------------------------------------------------
# Project created by QtCreator 2010-04-23T11:30:08
# -------------------------------------------------
TARGET = qtserial
TEMPLATE = app
SOURCES += main.cpp \
    serialtool.cpp \
    readserial.cpp
HEADERS += ui_serialtool.h \
    serialtool.h \
    readserial.h
FORMS += serialtool.ui
unix:LIBS += -lqextserialport
win32:LIBS+= libqextserialport.dll
unix:DEFINES = _TTY_POSIX_
win32:DEFINES = _TTY_WIN_
RESOURCES += icons.qrc
