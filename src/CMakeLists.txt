add_subdirectory(Engine)
add_subdirectory(Application)

set(TARGET "ecosystem")

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} PRIVATE ${DEPENDENCIES} Application)