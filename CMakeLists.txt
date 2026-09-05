cmake_minimum_required(VERSION 3.13)

# 1. Název projektu odpovídající id v mod.json
project(randomdudeidk555.audio_cue_trainer VERSION 1.0.0)

# 2. Vyhledání zdrojových souborů
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

# 3. Vytvoření sdílené knihovny
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# 4. Propojení s Geode SDK (tento příkaz se postará o načtení SDK)
setup_geode_mod(${PROJECT_NAME})
