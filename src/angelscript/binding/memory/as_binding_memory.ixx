//
// Created by sexey on 22.02.2026.
//
module;
#include <atomic>
#include <memory>

export module as.binding.memory;

export namespace as
{
    template<typename T>
    class C_SharedPtr
    {
    public:
        explicit C_SharedPtr(std::shared_ptr<T> instance) : instance_(std::move(instance)), refCount_(1) {}

        C_SharedPtr(const C_SharedPtr&) = delete;
        C_SharedPtr& operator=(const C_SharedPtr&) = delete;
        C_SharedPtr(C_SharedPtr&&) = delete;
        C_SharedPtr& operator=(C_SharedPtr&&) = delete;

        [[nodiscard]] T* get() const { return this->instance_.get(); }

        [[nodiscard]] std::shared_ptr<T> get_shared() const { return this->instance_; }

        template <typename base_t>
        static C_SharedPtr<base_t>* implicitCast(C_SharedPtr* self)
        {
            if (!self) {
                return nullptr;
            }

            if (auto basePtr = std::dynamic_pointer_cast<base_t>(self->get_shared())) {
                auto* result = new C_SharedPtr<base_t>(basePtr);
                self->release(); // transfer ownership from source handle
                return result;
            }

            return nullptr;
        }

        void addRef()
        {
            this->refCount_.fetch_add(1, std::memory_order_acq_rel);
        }

        void release()
        {
            if (this->refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete this;
            }
        }

    private:
        ~C_SharedPtr() = default; // prevent stack/member allocation

        std::shared_ptr<T> instance_;
        std::atomic<int32_t> refCount_;
    };
}
