cmake_minimum_required(VERSION 3.13)

# 1. Název projektu MUSÍ 1:1 odpovídat poli "id" v mod.json
project(randomdudeidk555.audio_cue_trainer VERSION 1.0.0)

# 2. Vyhledání Geode SDK
find_package(Geode REQUIRED)

# 3. Načtení všech zdrojových C++ souborů ve složce src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

# 4. Vytvoření knihovny (targetu) se stejným názvem jako ID módu
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# 5. Nastavení Geode módu (propojí knihovnu s Geode SDK a mod.json)
setup_geode_mod(${PROJECT_NAME})
