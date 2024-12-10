QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    beziercurve.cpp \
    camera.cpp \
    light.cpp \
    lightinputwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    polygonmodel.cpp \
    render.cpp \
    rotationviewer.cpp \
    viewerwindow.cpp

HEADERS += \
    beziercurve.h \
    camera.h \
    light.h \
    lightinputwindow.h \
    mainwindow.h \
    polygonmodel.h \
    render.h \
    rotationviewer.h \
    viewerwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
