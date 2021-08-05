#pragma once

#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ == 10 && __GNUC_MINOR__ <= 2
#define CONDUIT_USE_GCC_EXCEPTION_WORKAROUND 1
#endif
#endif

#if __has_include(<coroutine>)
#if defined(__clang__) && !defined(__cpp_impl_coroutine)
#define __cpp_impl_coroutine 201902L
#endif
#include <coroutine>

#ifdef __clang__

namespace std::experimental {
using std::coroutine_handle;
using std::coroutine_traits;
} // namespace std::experimental

#endif

#elif __has_include(<experimental/coroutine>)

#include <experimental/coroutine>
namespace std {
using std::experimental::coroutine_handle;
using std::experimental::coroutine_traits;
using std::experimental::suspend_always;
using std::experimental::suspend_never;
} // namespace std
#endif
