TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR=$$PWD/dist

HEADERS += \
    src/ecs.h \
    src/ecs/aicomponent.h \
    src/ecs/components.h \
    src/ecs/spritecomponent.h \
    src/ecs/transformcomponent.h \
    src/game.h \
    src/gamemap.h \
    src/gameobject.h \
    src/texturemanager.h \
    src/vector2d.h

SOURCES += \
        src/ecs.cpp \
        src/ecs/aicomponent.cpp \
        src/ecs/spritecomponent.cpp \
        src/ecs/transformcomponent.cpp \
        src/game.cpp \
        src/gamemap.cpp \
        src/gameobject.cpp \
        src/main.cpp \
        src/texturemanager.cpp \
        src/vector2d.cpp


INCLUDEPATH += libs/SDL2/include/
INCLUDEPATH += libs/SDL2_image/include/
INCLUDEPATH += src/

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
        $$quote(mkdir $$system_path($$DESTDIR\assets\ ) $$escape_expand(\n\t))
assets.commands  += \
        $$quote(xcopy /Y /S $$system_path($$PWD\assets\* ) $$system_path($$DESTDIR\assets\ ) $$escape_expand(\n\t))
INSTALLS += assets

LIBS += $$PWD/libs/SDL2/lib/x64/SDL2.lib
LIBS += $$PWD/libs/SDL2/lib/x64/SDL2main.lib
LIBS += $$PWD/libs/SDL2_image/lib/x64/SDL2_image.lib
LIBS += -lshell32

QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:CONSOLE
QMAKE_POST_LINK += $$quote(jom.exe install$$escape_expand(\n\t))

