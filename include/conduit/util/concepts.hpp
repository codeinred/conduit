#pragma once
#include <conduit/util/stdlib_coroutine.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace conduit {
// clang-format off
template <class A, class B>
concept same_as = std::is_same_v<A, B> && std::is_same_v<B, A>;

template <class F, class... Args>
concept invocable = requires(F func, Args... args) {
    { std::forward<F>(func)(std::forward<Args>(args)...) };
};

template <class From, class To>
concept convertible_to = std::is_convertible_v<From, To>
    && requires(std::add_rvalue_reference_t<From> (&f)()) {
    static_cast<To>(f());
};

template <class T>
concept non_void = !same_as<T, void>;

template <class T, class... Args>
using promise_t = typename std::coroutine_traits<T, Args...>::promise_type;

template <class T>
using handle_t = std::coroutine_handle<promise_t<T>>;


template<class T, class... Alt>
concept same_as_either = (same_as<T, Alt> || ...);

// Requires that either Alt... is empty, in which case any type matches
// or that T matches one of Alt
template<class T, class... Alt>
concept any_or_match = sizeof...(Alt) == 0 || same_as_either<T, Alt...>;

template<class Awaitable, class... ReturnSet>
concept hard_awaitable = requires(Awaitable a, std::coroutine_handle<> h) {
    { a.await_ready() } -> same_as<bool>;
    { a.await_suspend(h) } -> same_as_either<void, bool, std::coroutine_handle<>>;
    { a.await_resume() } -> any_or_match<ReturnSet...>;
};

template<class Awaitable, class... ReturnSet>
concept awaitable = hard_awaitable<Awaitable, ReturnSet...> || requires(Awaitable a) {
    { a.operator co_await() } -> hard_awaitable<ReturnSet...>;
};

template<class Promise>
concept co_promise = requires(Promise p) {
    { p.initial_suspend() } -> awaitable;
    { p.final_suspend() } -> awaitable;
    { p.get_return_object() } -> non_void;
    { p.unhandled_exception() } -> same_as<void>;
};

template<class Result>
concept co_result = requires { typename Result::promise_type; }
    && co_promise<typename Result::promise_type>
    && requires(promise_t<Result> p) { Result{p.get_return_object()}; };

template <class Promise, class Result>
concept return_object_aware = requires(Promise promise, Result* r) {
    { promise.set_return_object(r) };
};

template <class Promise>
concept has_get_value_member = co_promise<Promise>
    && requires(Promise p) {
    { p.get_value() };
};
template <class Promise>
concept has_value_member = co_promise<Promise>
    && requires(Promise p) {
    { p.value };
};

template <class Result, class Value>
concept can_co_return = co_result<Result>
    && requires(promise_t<Result> p, Value v) {
    { p.return_value(v) } -> same_as<void>;
};

template<class Promise>
concept awaitable_promise = awaitable<Promise> && co_promise<Promise>;

template<class Func, class... Args>
concept invokable_with = requires(Func f, Args... args) {
    { f(std::forward<Args>(args)...) };
};
// clang-format on
} // namespace conduit
