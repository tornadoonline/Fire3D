
set(_BUILD_DIR ${Third_DIR}/glfw/build)

add_custom_target(ext_glfw 
    COMMAND ${CMAKE_COMMAND} 
        -DBUILD_SHARED_LIBS=ON
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
        -DGLFW_INSTALL=ON
        -DCMAKE_INSTALL_PREFIX=${EXT_DIR}/glfw
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO}
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL}
        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG=${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}
        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE=${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}
        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO=${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO}
        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL=${CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL} 
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL}
        ..
        COMMAND ${CMAKE_COMMAND} --build .  --target install -j 33
    WORKING_DIRECTORY ${_BUILD_DIR}
    COMMENT "Build GLFW Library."
)

add_library(Fire3D::glfw SHARED IMPORTED)
set_target_properties(Fire3D::glfw PROPERTIES 
    INTERFACE_COMPILE_DEFINITIONS "GLFW_DLL"
    IMPORTED_IMPLIB_DEBUG ${CMAKE_BINARY_DIR}/lib/glfw3dll.lib
    IMPORTED_LOCATION_DEBUG ${CMAKE_BINARY_DIR}/bin/glfw3.dll
    IMPORTED_IMPLIB_RELEASE ${CMAKE_BINARY_DIR}/lib/glfw3dll.lib
    IMPORTED_LOCATION_RELEASE ${CMAKE_BINARY_DIR}/bin/glfw3.dll
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
    IMPORTED_CONFIGURATIONS Release
)
add_dependencies(Fire3D::glfw ext_glfw)

add_custom_command(
    TARGET ext_glfw
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_BUILD_DIR}
)

set(GLFW3_ICNLUDE_DIR ${EXT_DIR}/glfw/include)
set(GLFW3_ICNLUDE_DIRS ${EXT_DIR}/glfw/include)
set(GLFW_LIBRARIES Fire3D::glfw)

unset(_BUILD_DIR)