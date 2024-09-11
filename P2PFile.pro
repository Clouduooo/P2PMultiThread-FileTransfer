QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filereceiver.cpp \
    filesender.cpp \
    main.cpp \
    mainwindow.cpp \
    client.cpp \
    server.cpp

HEADERS += \
    filereceiver.h \
    filesender.h \
    mainwindow.h \
    client.h \
    server.h

FORMS += \
    mainwindow.ui \
    client.ui \
    server.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
