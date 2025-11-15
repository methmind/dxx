//
// Created by sexey on 13.11.2025.
//

#ifndef DXX_DLC_CALLBACK_API_H
#define DXX_DLC_CALLBACK_API_H

namespace lua::script
{
    constexpr std::string CALLBACK_API_CHUNK_NAME = "callback";

    constexpr auto CALLBACK_API_SCRIPT = R"(
        -- Callback Registration API with Script ID Tracking
        -- Supports arbitrary number of arguments and callback cleanup

        local callback = {}

        -- Internal storage: event_name -> { { func = callable, script_id = string }, ... }
        local _callbacks = {}

        -- Helper function to get calling script path using debug API
        -- Traverses the entire call stack to find the topmost script
        local function _get_calling_script_id()
            local level = 1
            local max_level = 0

            -- Find the maximum stack level
            while debug.getinfo(level, "S") do
                max_level = level
                level = level + 1
            end

            -- Read info from the topmost script level
            if max_level > 0 then
                local info = debug.getinfo(max_level, "S")
                if info and info.source then
                    local source = info.source
                    -- Remove leading '@' if present (lua debug convention for files)
                    if source:sub(1, 1) == "@" then
                        return source:sub(2)
                    end
                    return source
                end
            end

            return "unknown"
        end

        -- Helper function to validate event name
        local function _validate_event_name(event_name)
            if type(event_name) ~= "string" or event_name == "" then
                error("Event name must be a non-empty string", 3)
            end
        end

        -- Helper function to validate callback function
        local function _validate_callback(callback_func)
            if type(callback_func) ~= "function" then
                error("Callback must be a function", 3)
            end
        end

        -- Register a callback for an event
        -- @param event_name (string): Name of the event to listen to
        -- @param callback_func (function): Function to call when event is triggered
        -- @return (number): Callback ID for later removal (optional, can be used with unregister_by_callback_id)
        function callback.register(event_name, callback_func)
            _validate_event_name(event_name)
            _validate_callback(callback_func)

            local script_id = _get_calling_script_id()

            if not _callbacks[event_name] then
                _callbacks[event_name] = {}
            end

            local callback_entry = {
                func = callback_func,
                script_id = script_id
            }

            table.insert(_callbacks[event_name], callback_entry)

            -- Return the index (can be used for unregistering specific callbacks)
            return #_callbacks[event_name]
        end

        -- Unregister a specific callback by script ID and event name
        -- @param event_name (string): Name of the event
        -- @param script_id (string): Script path to unregister callbacks from
        -- @return (number): Number of callbacks removed
        function callback.unregister_by_script(event_name, script_id)
            _validate_event_name(event_name)

            if not _callbacks[event_name] then
                return 0
            end

            local removed_count = 0
            local i = 1

            while i <= #_callbacks[event_name] do
                if _callbacks[event_name][i].script_id == script_id then
                    table.remove(_callbacks[event_name], i)
                    removed_count = removed_count + 1
                else
                    i = i + 1
                end
            end

            -- Clean up empty event tables
            if #_callbacks[event_name] == 0 then
                _callbacks[event_name] = nil
            end

            return removed_count
        end

        -- Remove all callbacks registered by a specific script
        -- @param script_id (string): Script path to clean up
        -- @return (table): Statistics of removed callbacks { total = n, events = { event_name = count, ... } }
        function callback.unregister_all_by_script(script_id)
            local stats = {
                total = 0,
                events = {}
            }

            for event_name, callbacks_list in pairs(_callbacks) do
                local removed_count = 0
                local i = 1

                while i <= #callbacks_list do
                    if callbacks_list[i].script_id == script_id then
                        table.remove(callbacks_list, i)
                        removed_count = removed_count + 1
                    else
                        i = i + 1
                    end
                end

                if removed_count > 0 then
                    stats.total = stats.total + removed_count
                    stats.events[event_name] = removed_count
                end

                -- Clean up empty event tables
                if #callbacks_list == 0 then
                    _callbacks[event_name] = nil
                end
            end

            return stats
        end

        -- Unregister a specific callback by ID (position in callbacks list)
        -- @param event_name (string): Name of the event
        -- @param callback_id (number): Callback ID returned from register()
        -- @return (boolean): True if removed, false if not found
        function callback.unregister_by_id(event_name, callback_id)
            _validate_event_name(event_name)

            if not _callbacks[event_name] or not _callbacks[event_name][callback_id] then
                return false
            end

            table.remove(_callbacks[event_name], callback_id)

            if #_callbacks[event_name] == 0 then
                _callbacks[event_name] = nil
            end

            return true
        end

        -- Trigger an event with arbitrary number of arguments
        -- @param event_name (string): Name of the event to trigger
        -- @param ... : Arguments to pass to all registered callbacks
        -- @return (table): Results from all callbacks { results = {...}, errors = {...} }
        function callback.trigger(event_name, ...)
            _validate_event_name(event_name)

            if not _callbacks[event_name] then
                return { results = {}, errors = {} }
            end

            local results = {}
            local errors = {}

            for idx, callback_entry in ipairs(_callbacks[event_name]) do
                local success, result = pcall(callback_entry.func, ...)

                if success then
                    table.insert(results, {
                        callback_id = idx,
                        script_id = callback_entry.script_id,
                        result = result
                    })
                else
                    table.insert(errors, {
                        callback_id = idx,
                        script_id = callback_entry.script_id,
                        error = result
                    })
                end
            end

            return {
                results = results,
                errors = errors
            }
        end

        -- Get all registered callbacks for an event
        -- @param event_name (string): Name of the event
        -- @return (table): List of callbacks with their script IDs
        function callback.get_callbacks(event_name)
            _validate_event_name(event_name)

            if not _callbacks[event_name] then
                return {}
            end

            local result = {}
            for idx, callback_entry in ipairs(_callbacks[event_name]) do
                table.insert(result, {
                    id = idx,
                    script_id = callback_entry.script_id
                })
            end

            return result
        end

        -- Get all registered callbacks for a specific script
        -- @param script_id (string): Script path
        -- @return (table): List of events and callback counts
        function callback.get_callbacks_by_script(script_id)
            local result = {}

            for event_name, callbacks_list in pairs(_callbacks) do
                for idx, callback_entry in ipairs(callbacks_list) do
                    if callback_entry.script_id == script_id then
                        if not result[event_name] then
                            result[event_name] = {}
                        end
                        table.insert(result[event_name], idx)
                    end
                end
            end

            return result
        end

        -- Clear all callbacks (debug function)
        function callback.clear_all()
            _callbacks = {}
        end

        -- Get internal statistics
        -- @return (table): Statistics about registered callbacks
        function callback.get_stats()
            local stats = {
                total_events = 0,
                total_callbacks = 0,
                events = {}
            }

            for event_name, callbacks_list in pairs(_callbacks) do
                stats.total_events = stats.total_events + 1
                local count = #callbacks_list
                stats.total_callbacks = stats.total_callbacks + count

                local scripts_in_event = {}
                for _, callback_entry in ipairs(callbacks_list) do
                    if not scripts_in_event[callback_entry.script_id] then
                        scripts_in_event[callback_entry.script_id] = 0
                    end
                    scripts_in_event[callback_entry.script_id] = scripts_in_event[callback_entry.script_id] + 1
                end

                stats.events[event_name] = {
                    count = count,
                    scripts = scripts_in_event
                }
            end

            return stats
        end

        hook.set_on_render_start(function()
            callback.trigger("on_render_start")
        end)

        hook.set_on_lua_dispose(function(script_id)
            print("Dispose callbacks for: " .. script_id)
            callback.unregister_all_by_script(script_id)
        end)

        return callback
    )";
}

#endif //DXX_DLC_CALLBACK_API_H