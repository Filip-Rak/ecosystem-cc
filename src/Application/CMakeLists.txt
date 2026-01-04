set(APP_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CLI/ParseCli.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContextEntity/Cell.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContextEntity/Grid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContextEntity/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContextEntity/Randomizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/CameraMovementSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/InputSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/RenderSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/AgentAdaptationSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/AgentDecisionSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/AgentFeedingSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/AgentMovementSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/AgentOffspringSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/AgentPassingSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/CellBiomassSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/CLILoggerSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/SimLogSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Sim/SimRunnerSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/UISystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/MenuBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/SidePanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/StatusBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utility/AgentHelpers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utility/ReadGrid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utility/ReadPreset.cpp
)

add_library(Application ${APP_SOURCES})
target_include_directories(Application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(Application PUBLIC Engine)