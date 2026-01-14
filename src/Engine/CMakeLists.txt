set(ENGINE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Service/GUIService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Service/InputService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Service/SFRenderService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Service/SFWindowService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Service/TimeService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utility/TimeUtils.cpp
)

add_library(Engine ${ENGINE_SOURCES})
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(Engine PUBLIC ${DEPENDENCIES})