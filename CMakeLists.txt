cmake_minimum_required(VERSION 4.0)
project(dxx_dlc)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE minhook_files "./external/minhook/src/*.c")

add_definitions(-DDEBUG_OUTPUT)

add_compile_options(-Wa,-mbig-obj)

add_library(dxx_dlc SHARED main.cpp ${minhook_files}
        external/imgui/backends/imgui_impl_dx11.cpp
        external/imgui/backends/imgui_impl_win32.cpp
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_tables.cpp
        src/gui/gui_widget_interface.h
        src/gui/gui_widget_regedit.cpp
        src/gui/gui_widget_regedit.h
        src/gui/widget/gui_widget_window.cpp
        src/gui/widget/gui_widget_window.h
        src/gui/gui_container_interface.h
        src/gui/widget/gui_widget_navbar.cpp
        src/gui/widget/gui_widget_navbar.h
        src/gui/widget/gui_widget_context_menu.cpp
        src/gui/widget/gui_widget_context_menu.h
        src/gui/gui_clickable_interface.h
        src/gui/widget/gui_widget_menu_item.cpp
        src/gui/widget/gui_widget_menu_item.h
        src/gui/widget/gui_widget_separator.cpp
        src/gui/widget/gui_widget_separator.h
        src/menu/menu_main_form.cpp
        src/menu/menu_main_form.h
        src/gui/widget/gui_widget_child_window.cpp
        src/gui/widget/gui_widget_child_window.h
        src/gui/widget/gui_widget_unformatted_text.cpp
        src/gui/widget/gui_widget_unformatted_text.h
        src/menu/menu_settings_form.cpp
        src/menu/menu_settings_form.h
        src/gui/widget/gui_widget_same_line.cpp
        src/gui/widget/gui_widget_same_line.h
        src/gui/widget/gui_widget_checkbox.cpp
        src/gui/widget/gui_widget_checkbox.h
        src/gui/widget/gui_widget_slider_int.cpp
        src/gui/widget/gui_widget_slider_int.h
        src/home/home_directory.cpp
        src/home/home_directory.h
        src/lua/lua_script_engine.cpp
        src/lua/lua_script_engine.h
        src/debug/debug_output.cpp
        src/lua/lua_script_manager.cpp
        src/lua/lua_script_manager.h
        src/lua/lua_script_instance.cpp
        src/lua/lua_script_instance.h
        src/lua/binding/lua_binding_menu.cpp
        src/lua/binding/lua_binding_menu.h
        src/gui/gui_widget_type.h
        src/gui/widget/gui_widget_root.cpp
        src/gui/widget/gui_widget_root.h
        src/bootstrap/application_builder.cpp
        src/bootstrap/application_builder.h
        src/service_locator/service_container.h
        src/menu/menu_main_form_designer.cpp
        src/menu/menu_main_form_designer.h
        src/menu/menu_form_interface.h
        src/menu/menu_settings_form_designer.cpp
        src/menu/menu_settings_form_designer.h
        src/lua/binding/lua_container_interface.h
        src/lua/lua_utils.cpp
        src/lua/lua_utils.h
        src/gui/widget/gui_widget_windows_container.cpp
        src/gui/widget/gui_widget_windows_container.h
        src/gui/widget/gui_widget_button.cpp
        src/gui/widget/gui_widget_button.h
        src/gui/gui_slider_interface.h
        src/gui/widget/gui_widget_slider_float.cpp
        src/gui/widget/gui_widget_slider_float.h
        src/gui/widget/gui_widget_lua_list.cpp
        src/gui/widget/gui_widget_lua_list.h
        src/config/config.cpp
        src/config/config.h
        src/application.cpp
        src/application.h
        src/bootstrap/bootstrap_gui.cpp
        src/bootstrap/bootstrap_gui.h
        src/dx/dx_present.cpp
        src/dx/dx_present.h
        src/hook/hook_dispatcher.cpp
        src/hook/hook_dispatcher.h
        src/hook/hook_manager.cpp
        src/hook/hook_manager.h
        src/hook/impl/hook_impl_present.cpp
        src/hook/impl/hook_impl_present.h
        src/hook/hook_original_invoker.h
        src/renderer/renderer.cpp
        src/renderer/renderer.h
)

target_compile_definitions(dxx_dlc PRIVATE SOL_ALL_SAFETIES_ON=1 SOL_PRINT_ERRORS=1 SOL_EXCEPTIONS_SAFE_PROPAGATION=1)

target_include_directories(dxx_dlc PUBLIC
        "external/"
        "external/imgui/backends/"
        "external/imgui/"
        "external/luajit/src/"
        "external/sol/single/single/include/"
        "external/nlohmann/single_include/"
        "external/minhook/include/"
        "src/"
        "."
)

target_link_libraries(dxx_dlc PRIVATE d3d11 d3dcompiler dwmapi ${CMAKE_SOURCE_DIR}/external/luajit/src/libluajit.a)