//
// Created by sexey on 14.02.2026.
//
module;
#include <cassert>

export module service.locator;

export class C_ServiceLocator
{
public:
    template<typename T>
    static void Register(T* instance)
    {
        GetInstancePtr<T>() = instance;
    }

    template<typename T>
    static void Remove()
    {
        GetInstancePtr<T>() = nullptr;
    }

    template<typename T>
    static T* Get()
    {
        T* instance = GetInstancePtr<T>();
        assert(instance != nullptr && "Service is not registered!");

        return instance;
    }

    template<typename T>
    static bool Has()
    {
        return GetInstancePtr<T>() != nullptr;
    }

private:
    template<typename T>
    static T*& GetInstancePtr()
    {
        static T* instance = nullptr;
        return instance;
    }
};