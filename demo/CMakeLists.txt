
add_executable(Demo demo.cpp)

# Link with the Firecrest library
target_link_libraries(Demo PRIVATE Firecrest)

# Resource path definition for the demo
target_compile_definitions(Demo PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")

# Use the same C++ standard as the library
set_property(TARGET Demo PROPERTY CXX_STANDARD 20)
set_property(TARGET Demo PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET Demo PROPERTY CXX_EXTENSIONS OFF)
