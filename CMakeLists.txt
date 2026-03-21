cmake_minimum_required(VERSION 3.13)
project("Firecrest")

# === Gather Firecrest sources === 
file(GLOB_RECURSE FIRECREST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# === Create the Firecrest library ===
add_library(Firecrest STATIC ${FIRECREST_SOURCES})

# Include directories
target_include_directories(Firecrest PUBLIC
    src
    libraries/glm
    libraries/FixedIndexVector
    libraries/stb_image
    libraries/glad/include
)

# Dependencies
add_subdirectory(libraries/glfw)
add_subdirectory(libraries/glm)
add_subdirectory(libraries/glad/)

set(MSDF_ATLAS_USE_SKIA OFF)
add_subdirectory(libraries/msdf-atlas-gen)

find_package(OpenGL REQUIRED)
target_link_libraries(Firecrest PUBLIC glfw OpenGL::GL glad msdf-atlas-gen)

# Definitions and standard
target_compile_definitions(Firecrest PUBLIC GLM_ENABLE_EXPERIMENTAL)
set_property(TARGET Firecrest PROPERTY CXX_STANDARD 20)

# === Demo Executable ===
add_subdirectory(demo)
