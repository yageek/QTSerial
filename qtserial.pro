# -------------------------------------------------
# Project created by QtCreator 2010-04-23T11:30:08
# -------------------------------------------------
TARGET = qtserial
TEMPLATE = app
SOURCES += main.cpp \
    win_qextserialport.cpp \
    serialtool.cpp \
    readserial.cpp \
    qextserialport.cpp \
    qextserialbase.cpp \
    posix_qextserialport.cpp
HEADERS += win_qextserialport.h \
    ui_serialtool.h \
    serialtool.h \
    readserial.h \
    qextserialport.h \
    qextserialbase.h \
    posix_qextserialport.h
FORMS += serialtool.ui
unix:DEFINES = _TTY_POSIX_
win32:DEFINES = _TTY_WIN_
