//
// Created by sexey on 13.11.2025.
//

#ifndef DXX_DLC_CALLBACK_API_H
#define DXX_DLC_CALLBACK_API_H

#include <string>

namespace lua::script
{
    constexpr std::string CALLBACK_API_CHUNK_NAME = "callback";

    constexpr auto CALLBACK_API_SCRIPT = R"(
        -- Callback Registration API with Script ID Tracking
        -- Supports arbitrary number of arguments and callback cleanup

        local callback = {}

        -- Compatibility: unpack location varies between Lua versions
        local unpack = table.unpack or unpack

        -- Internal storage: event_name -> { { func = callable, script_id = string, id = unique_id }, ... }
        local _callbacks = {}

        -- Counter for generating unique IDs
        local _next_callback_id = 1

        -- Storage for callback lookup by unique ID: unique_id -> { event_name = string, index = number }
        local _callback_lookup = {}

        -- Logger function (can be replaced with custom implementation)
        local _logger = {
            error = function(msg)
                print("[ERROR] " .. msg)
            end
        }

        -- Set custom logger
        function callback.set_logger(logger)
            if type(logger) == "table" and type(logger.error) == "function" then
                _logger = logger
            else
                error("Logger must be a table with error function")
            end
        end

        -- Helper function to get calling script path using debug API
        -- Traverses the entire call stack to find the topmost script
        local function _get_calling_script_id()
            local level = 2  -- Start from level 2 to skip this helper function
            local max_level = 0

            -- Find the maximum stack level
            while debug.getinfo(level, "S") do
                max_level = level
                level = level + 1
            end

            -- Read info from the topmost script level
            if max_level >= 2 then
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

        -- Rebuild lookup table for an event
        local function _rebuild_lookup(event_name)
            if not _callbacks[event_name] then
                return
            end

            for index, callback_entry in ipairs(_callbacks[event_name]) do
                if callback_entry.id then
                    _callback_lookup[callback_entry.id] = {
                        event_name = event_name,
                        index = index
                    }
                end
            end
        end

        -- Register a callback for an event
        -- @param event_name (string): Name of the event to listen to
        -- @param callback_func (function): Function to call when event is triggered
        -- @return (number): Stable callback ID for later removal
        function callback.register(event_name, callback_func)
            _validate_event_name(event_name)
            _validate_callback(callback_func)

            local script_id = _get_calling_script_id()
            local unique_id = _next_callback_id
            _next_callback_id = _next_callback_id + 1

            if not _callbacks[event_name] then
                _callbacks[event_name] = {}
            end

            local callback_entry = {
                func = callback_func,
                script_id = script_id,
                id = unique_id
            }

            table.insert(_callbacks[event_name], callback_entry)

            -- Update lookup table
            _callback_lookup[unique_id] = {
                event_name = event_name,
                index = #_callbacks[event_name]
            }

            -- Return stable unique ID
            return unique_id
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
                    local callback_id = _callbacks[event_name][i].id
                    if callback_id then
                        _callback_lookup[callback_id] = nil
                    end
                    table.remove(_callbacks[event_name], i)
                    removed_count = removed_count + 1
                else
                    i = i + 1
                end
            end

            -- Clean up empty event tables
            if #_callbacks[event_name] == 0 then
                _callbacks[event_name] = nil
            else
                -- Rebuild lookup for remaining callbacks
                _rebuild_lookup(event_name)
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
                        local callback_id = callbacks_list[i].id
                        if callback_id then
                            _callback_lookup[callback_id] = nil
                        end
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
                else
                    -- Rebuild lookup for remaining callbacks
                    _rebuild_lookup(event_name)
                end
            end

            return stats
        end

        -- Unregister a specific callback by its unique stable ID
        -- @param callback_id (number): Stable callback ID returned from register()
        -- @return (boolean): True if removed, false if not found
        function callback.unregister_by_id(callback_id)
            local lookup_entry = _callback_lookup[callback_id]

            if not lookup_entry then
                return false
            end

            local event_name = lookup_entry.event_name
            local index = lookup_entry.index

            if not _callbacks[event_name] or not _callbacks[event_name][index] then
                -- Lookup is stale, clean it up
                _callback_lookup[callback_id] = nil
                return false
            end

            -- Verify the ID matches (double-check consistency)
            if _callbacks[event_name][index].id ~= callback_id then
                -- Index has shifted, need to rebuild
                _rebuild_lookup(event_name)
                -- Try to find it again
                lookup_entry = _callback_lookup[callback_id]
                if not lookup_entry then
                    return false
                end
                index = lookup_entry.index
            end

            -- Remove the callback
            table.remove(_callbacks[event_name], index)
            _callback_lookup[callback_id] = nil

            -- Clean up empty event tables
            if #_callbacks[event_name] == 0 then
                _callbacks[event_name] = nil
            else
                -- Rebuild lookup for remaining callbacks since indices shifted
                _rebuild_lookup(event_name)
            end

            return true
        end

        -- Error handler for xpcall with stack trace
        local function _error_handler(err)
            return debug.traceback(tostring(err), 2)
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
            local args = {...}
            local n = select('#', ...)  -- Store argument count for proper unpacking

            for idx, callback_entry in ipairs(_callbacks[event_name]) do
                -- Use xpcall instead of pcall to get stack trace
                local success, result = xpcall(
                    function() return callback_entry.func(unpack(args, 1, n)) end,
                    _error_handler
                )

                if success then
                    table.insert(results, {
                        callback_id = callback_entry.id,
                        script_id = callback_entry.script_id,
                        result = result
                    })
                else
                    local error_info = {
                        callback_id = callback_entry.id,
                        script_id = callback_entry.script_id,
                        error = result
                    }
                    table.insert(errors, error_info)

                    -- Log only errors during callback execution
                    _logger.error(string.format(
                        "Callback #%d for event '%s' from script '%s' failed:\n%s",
                        callback_entry.id, event_name, callback_entry.script_id, result
                    ))
                end
            end

            return {
                results = results,
                errors = errors
            }
        end

        -- Get all registered callbacks for an event
        -- @param event_name (string): Name of the event
        -- @return (table): List of callbacks with their script IDs and stable IDs
        function callback.get_callbacks(event_name)
            _validate_event_name(event_name)

            if not _callbacks[event_name] then
                return {}
            end

            local result = {}
            for idx, callback_entry in ipairs(_callbacks[event_name]) do
                table.insert(result, {
                    id = callback_entry.id,
                    script_id = callback_entry.script_id
                })
            end

            return result
        end

        -- Get all registered callbacks for a specific script
        -- @param script_id (string): Script path
        -- @return (table): List of events and callback IDs
        function callback.get_callbacks_by_script(script_id)
            local result = {}

            for event_name, callbacks_list in pairs(_callbacks) do
                for idx, callback_entry in ipairs(callbacks_list) do
                    if callback_entry.script_id == script_id then
                        if not result[event_name] then
                            result[event_name] = {}
                        end
                        table.insert(result[event_name], callback_entry.id)
                    end
                end
            end

            return result
        end

        -- Clear all callbacks (debug function)
        function callback.clear_all()
            _callbacks = {}
            _callback_lookup = {}
            _next_callback_id = 1
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

        -- Protected hook setup with error handling
        local function _setup_hooks()
            hook.set_on_render_start(function()
                local success, err = pcall(function()
                    local rendererFrame = renderer.get_renderer_primitives()
                    callback.trigger("on_render_start", rendererFrame)
                    --rendererFrame:bake()
                end)

                if not success then
                    _logger.error("on_render_start hook failed: " .. tostring(err))
                end
            end)

            hook.set_on_update(function()
                local success, err = pcall(function()
                    callback.trigger("on_update")
                end)

                if not success then
                    _logger.error("on_update hook failed: " .. tostring(err))
                end
            end)

            hook.set_on_entity_create(function(entity)
                local success, err = pcall(function()
                    callback.trigger("on_entity_create", entity)
                end)

                if not success then
                    _logger.error("on_entity_create hook failed: " .. tostring(err))
                end
            end)

            hook.set_on_entity_remove(function(entity)
                local success, err = pcall(function()
                    callback.trigger("on_entity_remove", entity)
                end)

                if not success then
                    _logger.error("on_entity_remove hook failed: " .. tostring(err))
                end
            end)

            hook.set_on_level_init(function()
                local success, err = pcall(function()
                    callback.trigger("on_level_init")
                end)

                if not success then
                    _logger.error("on_level_init hook failed: " .. tostring(err))
                end
            end)

            hook.set_on_level_shutdown(function()
                local success, err = pcall(function()
                    callback.trigger("on_level_shutdown")
                end)

                if not success then
                    _logger.error("on_level_shutdown hook failed: " .. tostring(err))
                end
            end)

            hook.set_on_lua_dispose(function(script_id)
                local success, err = pcall(function()
                    callback.unregister_all_by_script(script_id)
                end)

                if not success then
                    _logger.error("on_lua_dispose hook failed: " .. tostring(err))
                end
            end)
        end

        -- Setup hooks when module is loaded
        _setup_hooks()

        return callback
    )";
}

#endif //DXX_DLC_CALLBACK_API_H