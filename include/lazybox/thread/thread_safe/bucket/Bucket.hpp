#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_THREAD_THREAD_SAFE_BUCKET_BUCKET_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_THREAD_THREAD_SAFE_BUCKET_BUCKET_HPP_

#include <forward_list>
#include <algorithm>

#include "lazybox/thread/Lock.hpp"
#include "lazybox/Assert.hpp"

namespace lbox::thread_safe {

template <class T>
class Bucket {
public:
    Bucket()  = default;
    ~Bucket() = default;

    bool Contain(const T &value) {
        std::lock_guard<FastLock> lg{lock_};

        return std::find(list_.begin(), list_.end(), value) != list_.end();
    }

    template <class U>
    void Add(U &&value) {
        std::lock_guard<FastLock> lg{lock_};

        auto i = std::count(this->list_.begin(), this->list_.end(), value);
        Assert(i <= 1);

        if(i == 0)
            list_.push_front(std::forward<U>(value));
    }

    void Remove(const T &value) {
        std::lock_guard<FastLock> lg{lock_};

        // std::erase(this->list_, value); // C++20
        std::ignore = std::remove(this->list_.begin(), this->list_.end(), value);
    }

    std::vector<T> Dump() {
        std::lock_guard<FastLock> lg{lock_};

        std::vector<T> vec{list_.begin(), list_.end()};
        return vec;
    }

    template <class Fn>
    void Foreach(Fn func) {
        std::lock_guard<FastLock> lg{lock_};

        std::for_each(this->list_.begin(), this->list_.end(), func);
    }

private:
    FastLock             lock_;
    std::forward_list<T> list_;
};

template <class K, class V>
class Bucket<std::pair<K, V>> {
    using PairType = std::pair<K, V>;

public:
    Bucket()  = default;
    ~Bucket() = default;

    bool Contain(const K &value) {
        std::lock_guard<FastLock> lg{lock_};

        return Find(value) != list_.end();
    }

    template <class U>
    void Add(U &&value) {
        std::lock_guard<FastLock> lg{lock_};
        if(Find(value.first) == list_.end()) {
            this->list_.push_front(std::forward<U>(value));
        }
    }

    void Remove(const K &value) {
        std::lock_guard<FastLock> lg{lock_};

#if(__cplusplus >= 202002L)
        std::erase_if(this->list_, [&value](const PairType &pair) { return pair.first == value; });
#else
        auto &it = this->Find(value).operator*();
        std::ignore = std::remove(this->list_.begin(), this->list_.end(), it);
#endif
    }

    bool Get(const K &key, V &value) {
        std::lock_guard<FastLock> lg{lock_};

        auto it = Find(key);
        if(it != list_.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    std::vector<PairType> Dump() {
        std::lock_guard<FastLock> lg{lock_};

        std::vector<PairType> vec{list_.begin(), list_.end()};
        return vec;
    }

    template <class Fn>
    void Foreach(Fn func) {
        std::lock_guard<FastLock> lg{lock_};

        std::for_each(this->list_.begin(), this->list_.end(), func);
    }

private:
    typename std::forward_list<PairType>::iterator Find(const K &key) {
        typename std::forward_list<PairType>::iterator it = list_.begin();
        while(it != list_.end()) {
            if(it->first == key) {
                return it;
            }
            it++;
        }
        return list_.end();
    }

private:
    FastLock                    lock_;
    std::forward_list<PairType> list_;
};


} // namespace lbox::thread_safe

#endif // LAZYBOX_INCLUDE_LAZYBOX_THREAD_THREAD_SAFE_BUCKET_BUCKET_HPP_
