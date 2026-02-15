cmake_minimum_required(VERSION 4.0)
project(dxx_dlc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

add_definitions(-DDEBUG_OUTPUT)
add_definitions(-DLUA_BUILD_AS_STATIC)

add_compile_options(-Wa,-mbig-obj)

file(GLOB_RECURSE MINHOOK_MODULE "${CMAKE_SOURCE_DIR}/external/minhook/src/*.c")

add_library(dxx_dlc SHARED main.cpp ${MINHOOK_MODULE}
        src/debug/debug_output.cpp
)

target_sources(dxx_dlc PUBLIC
        FILE_SET module_files TYPE CXX_MODULES FILES
        src/application.ixx
        src/directx/dx_swap_chain.ixx
        src/service/service_container.ixx
        src/hook/hook_container.ixx
        src/bootstrap/bootstrap.ixx
        src/service/service_locator.ixx
        src/bootstrap/bootstrap_sdk.ixx
        src/bootstrap/bootstrap_hook.ixx
        src/hook/impl/hook_impl_present.ixx
        src/hook/impl/hook_impl_resize_buffers.ixx
        src/hook/hook_dispatcher.ixx
)

target_compile_options(dxx_dlc PRIVATE -mcx16)

target_include_directories(dxx_dlc PUBLIC
        "external/"
        "src/"
        "."
        "external/minhook/include/"
        "external/libguarded/src/"
)

target_link_libraries(dxx_dlc PRIVATE d3d11)
