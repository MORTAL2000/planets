cmake_minimum_required(VERSION 3.0)
project(Planets)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions( -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/" )
add_definitions( -DASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/" )

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libs/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(libs/assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(include/
                    src/
                    libs/assimp/include/
                    libs/glad/include/
                    libs/glfw/include/
                    libs/glm/
                    libs/stb/)

file(GLOB VENDORS_SOURCES libs/glad/src/glad.c)

# Here, we make a variable that is actually a big list of all our source files.
# Note that the file also contains the directory w/ respect to this CMakeLists.txt.
# Every time we add a new source file, remember to add it to this list before
# you compile! Doing so manually is better than recursively (i.e. with file(GLOB_RECURSE ...))
# because that can lead to frustraing build errors if you're not careful.
set(PROJECT_SOURCES
    src/main.cpp
    src/scene.cpp
    src/common/planet.cpp
    src/common/universe.cpp
    src/entities/entity.cpp
    src/entities/space_craft.cpp
    src/geometry/mesh.cpp
    src/geometry/model.cpp
    # src/geometry/skinned_mesh.cpp
    src/geometry/sphere.cpp
    src/geometry/terrain_face.cpp
	src/graphics/gl_error.cpp
	src/graphics/window_size.cpp
	src/graphics/shader.cpp
	src/graphics/texture.cpp
	src/graphics/frame_buffer.cpp
	src/graphics/camera.cpp
	src/graphics/tangent_calculator.cpp
	src/graphics/controls/flying_camera.cpp
    src/graphics/controls/planet_camera.cpp
    src/graphics/input/key_input.cpp
	src/graphics/input/mouse_input.cpp
	src/graphics/renderers/lines.cpp
	src/graphics/renderers/phong.cpp
	src/graphics/renderers/planet_renderer.cpp
	src/graphics/renderers/underwater_renderer.cpp
    src/graphics/renderable.cpp
    src/graphics/vert_attributes.cpp
	src/graphics/vert_buffer.cpp
	src/utils/generation/planet_generator.cpp
	src/utils/obb.cpp
	src/utils/resource_manager.cpp
)

file(GLOB PROJECT_HEADERS include/*.hpp)

file(GLOB PROJECT_SHADERS shaders/*.comp
                          shaders/*.frag
                          shaders/*.geom
                          shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Sources" FILES ${PROJECT_SOURCES}) 
source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})