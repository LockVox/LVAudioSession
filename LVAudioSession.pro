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



unix:!macx: LIBS += -ljthread
unix:!macx: LIBS += -ljrtp
unix:!macx: LIBS += -lemip
unix:!macx: LIBS += -lopus
unix:!macx: LIBS += -lportaudio


INCLUDEPATH+= /usr/local/include/ \
              /usr/local/include/emiplib \
              /usr/local/include/jrtplib3 \
              /usr/local/include/jthread \
              /usr/include/opus
