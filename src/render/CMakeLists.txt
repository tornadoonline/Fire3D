cmake_minimum_required(VERSION 3.12)
project(render)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH ${Vulkan_DIR})
find_package(Vulkan REQUIRED)

file(GLOB render_inc_source *.h *.hpp)
file(GLOB render_cpp_source *.c *.cpp)

add_library(librender ${render_inc_source} ${render_cpp_source})

include_directories(${Vulkan_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} render.cxx)

target_link_libraries(${PROJECT_NAME} librender  ${GLFW_LIBRARIES} ${Vulkan_LIBRARIES})

set(SPIRV_EXEC ${Vulkan_DIR}/Bin/glslangValidator)
# generate .spv files
add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${SPIRV_EXEC} -V ${CMAKE_SOURCE_DIR}/res/shader/normal.vert -o ${CMAKE_SOURCE_DIR}/res/shader/normal.vert.spv
    COMMAND ${SPIRV_EXEC} -V ${CMAKE_SOURCE_DIR}/res/shader/normal.frag -o ${CMAKE_SOURCE_DIR}/res/shader/normal.frag.spv
)