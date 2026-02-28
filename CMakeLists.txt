cmake_minimum_required(VERSION 4.0)
project(dxx_dlc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
set(ANGEL_SCRIPT_CMAKE "${CMAKE_CURRENT_SOURCE_DIR}/external/angelscript/angelscript/projects/cmake/")
set(MEM_SCANNER_CMAKE "${CMAKE_CURRENT_SOURCE_DIR}/external/mem_scanner/")

add_definitions(-DDEBUG_OUTPUT)

add_compile_options(-Wa,-mbig-obj)

add_subdirectory(${ANGEL_SCRIPT_CMAKE} angelscript_build)
add_subdirectory(${MEM_SCANNER_CMAKE} mem_scanner_build)

file(GLOB_RECURSE ANGEL_SCRIPT_ADDONS "${CMAKE_CURRENT_SOURCE_DIR}/external/angelscript/add_on/*.cpp")
file(GLOB_RECURSE MINHOOK_MODULE "${CMAKE_SOURCE_DIR}/external/minhook/src/*.c")

add_library(dxx_dlc SHARED main.cpp ${MINHOOK_MODULE} ${ANGEL_SCRIPT_ADDONS}
        src/debug/debug_output.cpp
        #IMGUI
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/backends/imgui_impl_dx11.cpp
        external/imgui/backends/imgui_impl_win32.cpp
        #IMGUI END
        #XXHASH
        external/xxhash/xxhash.c
        #XXHASH END
        #LZ4
        external/lz4/lib/lz4.c
        #LZ4 END
        #S3TC
        external/s3tc/s3tc.cpp
        #S3TC END
        #DirectX Math
        external/dx_xtk/Inc/SimpleMath.inl
        external/dx_math/Inc/DirectXMathMatrix.inl
        src/angelscript/binding/as_binding_gui.ixx
        #DirectX Math END
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
        src/hook/hook_dispatcher.ixx
        src/renderer/renderer.ixx
        src/hook/impl/hook_impl_type.ixx
        src/gui/widget/gui_widget_base.ixx
        src/gui/gui_widget_regedit.ixx
        src/queue/worker_queue.ixx
        src/hash/xxhash_wrapper.ixx
        src/gui/widget/gui_widget_container.ixx
        src/gui/widget/gui_widget_type.ixx
        src/bootstrap/bootstrap_gui.ixx
        src/gui/widget/gui_widget_root.ixx
        src/angelscript/as_engine.ixx
        src/bootstrap/bootstrap_angelscript.ixx
        src/angelscript/as_engine_binding_interface.ixx
        src/menu/menu_navbar_form.ixx
        src/menu/menu_form_interface.ixx
        src/menu/menu_navbar_form_designer.ixx
        src/gui/widget/gui_widget_navbar.ixx
        src/gui/widget/gui_widget_context_menu.ixx
        src/gui/widget/gui_widget_separator.ixx
        src/gui/widget/gui_widget_windows_container.ixx
        src/gui/widget/gui_widget_menu_item.ixx
        src/gui/widget/gui_widget_clickable_base.ixx
        src/menu/menu_settings_form.ixx
        src/menu/menu_settings_form_designer.ixx
        src/gui/widget/gui_widget_window.ixx
        src/gui/widget/gui_widget_child_window.ixx
        src/gui/widget/gui_widget_same_line.ixx
        src/gui/widget/gui_widget_checkbox.ixx
        src/gui/widget/gui_widget_text.ixx
        src/gui/widget/gui_widget_slider_base.ixx
        src/gui/widget/gui_widget_slider.ixx
        src/gui/widget/gui_widget_button.ixx
        src/home/home_directory.ixx
        src/gui/widget/gui_widget_script_list.ixx
        src/angelscript/as_context_pool.ixx
        src/angelscript/as_manager.ixx
        src/angelscript/as_instance.ixx
        src/config/config.ixx
        src/input/bind_system.ixx
        src/sdk/interface/sdk_interface_scanner.ixx
        src/sdk/locator/sdk_locator_source2_client.ixx
        src/sdk/engine/sdk_source2_client.ixx
        src/hook/impl/hook_impl_frame_stage_notify.ixx
        src/sdk/datatype/sdk_client_frame_stage_t.ixx
        src/angelscript/binding/as_binding_imgui.ixx
        src/angelscript/as_engine_interface.ixx
        src/angelscript/binding/memory/as_binding_memory.ixx
        src/angelscript/binding/as_binding_hook.ixx
        src/angelscript/binding/as_binding_renderer.ixx
        src/sdk/locator/sdk_locator_vfile_system.ixx
        src/sdk/filesystem/sdk_base_filesystem.ixx
        src/memory/vmt_call.ixx
        src/renderer/renderer_load_image.ixx
        src/sdk/parser/sdk_parser_vtex.ixx
        src/sdk/locator/sdk_locator_matrices_for_view.ixx
        src/sdk/sdk_signature.ixx
        src/memory/pattern_scanner.ixx
        src/hook/impl/hook_impl_get_matrices_for_view.ixx
        src/sdk/render/sdk_matrices_system.ixx
        src/sdk/math/sdk_math_matrix.ixx
        src/sdk/math/sdk_math_vector.ixx
        src/sdk/math/sdk_math_world_to_screen.ixx
        src/sdk/engine/sdk_source2_engine_to_client.ixx
        src/sdk/locator/sdk_locator_source2_engine_to_client.ixx
        src/sdk/locator/sdk_locator_world_state.ixx
        src/hook/impl/hook_impl_level_state_change.ixx
)

target_compile_options(dxx_dlc PRIVATE -mcx16 -mavx2 -mfma)

target_include_directories(dxx_dlc PUBLIC
        "external/"
        "src/"
        "."
        "external/minhook/include/"
        "external/libguarded/src/"
        "external/imgui/backends/"
        "external/imgui/"
        "external/xxhash/"
        "external/angelscript/add_on/"
        "external/nlohmann/single_include/"
        "external/hde/include/"
        "external/asbind20/include/"
        "external/lz4/lib/"
        "external/mem_scanner/include/"
        "external/dx_math/Inc/"
        "external/dx_xtk/Inc/"
)

target_link_libraries(dxx_dlc PRIVATE d3d11 d3dcompiler dwmapi angelscript ws2_32 LightningScanner)
