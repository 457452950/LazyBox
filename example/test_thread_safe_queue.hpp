#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_QUEUE_HPP_
#define LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_QUEUE_HPP_

#include "lazybox/test/Test.h"
#include "lazybox/thread/thread_safe/Queue.hpp"

template <class T>
using Queue = lbox::thread_safe::Queue<T>;

class Data : lbox::NonCopyAble {
public:
    Data(int a) : a(a) {}
    ~Data() {}
    Data(Data &&a) { this->a = a.a; }
    Data &operator=(Data &&d) {
        this->a = d.a;
        return *this;
    }

    int a;
};

TCASE(ThreadSafe, Queue) {
    printf("thread safe queue\n");
    {
        Queue<Data> q1;
        try {
            auto p1 = q1.Pop();
        } catch(std::out_of_range &error) {
            printf("error %s\n", error.what());
            AddPass(true);
        }

        q1.Push(Data{2});
        auto p1 = q1.Pop();
        TASSERT(p1.a == 2, "push 2 but pop error");

        q1.Push(Data{3});
        Data p2(1);
        TASSERT(q1.Pop(p2) == true, "push 3 but pop error");
        TASSERT(p2.a == 3, "push 3 but not");
    }

    {
        Queue<int *> q2;
        auto         p2 = q2.Pop();
        TASSERT(p2 == nullptr, "pop error");

        q2.Push(new int{1});
        auto p = q2.Pop();
        TASSERT(*p == 1, "push 1 but pop error");
        delete p;

        q2.Push(new int{2});
        TASSERT(q2.Pop(p) == true, "push 2 but pop error");
        TASSERT(*p == 2, "push 2 but not");
        delete p;
    }

    {
        Queue<std::shared_ptr<int>> q2;
        auto                        p2 = q2.Pop();
        TASSERT(p2 == nullptr, "pop error");

        q2.Push(std::make_shared<int>(1));
        auto p = q2.Pop();
        TASSERT(*p == 1, "push 1 but pop error");

        q2.Push(std::make_shared<int>(2));
        TASSERT(q2.Pop(p) == true, "push 2 but pop error");
        TASSERT(*p == 2, "push 2 but not");
    }

    {
        Queue<std::unique_ptr<int>> q2;
        auto                        p2 = q2.Pop();
        TASSERT(p2 == nullptr, "pop error");

        q2.Push(std::make_unique<int>(1));
        auto p = q2.Pop();
        TASSERT(*p == 1, "push 1 but pop error");

        q2.Push(std::make_unique<int>(2));
        TASSERT(q2.Pop(p) == true, "push 2 but pop error");
        TASSERT(*p == 2, "push 2 but not");
    }
}

#endif // LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_QUEUE_HPP_
