#pragma once
#include <common.hpp>

namespace async {
struct jump {
    std::coroutine_handle<> callback = std::noop_coroutine();
    constexpr bool await_ready() noexcept { return false; }
    auto await_suspend(std::coroutine_handle<>) noexcept { return callback; }
    void await_resume() noexcept {}
};
} // namespace async