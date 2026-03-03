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
    // Callback вызывается ровно один раз — когда последний хэндл умирает.
    struct dispose_token_s
    {
        using callback_t = std::function<void()>;

        explicit dispose_token_s(callback_t callback) : callback_(std::move(callback)) {}

        ~dispose_token_s()
        {
            if (this->callback_) {
                this->callback_();
            }
        }

        dispose_token_s(const dispose_token_s&) = delete;
        dispose_token_s& operator=(const dispose_token_s&) = delete;

    private:
        callback_t callback_;
    };

    template<typename T>
    class C_SharedPtr
    {
    public:
        ~C_SharedPtr() = default;

        explicit C_SharedPtr(std::shared_ptr<T> instance) : instance_(std::move(instance)), refCount_(1) {}

        explicit C_SharedPtr(std::shared_ptr<T> instance, std::shared_ptr<dispose_token_s> token) :
            instance_(std::move(instance)), refCount_(1), disposeToken_(std::move(token)) {}

        C_SharedPtr(const C_SharedPtr& other) :
            instance_(other.instance_), refCount_(1), disposeToken_(other.disposeToken_) {}

        C_SharedPtr& operator=(const C_SharedPtr& other)
        {
            if (this != &other) {
                this->instance_ = other.instance_;
                this->disposeToken_ = other.disposeToken_;
            }

            return *this;
        }

        C_SharedPtr(C_SharedPtr&& other) noexcept :
            instance_(std::move(other.instance_)), refCount_(1),
            disposeToken_(std::move(other.disposeToken_)) {}

        C_SharedPtr& operator=(C_SharedPtr&& other) noexcept
        {
            if (this != &other) {
                this->instance_ = std::move(other.instance_);
                this->disposeToken_ = std::move(other.disposeToken_);
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
                // Каст разделяет disposeToken_ с оригиналом. Dispose вызовется только когда умрёт последний хэндл (оригинал или каст).
                return new C_SharedPtr<base_t>(basePtr, self->disposeToken_);
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
        template<typename U> friend class C_SharedPtr;

        std::shared_ptr<T> instance_;
        std::atomic<int32_t> refCount_;
        std::shared_ptr<dispose_token_s> disposeToken_;
    };
}
