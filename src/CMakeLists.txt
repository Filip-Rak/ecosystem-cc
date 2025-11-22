add_subdirectory(Engine)
add_subdirectory(Application)

set(TARGET "ecosystem")

add_executable(${TARGET} ${ENGINE_SOURCES} ${APP_SOURCES} main.cpp)
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET} PRIVATE ${DEPENDENCIES})