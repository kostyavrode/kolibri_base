cmake_minimum_required(VERSION 3.16)

project(FileXorProcessor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/fileprocessor.h
    src/fileprocessor.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FileXorProcessor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(FileXorProcessor
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(FileXorProcessor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(FileXorProcessor PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FileXorProcessor)
endif()

