QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithms.cpp \
    draw.cpp \
    edge.cpp \
    main.cpp \
    mainform.cpp \
    qpoint3df.cpp \
    settings2.cpp \
    sortpointsbyx.cpp \
    sortpointsbyy.cpp \
    triangle.cpp

HEADERS += \
    algorithms.h \
    draw.h \
    edge.h \
    mainform.h \
    qpoint3df.h \
    settings2.h \
    sortpointsbyx.h \
    sortpointsbyy.h \
    triangle.h

FORMS += \
    mainform.ui \
    settings2.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
