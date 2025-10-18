include(FetchContent)

FetchContent_Declare(
    entt
    GIT_REPOSITORY git@github.com:skypjack/entt.git
    GIT_TAG v3.15.0
)
FetchContent_MakeAvailable(entt)

FetchContent_Declare(
    glm
    GIT_REPOSITORY git@github.com:g-truc/glm.git
    GIT_TAG 1.0.1 
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY git@github.com:SFML/SFML.git
    GIT_TAG 3.0.1
)
FetchContent_MakeAvailable(sfml)

FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY git@github.com:Neargye/magic_enum.git
    GIT_TAG v0.9.7 
)
FetchContent_MakeAvailable(magic_enum)

set(DEPENDENCIES
    EnTT::EnTT
    glm::glm
    sfml-system 
    sfml-window 
    sfml-graphics
    magic_enum::magic_enum
)