TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR=$$PWD/dist

SOURCES += \
        src/game.cpp \
        src/main.cpp


INCLUDEPATH += SDL2/include/

OTHER_FILES += README.md

SDLdll.files +=  \
    $$PWD/SDL2/lib/x64/SDL2.dll \
    $$PWD/SDL2/lib/x64/SDL2.dll
    $$PWD/SDL2/lib/x64/SDL2.dll
SDLdll.path = $$DESTDIR
INSTALLS += SDLdll

LIBS += $$PWD/SDL2/lib/x64/SDL2.lib
LIBS += $$PWD/SDL2/lib/x64/SDL2main.lib
LIBS += -lshell32

QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:CONSOLE
QMAKE_POST_LINK += $$quote(jom.exe install$$escape_expand(\n\t))

HEADERS += \
    src/game.h
