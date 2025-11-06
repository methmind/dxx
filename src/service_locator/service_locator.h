//
// Created by sexey on 01.09.2025.
//

#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

#include <memory>

class C_ServiceLocator
{
public:
    template<typename T>
    static std::shared_ptr<T> getInstance() {
        static std::shared_ptr<T> instance = std::make_shared<T>();
        return instance;
    }

    #define REGISTER_GLOBAL_SERVICE(T) \
        template std::shared_ptr<T> C_ServiceLocator::getInstance<T>();
};

#endif //SERVICE_LOCATOR_H