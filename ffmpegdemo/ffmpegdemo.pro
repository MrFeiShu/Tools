QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ctransmanager.cpp \
    cvideotrans.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ctransmanager.h \
    cvideotrans.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    ffmpegdemo_yue_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += $$PWD/include

LIBS += $$PWD/lib/avcodec.lib  \
        $$PWD/lib/avdevice.lib  \
    $$PWD/lib/avfilter.lib  \
    $$PWD/lib/avformat.lib  \
    $$PWD/lib/avutil.lib  \
    $$PWD/lib/postproc.lib  \
    $$PWD/lib/swresample.lib  \
    $$PWD/lib/swscale.lib  \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
