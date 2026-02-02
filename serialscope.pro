QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

INCLUDEPATH += include

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/SerialHandler.cpp \
    src/RingBuffer.cpp \
    src/portconfig.cpp \
    src/graphoptions.cpp \
    src/exportdlg.cpp \
    src/cursordata.cpp \
    src/ringbuffer.cpp \
    src/qcustomplot.cpp\
    src/serialhandler.cpp

HEADERS += \
    include/mainwindow.h \
    include/qcustomplot.h\
    include/SerialHandler.h \
    include/RingBuffer.h \
    include/portconfig.h \
    include/graphoptions.h \
    include/exportdlg.h \
    include/cursordata.h \
    include/ringbuffer.h \
    include/serialhandler.h

FORMS += \
    ui/MainWindow.ui \
    ui/portconfig.ui \
    ui/graphoptions.ui \
    ui/cursordata.ui \
    ui/exportdlg.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
