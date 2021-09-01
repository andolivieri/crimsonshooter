TEMPLATE = app
CONFIG += c++17
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR=$$PWD/dist

HEADERS += \
    src/assetmanager.h \
    src/collision.h \
    src/ecs.h \
    src/ecs/aicomponent.h \
    src/ecs/animation.h \
    src/ecs/bloodpatch.h \
    src/ecs/collidercomponent.h \
    src/ecs/components.h \
    src/ecs/damagemodel.h \
    src/ecs/decaycomponent.h \
    src/ecs/dumbcomponent.h \
    src/ecs/foespawnercomponent.h \
    src/ecs/inputcomponent.h \
    src/ecs/predicatecomponent.h \
    src/ecs/projectilecomponent.h \
    src/ecs/relationshipcomponent.h \
    src/ecs/scorecollector.h \
    src/ecs/soundcomponent.h \
    src/ecs/spritecomponent.h \
    src/ecs/textcomponent.h \
    src/ecs/tilecomponent.h \
    src/ecs/timercomponent.h \
    src/ecs/transformcomponent.h \
    src/ecs/weaponcomponent.h \
    src/game.h \
    src/gamemap.h \
    src/math2d.h \
    src/scenes/level0.h \
    src/scenes/scenes.h \
    src/texturemanager.h \
    src/utils.h \
    src/vector2d.h

SOURCES += \
        src/assetmanager.cpp \
        src/collision.cpp \
        src/ecs.cpp \
        src/ecs/aicomponent.cpp \
        src/ecs/animation.cpp \
        src/ecs/bloodpatch.cpp \
        src/ecs/collidercomponent.cpp \
        src/ecs/damagemodel.cpp \
        src/ecs/decaycomponent.cpp \
        src/ecs/dumbcomponent.cpp \
        src/ecs/foespawnercomponent.cpp \
        src/ecs/inputcomponent.cpp \
        src/ecs/predicatecomponent.cpp \
        src/ecs/projectilecomponent.cpp \
        src/ecs/relationshipcomponent.cpp \
        src/ecs/scorecollector.cpp \
        src/ecs/soundcomponent.cpp \
        src/ecs/spritecomponent.cpp \
        src/ecs/textcomponent.cpp \
        src/ecs/tilecomponent.cpp \
        src/ecs/timercomponent.cpp \
        src/ecs/transformcomponent.cpp \
        src/ecs/weaponcomponent.cpp \
        src/game.cpp \
        src/gamemap.cpp \
        src/main.cpp \
        src/math2d.cpp \
        src/texturemanager.cpp \
        src/utils.cpp \
        src/vector2d.cpp


INCLUDEPATH += libs/SDL2/include/
INCLUDEPATH += libs/SDL2_image/include/
INCLUDEPATH += libs/SDL2_mixer/include/
INCLUDEPATH += libs/SDL2_ttf/include/
INCLUDEPATH += src/

OTHER_FILES += README.md

SDLdll.files +=  \
    $$PWD/libs/SDL2/lib/x64/SDL2.dll \
    $$PWD/libs/SDL2_image/lib/x64/libjpeg-9.dll \
    $$PWD/libs/SDL2_image/lib/x64/libpng16-16.dll\
    $$PWD/libs/SDL2_image/lib/x64/libtiff-5.dll\
    $$PWD/libs/SDL2_image/lib/x64/libwebp-7.dll\
    $$PWD/libs/SDL2_image/lib/x64/SDL2_image.dll\
    $$PWD/libs/SDL2_image/lib/x64/zlib1.dll \
    #$$PWD/libs/SDL2_mixer/lib/x64/libFLAC-8.dll \
    #$$PWD/libs/SDL2_mixer/lib/x64/libmodplug-1.dll \
    #$$PWD/libs/SDL2_mixer/lib/x64/libmpg123-0.dll \
    #$$PWD/libs/SDL2_mixer/lib/x64/libogg-0.dll \
    #$$PWD/libs/SDL2_mixer/lib/x64/libopus-0.dll \
    #$$PWD/libs/SDL2_mixer/lib/x64/libopusfile-0.dll \
    #$$PWD/libs/SDL2_mixer/lib/x64/libvorbis-0.dll \
    #$$PWD/libs/SDL2_mixer/lib/x64/libvorbisfile-3.dll \
    $$PWD/libs/SDL2_mixer/lib/x64/SDL2_mixer.dll \
    $$PWD/libs/SDL2_ttf/lib/x64/libfreetype-6.dll \
    $$PWD/libs/SDL2_ttf/lib/x64/SDL2_ttf.dll

SDLdll.path = $$DESTDIR
INSTALLS += SDLdll

assets.path  = $${DESTDIR}
assets.commands  += \
        $$quote(if not exist $$system_path($$DESTDIR\assets\ ) mkdir $$system_path($$DESTDIR\assets\ ) $$escape_expand(\n\t))
assets.commands  += \
        $$quote(xcopy /Y /S $$system_path($$PWD\assets\* ) $$system_path($$DESTDIR\assets\ ) $$escape_expand(\n\t))
INSTALLS += assets

LIBS += $$PWD/libs/SDL2/lib/x64/SDL2.lib
LIBS += $$PWD/libs/SDL2/lib/x64/SDL2main.lib
LIBS += $$PWD/libs/SDL2_image/lib/x64/SDL2_image.lib
LIBS += $$PWD/libs/SDL2_mixer/lib/x64/SDL2_mixer.lib
LIBS += $$PWD/libs/SDL2_ttf/lib/x64/SDL2_ttf.lib
LIBS += -lshell32

QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:CONSOLE
QMAKE_POST_LINK += $$quote(jom.exe install$$escape_expand(\n\t))

