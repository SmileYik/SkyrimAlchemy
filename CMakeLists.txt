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

include_directories(
  build/skyrimalchemy_autogen/include
)

set(
  skyrimalchemy_RCCS
  resources/resources.qrc
)

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

QT5_ADD_RESOURCES(resources_RCC_SRCS ${skyrimalchemy_RCCS})

# Tell CMake to create the helloworld executable
add_executable(
  skyrimalchemy
  ${skyrimalchemy_SRC}
  ${resources_RCC_SRCS}
)

# Use the Widgets module from Qt 5.
target_link_libraries(skyrimalchemy Qt5::Widgets)

# Install the executable
install(TARGETS skyrimalchemy DESTINATION bin)
