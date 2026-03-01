//
// Created by sexey on 22.02.2026.
//
module;
#include <atomic>
#include <functional>
#include <memory>

export module as.binding.memory;

export namespace as
{
    template<typename T>
    class C_SharedPtr
    {
    public:
        using on_dispose_callback_t = std::function<void(C_SharedPtr*)>;

        ~C_SharedPtr()
        {
            if (this->onDispose_) {
                this->onDispose_(this);
            }
        }

        explicit C_SharedPtr(std::shared_ptr<T> instance) : instance_(std::move(instance)), refCount_(1) {}

        explicit C_SharedPtr(std::shared_ptr<T> instance, const on_dispose_callback_t& onDispose) :
            instance_(std::move(instance)), refCount_(1), onDispose_(onDispose) {}

        C_SharedPtr(const C_SharedPtr& other) : instance_(other.instance_), refCount_(1) {}

        C_SharedPtr& operator=(const C_SharedPtr& other)
        {
            if (this != &other) {
                this->instance_ = other.instance_;
            }

            return *this;
        }

        C_SharedPtr(C_SharedPtr&& other) noexcept : instance_(std::move(other.instance_)), refCount_(1) {}

        C_SharedPtr& operator=(C_SharedPtr&& other) noexcept
        {
            if (this != &other) {
                this->instance_ = std::move(other.instance_);
            }

            return *this;
        }

        explicit operator bool() const { return this->get() != nullptr; }

        T* operator ->() const { return this->get(); }

        [[nodiscard]] T* get() const { return this->instance_.get(); }

        [[nodiscard]] std::shared_ptr<T> get_shared() const { return this->instance_; }

        template <typename base_t>
        static C_SharedPtr<base_t>* implicitCast(C_SharedPtr* self)
        {
            if (!self) {
                return nullptr;
            }

            if (auto basePtr = std::dynamic_pointer_cast<base_t>(self->get_shared())) {
                // Создаем новую обертку.
                // Удалять виджет из реестра должен только оригинальный хэндл.

                // self->release();
                return new C_SharedPtr<base_t>(basePtr);
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

    protected:
        [[nodiscard]] int32_t refCount() const { return this->refCount_.load(std::memory_order_acquire); }

    private:
        std::shared_ptr<T> instance_;
        std::atomic<int32_t> refCount_;
        on_dispose_callback_t onDispose_;
    };
}
