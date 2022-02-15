#pragma once
#include <conduit/generator.hpp>
#include <functional>

namespace conduit {
template <class F, class T>
auto fmap(F func, generator<T> source) -> generator<
    std::invoke_result_t<F&, typename generator<T>::iterator::reference>> {
    for (auto&& value : source) {
        co_yield std::invoke(func, static_cast<decltype(value)>(value));
    }
}
} // namespace conduit
