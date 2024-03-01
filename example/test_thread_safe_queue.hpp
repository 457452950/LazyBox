#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_QUEUE_HPP_
#define LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_QUEUE_HPP_

#include "lazybox/test/Test.h"
#include "lazybox/thread_safe/queue.hpp"

template <class T>
using Queue = lbox::thread_safe::Queue<T>;

TCASE(ThreadSafe, Queue) { Queue<int> a; }


#endif // LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_QUEUE_HPP_
