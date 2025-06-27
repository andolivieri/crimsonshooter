cmake_minimum_required(VERSION 3.16)
project(crimsonshooter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

# Include directories
include_directories(
    libs/SDL2/include
    libs/SDL2_image/include
    libs/SDL2_mixer/include
    libs/SDL2_ttf/include
    src
)

# Automatically find all source and header files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link SDL2 libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x86/SDL2.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x86/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2_image/lib/x86/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x86/SDL2_mixer.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2_ttf/lib/x86/SDL2_ttf.lib
        shell32
    )
    
    # Set console subsystem for Windows
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
    
    # Copy DLLs alongside executable
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x86/SDL2.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_image/lib/x86/libpng16-16.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libpng16-16.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_image/lib/x86/SDL2_image.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_image.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_image/lib/x86/zlib1.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/zlib1.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x86/SDL2_mixer.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_mixer.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x86/libogg-0.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libogg-0.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x86/libvorbis-0.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libvorbis-0.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x86/libvorbisfile-3.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libvorbisfile-3.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_ttf/lib/x86/libfreetype-6.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libfreetype-6.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_ttf/lib/x86/SDL2_ttf.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_ttf.dll
    )
    
    # Copy assets directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
else()
    # For Linux/Mac, use pkg-config to find SDL2
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    
    target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )
    
    target_compile_options(${PROJECT_NAME} PRIVATE
        ${SDL2_CFLAGS_OTHER}
        ${SDL2_IMAGE_CFLAGS_OTHER}
        ${SDL2_MIXER_CFLAGS_OTHER}
        ${SDL2_TTF_CFLAGS_OTHER}
    )
    
    # Copy assets directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()