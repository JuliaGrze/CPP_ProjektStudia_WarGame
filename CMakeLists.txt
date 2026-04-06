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
    pages/battlepage.h pages/battlepage.cpp pages/battlepage.ui
    application/models/gamestate.h application/models/gamestate.cpp
    application/models/board.h application/models/board.cpp
    application/models/tile.h application/models/tile.cpp
    application/models/team.h application/models/team.cpp
    application/models/unit.h application/models/unit.cpp
    application/models/infantry.h application/models/infantry.cpp
    application/models/tank.h application/models/tank.cpp
    application/models/artillery.h application/models/artillery.cpp
    application/models/medic.h application/models/medic.cpp
    application/models/enums/terraintype.h
    application/models/enums/maptype.h
    application/models/enums/teamside.h
    application/helpers/unitplacementhelper.h application/helpers/unitplacementhelper.cpp
    application/services/gamesetupservice.h application/services/gamesetupservice.cpp
    application/services/gameconfigvalidationservice.h application/services/gameconfigvalidationservice.cpp
    application/services/battleboardservice.h application/services/battleboardservice.cpp

    application/controllers/gamecontroller.h application/controllers/gamecontroller.cpp
    application/engine/battleengine.h application/engine/battleengine.cpp
    application/models/interfaces/IUnit.h
    application/models/player.h application/models/player.cpp
    application/controllers/interfaces/Iplayercontroller.h
    application/controllers/humancontroller.h application/controllers/humancontroller.cpp
    application/controllers/aicontroller.h application/controllers/aicontroller.cpp
    application/engine/iattackstrategy.h
    application/engine/defaultattackstrategy.h
    application/engine/defaultattackstrategy.cpp
    application/engine/attackresolver.h application/engine/attackresolver.cpp
    pages/rulespage.h pages/rulespage.cpp pages/rulespage.ui
    application/helpers/battleuiformatter.h application/helpers/battleuiformatter.cpp

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
