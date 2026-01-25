include(FetchContent)

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.15.0
)
FetchContent_MakeAvailable(entt)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1 
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
)
FetchContent_MakeAvailable(sfml)

FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.7 
)
FetchContent_MakeAvailable(magic_enum)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.5.0
)
FetchContent_MakeAvailable(CLI11)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.1
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)

FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0
)
FetchContent_MakeAvailable(imgui-sfml)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG f1c79c02822848a9bed4315b12c8c8f3761e1296
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0 
)
FetchContent_MakeAvailable(json)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

set(DEPENDENCIES
    CLI11::CLI11
    EnTT::EnTT
    glm::glm
    ImGui-SFML::ImGui-SFML
    magic_enum::magic_enum
    nlohmann_json::nlohmann_json
    sfml-graphics
    sfml-system 
    sfml-window 
    stb 
)