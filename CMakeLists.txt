cmake_minimum_required(VERSION 3.5)

project(tscUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)



set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)


set(MAIN_FILES
    TSCInclude

    include/types
    include/includeQt
    include/hs.h

    include/mainwindow.hpp
    include/ffmpegworker.hpp
    include/worker.hpp
    include/threadMgr.hpp

    src/mainwindow.cpp

    src/ffmpegworker.cpp
    src/worker.cpp
    src/threadMgr.cpp

    src/types.cpp


    res/assets.qrc
    res/res.rc

    widgets/include/tabWig.hpp
    widgets/src/tabWig.cpp

    widgets/include/debugWindow.hpp
    widgets/src/debugWindow.cpp
)

qt6_wrap_ui(ui_FILES
    src/mainwindow.ui
    widgets/src/debugWindow.ui
)

set(WEDGETS_FILES
    widgets/textbrowser.hpp widgets/textbrowser.cpp
)

set(TS_FILES
    ./lang/tscUI_zh_CN.ts
)

set(PROJECT_SOURCES
        WIN32 main.cpp
        ${MAIN_FILES}
        ${ui_FILES}
        ${WEDGETS_FILES}
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(tscUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(tscUI SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(tscUI
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(tscUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(tscUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(tscUI)
endif()

target_include_directories(tscUI PRIVATE "${CMAKE_BINARY_DIR}/tscUI_autogen/include_debug")
