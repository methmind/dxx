//
// Created by sexey on 09.02.2026.
//
module;
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>

export module service.container;

consteval uint64_t fnv1a(const std::string_view str)
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
    constexpr std::string_view fullFunctionName = __PRETTY_FUNCTION__;
    constexpr auto prefix = fullFunctionName.find("T = ") + 4;
    constexpr auto suffix = fullFunctionName.size() - fullFunctionName.find_last_of(']');

    return fullFunctionName.substr(prefix, fullFunctionName.size() - prefix - suffix);
}

template<typename T>
struct type_hash_s
{
    static constexpr auto Name = type_name_impl<T>();
    static constexpr uint64_t Value = fnv1a(Name);

    [[nodiscard]] static consteval uint64_t GetHash() { return Value; }

    [[nodiscard]] static consteval std::string_view GetName() { return Name; }
};

export class C_ServiceContainer
{
public:
    C_ServiceContainer() = default;

    ~C_ServiceContainer()
    {
        this->services_.clear();
        while (!this->deleteOrder_.empty()) {
            this->deleteOrder_.pop_back();
        }
    }

    template<typename T, typename ... args_t>
    std::shared_ptr<T> add(args_t&& ... args)
    {
        constexpr auto serviceHash = type_hash_s<T>::GetHash();
        if (auto it = this->services_.find(serviceHash); it != this->services_.end()) {
            return std::static_pointer_cast<T>(it->second);
        }

        auto it = this->services_.emplace(serviceHash, std::make_shared<T>(std::forward<args_t>(args)...));
        this->deleteOrder_.push_back(it.first->second);

        return std::static_pointer_cast<T>(it.first->second);
    }

    template<typename T>
    std::shared_ptr<T> get()
    {
        constexpr auto serviceHash = type_hash_s<T>::GetHash();
        auto it = this->services_.find(serviceHash);
        if (it == this->services_.end()) {
            return nullptr;
        }

        return std::static_pointer_cast<T>(it->second);
    }

private:
    std::unordered_map<uint64_t, std::shared_ptr<void>> services_;
    std::deque<std::shared_ptr<void>> deleteOrder_;
};
