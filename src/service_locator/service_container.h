//
// Created by sexey on 29.10.2025.
//

#ifndef SERVICE_CONTAINER_H
#define SERVICE_CONTAINER_H

#include <memory>
#include <string_view>
#include <unordered_map>
#include "debug/debug_output.h"

namespace detail
{
    constexpr uint64_t fnv1a(const std::string_view str)
    {
        uint64_t hash = 14695981039346656037ULL;
        for (const char c : str) {
            hash ^= static_cast<uint64_t>(c);
            hash *= 1099511628211ULL;
        }
        return hash;
    }

    template<typename T>
    consteval auto type_name_impl()
    {
#if defined(__GNUC__) || defined(__clang__)
        std::string_view name = __PRETTY_FUNCTION__;
        constexpr std::string_view prefix = "consteval auto detail::type_name_impl() [with T = ";
        constexpr std::string_view suffix = "]";
#elif defined(_MSC_VER)
        std::string_view name = __FUNCSIG__;
        constexpr std::string_view prefix = "auto __cdecl detail::type_name_impl<";
        constexpr std::string_view suffix = ">(void)";
#endif
        return std::string_view(name.begin() + prefix.size(), name.end() - suffix.size());
    }
}

template<typename T>
struct type_hash_s
{
    static constexpr auto Name = detail::type_name_impl<T>();
    static constexpr uint64_t Value = detail::fnv1a(Name);

    static constexpr uint64_t Get() { return Value; }
    static constexpr std::string_view GetName() { return Name; }
};

class C_ServiceContainer
{
private:
    std::unordered_map<uint64_t, std::shared_ptr<void>> services_;

public:

    template<typename int_t, typename   ... args_t>
    std::shared_ptr<int_t> add(args_t&& ... args)
    {
        constexpr auto key = type_hash_s<int_t>::Get();
        constexpr auto typeName = type_hash_s<int_t>::GetName();

        dbg("Registered service: '%s' (hash: %llu)", typeName.data(), key);

        auto it = this->services_.emplace(key, std::make_shared<int_t>(std::forward<args_t>(args)...));
        return std::static_pointer_cast<int_t>(it.first->second);
    }

    template<typename int_t>
    void add(std::shared_ptr<int_t> service)
    {
        constexpr auto key = type_hash_s<int_t>::Get();
        constexpr auto typeName = type_hash_s<int_t>::GetName();

        if (this->services_.contains(key)) {
            return;
        }

        dbg("Registered service: '%s' (hash: %llu)", typeName, key);
        this->services_[key] = service;
    }

    template<typename int_t>
    std::shared_ptr<int_t> get()
    {
        constexpr auto key = type_hash_s<int_t>::Get();

        const auto it = this->services_.find(key);
        return it != this->services_.end() ? std::static_pointer_cast<int_t>(it->second) : nullptr;
    }

    C_ServiceContainer() = default;

    ~C_ServiceContainer() = default;
};

#endif //SERVICE_CONTAINER_H