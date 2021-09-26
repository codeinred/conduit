#include "run_test.hpp"
#include <conduit/task.hpp>
#include <stdexcept>

task test_task_ex(std::string what) {
    throw std::logic_error(what);

    co_return;
}

future<std::string> test_task_except_1(std::string on_success) {
    try {
        auto coro = test_task_ex(on_success);
        co_await coro;
        co_return "No exception caught";
    } catch (std::logic_error& ex) {
        co_return ex.what();
    }
}

RUN_CORO_TEST(test_task_except_1);
