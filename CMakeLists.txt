cmake_minimum_required(VERSION 3.16)
if(WIN32 AND "${CMAKE_GENERATOR_PLATFORM}" STREQUAL "")
    set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "Platform (x64 matches bundled SDL)" FORCE)
endif()

project(crimsonshooter)

if(WIN32 AND MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR
        "Can only build x64 on Windows. Delete build dir")
endif()

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

# Embed everything under assets/ directly into the binary
set(EMBED_SCRIPT ${CMAKE_SOURCE_DIR}/cmake/embed_assets.cmake)
set(GENERATED_ASSETS ${CMAKE_BINARY_DIR}/generated/embedded_assets.cpp)
file(GLOB_RECURSE ASSET_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/assets/*)
add_custom_command(
    OUTPUT ${GENERATED_ASSETS}
    COMMAND ${CMAKE_COMMAND}
        -DASSET_DIR=${CMAKE_SOURCE_DIR}/assets
        -DPROJECT_ROOT=${CMAKE_SOURCE_DIR}
        -DOUTPUT_CPP=${GENERATED_ASSETS}
        -P ${EMBED_SCRIPT}
    DEPENDS ${ASSET_FILES} ${EMBED_SCRIPT}
    COMMENT "Embedding assets -> ${GENERATED_ASSETS}"
    VERBATIM
)
target_sources(${PROJECT_NAME} PRIVATE ${GENERATED_ASSETS})

# clang-tidy integration
option(ENABLE_CLANG_TIDY "Enable clang-tidy analysis" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
        )
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    else()
        message(WARNING "clang-tidy not found!")
    endif()
endif()

# Custom target for running clang-tidy manually
if(CLANG_TIDY_EXE)
    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY_EXE}
        --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        --header-filter=.*
        ${SOURCES}
        --
        -std=c++17
        -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_SOURCE_DIR}/libs/SDL2/include
        -I${CMAKE_SOURCE_DIR}/libs/SDL2_image/include
        -I${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/include
        -I${CMAKE_SOURCE_DIR}/libs/SDL2_ttf/include
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on source files"
    )
endif()

# Link SDL2 libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2_image/lib/x64/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/SDL2_mixer.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2_ttf/lib/x64/SDL2_ttf.lib
        shell32
    )
    
    # Set console subsystem for Windows
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
    
    # Copy DLLs alongside executable
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_image/lib/x64/libpng16-16.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libpng16-16.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_image/lib/x64/SDL2_image.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_image.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_image/lib/x64/zlib1.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/zlib1.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/SDL2_mixer.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_mixer.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/libogg-0.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libogg-0.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/libvorbis-0.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libvorbis-0.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/libvorbisfile-3.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libvorbisfile-3.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_ttf/lib/x64/libfreetype-6.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libfreetype-6.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/libs/SDL2_ttf/lib/x64/SDL2_ttf.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_ttf.dll
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

    find_library(MATH_LIBRARY m)
    if(MATH_LIBRARY)
        target_link_libraries(${PROJECT_NAME} ${MATH_LIBRARY})
    endif()
    
    target_compile_options(${PROJECT_NAME} PRIVATE
        ${SDL2_CFLAGS_OTHER}
        ${SDL2_IMAGE_CFLAGS_OTHER}
        ${SDL2_MIXER_CFLAGS_OTHER}
        ${SDL2_TTF_CFLAGS_OTHER}
    )
endif()