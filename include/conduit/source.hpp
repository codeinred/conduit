#pragma once
#include <conduit/async/coro.hpp>
#include <conduit/mixin/awaitable_parts.hpp>
#include <conduit/mixin/promise_parts.hpp>
#include <conduit/util/optional_ref.hpp>

#include <optional>

namespace conduit::promise {
template <class ReturnValue>
struct source : mixin::HasOwnerAndCallback,
                mixin::GetReturnObject<source<ReturnValue>>,
                mixin::UnhandledException<source<ReturnValue>> {
   private:
    ReturnValue const* pointer = nullptr;

   public:
    constexpr auto get_value() & noexcept { return optional_ref{pointer}; }
    constexpr auto get_value() && noexcept -> std::optional<ReturnValue> {
        if (pointer) {
            return std::optional<ReturnValue>{*pointer};
        } else {
            return std::nullopt;
        }
    }
    constexpr auto yield_value(ReturnValue const& value) noexcept {
        pointer = &value;
        return callback.release();
    }
    constexpr void return_void() noexcept { pointer = nullptr; }
};
template <>
struct source<void> : mixin::HasOwnerAndCallback,
                      mixin::GetReturnObject<source<void>>,
                      mixin::UnhandledException<source<void>>,
                      mixin::ReturnVoid {
   public:
    auto yield_value(tags::nothing_t) noexcept { return callback.release(); }
    auto yield_value(int) noexcept { return callback.release(); }
};
} // namespace conduit::promise

namespace conduit {
template <class T>
struct source : async::coro<promise::source<T>> {
    using promise_type = promise::source<T>;
    using async::coro<promise::source<T>>::coro;
};
} // namespace conduit
