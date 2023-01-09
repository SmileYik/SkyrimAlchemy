cmake_minimum_required(VERSION 3.0)
project(SkyrimAlchemy)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

set(skyrimalchemy_SRC
  src/main.cpp
  src/skyrimalchemy.cpp
  src/alchemy_effect.cpp
  src/alchemy_material.cpp
  src/alchemy_recipe.cpp
  src/material_manager.cpp
  src/alchemy_material_model.cpp
  src/alchemy_effect_model.cpp
  src/alchemy_recipe_model.cpp
)

include_directories(
  build/skyrimalchemy_autogen/include
)

# Tell CMake to create the helloworld executable
add_executable(skyrimalchemy ${skyrimalchemy_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(skyrimalchemy Qt5::Widgets)

# Install the executable
install(TARGETS skyrimalchemy DESTINATION bin)
