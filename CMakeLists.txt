cmake_minimum_required(VERSION 3.24)

project(TimeIsMoneySerial_CMake VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/felix/Qt/6.5.0/gcc_64")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(IsLibrary ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets SerialPort)

if (IsLibrary)
set(PROJECT_SOURCES
        serialoptions.cpp
        serialoptions.hpp
        serialoptions.ui
        serialreader_qt.cpp
        serialreader_qt.hpp
)
else ()
set(PROJECT_SOURCES
        main.cpp
        serialoptions.cpp
        serialoptions.hpp
        serialoptions.ui
        serialreader_qt.cpp
        serialreader_qt.hpp
)
endif ()

if (IsLibrary)
add_library(TimeIsMoneySerial_CMake
        SHARED
        ${PROJECT_SOURCES}
)
else ()
    add_executable(TimeIsMoneySerial_CMake ${PROJECT_SOURCES})
endif ()

target_link_libraries(TimeIsMoneySerial_CMake
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::SerialPort
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TimeIsMoneySerial_CMake)
endif()

set_target_properties(TimeIsMoneySerial_CMake PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TimeIsMoneySerial_CMake
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TimeIsMoneySerial_CMake)
endif()
