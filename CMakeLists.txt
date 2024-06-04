cmake_minimum_required(VERSION 3.5)

project(MetaMover VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Concurrent)

set(TS_FILES MetaMover_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        metamovermainwindow.cpp
        metamovermainwindow.h
        metamovermainwindow.ui
        exif.cpp    # Added exif source file
        exif.h      # Added exif header file
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MetaMover
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        appconfig.h
        appconfigmanager.h appconfigmanager.cpp
        resources.qrc
        basicfilehandler.h basicfilehandler.cpp
        photofilehandler.h photofilehandler.cpp
        scanner.h scanner.cpp
        videofilehandler.h videofilehandler.cpp
        ifilehandler.h
        filehandlerfactory.h
        transfermanager.h transfermanager.cpp
        directorytransfer.h directorytransfer.cpp
        appicon.rc
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MetaMover APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(MetaMover SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MetaMover
            ${PROJECT_SOURCES}
            appicon.rc
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(MetaMover PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MetaMover)
endif()
set_target_properties(MetaMover PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MetaMover
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MetaMover)
endif()


# CPack configuration
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "MetaMover")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Blake Azuela")
set(CPACK_PACKAGE_CONTACT "Blake Azuela <blake@azuela.info>")
set(CPACK_PACKAGE_VERSION "0.1.1")
set(CPACK_PACKAGE_DESCRIPTION "MetaMover Application")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MetaMover aims to organize cluttered DCIM folders, SD cards and cell phones. Essentially, it uses image EXIF data to create a folder structure such as year, month, date and camera model. Your images are then organized into the said structure. It handles duplicates really well and can place images that don't contain EXIF data into a separate folder.")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6, libqt6concurrent6, libqt6network6, libstdc++6, libgcc1, libc6, zlib1g, libgl1-mesa-glx, libglib2.0-0, libsm6, libxi6, libxrender1, libxrandr2, libfreetype6, libfontconfig1")
include(CPack)

install(TARGETS MetaMover DESTINATION /usr/local/bin)
