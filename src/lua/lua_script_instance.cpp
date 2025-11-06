//
// Created by sexey on 26.10.2025.
//

#include "lua_script_instance.h"

namespace lua
{
    void C_LuaScriptInstance::addWidget(const gui::widget_ptr_t& widget)
    {
        this->widgets_.push_back(widget);
    }

    bool C_LuaScriptInstance::initialize(const sol::protected_function_result& instance)
    {
        auto module = instance.get<sol::table>();
        if (!module.valid()) {
            return false;
        }

        if (this->entryPoint_= module[LUA_ENTRY_FUNCTION_NAME];
            !this->entryPoint_.valid() || this->entryPoint_.get_type() != sol::type::function) {
            return false;
        }

        this->entryPoint_(0);
        return true;
    }

    C_LuaScriptInstance::~C_LuaScriptInstance()
    {
        if (this->entryPoint_.valid()) {
            this->entryPoint_(1337);
        }

        for (const auto& widget : this->widgets_) {
            this->widgetRegedit_->remove(widget->getID().c_str());
        }
    }
} // lua