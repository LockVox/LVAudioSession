QT -= gui

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += MAKE_TEST_LIB

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ccodec.cpp \
    chardwaresound.cpp \
    crtpcomp.cpp \
    lvaudiosession.cpp \
    main.cpp

HEADERS += \
    ccodec.h \
    chardwaresound.h \
    config.h \
    crtpcomp.h \
    lvaudiosession.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/lib/ -ljrtplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/lib/ -ljrtplibd
else:unix:!macx: LIBS += -L$$PWD/libs/lib/ -ljrtplib

INCLUDEPATH += $$PWD/libs/header/jrtplib
DEPENDPATH += $$PWD/libs/header/jrtplib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/lib/ -ljthread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/lib/ -ljthreadd
else:unix:!macx: LIBS += -L$$PWD/libs/lib/ -ljthread

INCLUDEPATH += $$PWD/libs/header/jthread
DEPENDPATH += $$PWD/libs/header/jthread

unix:!macx|win32: LIBS += -L$$PWD/libs/lib/ -lopus

INCLUDEPATH += $$PWD/libs/header/opus
DEPENDPATH += $$PWD/libs/lib/opus

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/lib/ -lportaudio_x64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/lib/ -lportaudio_x64d
else:unix:!macx: LIBS += -L$$PWD/libs/lib/ -lportaudio_x64

INCLUDEPATH += $$PWD/libs/header/portaudio
DEPENDPATH += $$PWD/libs/header/portaudio
