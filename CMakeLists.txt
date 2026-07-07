cmake_minimum_required(VERSION 3.20)

project(Auto_telescopio)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar a SFML
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
find_package(cpr REQUIRED)

# Criar o executável
add_executable(Auto_telescopio
    src/main.cpp
    src/application.cpp
    src/Telescopio.cpp
    src/Astrodata.cpp
    
)
target_include_directories(Auto_telescopio
    PRIVATE
        include
)

# Linkar as bibliotecas
target_link_libraries(Auto_telescopio
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        cpr::cpr
)