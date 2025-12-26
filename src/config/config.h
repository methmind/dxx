//
// Created by sexey on 04.11.2025.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include "gui/gui_widget_regedit.h"

namespace config
{
    class C_Config
    {
    private:

        static void LoadPriority(nlohmann::json& document,  const gui::C_WidgetRegedit::widget_list_t& widgets);

    public:

        static bool Load(const std::filesystem::path& path, const std::shared_ptr<gui::C_WidgetRegedit>& regedit);

        static bool Save(const std::filesystem::path& path, const gui::C_WidgetRegedit::widget_list_t& widgets);
    };
} // config

#endif //CONFIG_H