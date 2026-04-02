cmake_minimum_required(VERSION 3.19)
project(WarGame LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Svg)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(WarGame
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    pages/menupage.h
    pages/menupage.cpp
    pages/menupage.ui
    config/gameconfig.h
    config/gameconfigdata.cpp
    config/gameconfigdata.h
    pages/configpage.h
    pages/configpage.cpp
    pages/configpage.ui
    config/teamComposition.h
    pages/teamconfigdialog.h
    pages/teamconfigdialog.cpp
    pages/teamconfigdialog.ui
    resources.qrc
)

target_link_libraries(WarGame
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Svg
)

include(GNUInstallDirs)

install(TARGETS WarGame
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WarGame
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
