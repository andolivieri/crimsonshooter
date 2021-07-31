TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR=$$PWD/dist

HEADERS += \
    src/game.h

SOURCES += \
        src/game.cpp \
        src/main.cpp


INCLUDEPATH += libs/SDL2/include/
INCLUDEPATH += libs/SDL2_image/include/

OTHER_FILES += README.md

SDLdll.files +=  \
    $$PWD/libs/SDL2/lib/x64/SDL2.dll \
    $$PWD/libs/SDL2_image/lib/x64/libjpeg-9.dll \
    $$PWD/libs/SDL2_image/lib/x64/libpng16-16.dll\
    $$PWD/libs/SDL2_image/lib/x64/libtiff-5.dll\
    $$PWD/libs/SDL2_image/lib/x64/libwebp-7.dll\
    $$PWD/libs/SDL2_image/lib/x64/SDL2_image.dll\
    $$PWD/libs/SDL2_image/lib/x64/zlib1.dll
SDLdll.path = $$DESTDIR
INSTALLS += SDLdll

assets.path  = $${DESTDIR}
assets.commands  += \
        $$quote(xcopy /Y /S $$system_path($$PWD\assets\ ) $$system_path($$DESTDIR\assets\ ) $$escape_expand(\n\t))
INSTALLS += assets

LIBS += $$PWD/libs/SDL2/lib/x64/SDL2.lib
LIBS += $$PWD/libs/SDL2/lib/x64/SDL2main.lib
LIBS += $$PWD/libs/SDL2_image/lib/x64/SDL2_image.lib
LIBS += -lshell32

QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:CONSOLE
QMAKE_POST_LINK += $$quote(jom.exe install$$escape_expand(\n\t))

