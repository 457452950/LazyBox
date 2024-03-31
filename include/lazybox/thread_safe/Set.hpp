#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_SET_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_SET_HPP_

#include <unordered_set>

#include "bucket/Bucket.hpp"

namespace lbox::thread_safe {

template <class T, int bucket_count = 13, class Hasher = std::hash<T>>
class Set {
public:
    Set()  = default;
    ~Set() = default;

    bool Contain(const T &value) { return getBucket(value).Contain(value); }

    template <class U>
    void Add(U &&value) {
        getBucket(value).Add(std::forward<U>(value));
    }

    void Remove(const T &value) { getBucket(value).Remove(value); }

    std::unordered_set<T> Dump() {
        std::unordered_set<T> set;
        for(auto &bucket : this->buckets_) {
            auto d = bucket.Dump();
            set.insert(d.begin(), d.end());
        }
        return set;
    }

    template <class Fn>
    void Foreach(Fn func) {
        for(auto &it : buckets_) {
            it.Foreach(func);
        }
    }

private:
    auto       getIndex(const T &value) { return Hasher()(value) % bucket_count; }
    Bucket<T> &getBucket(const T &value) { return this->buckets_.at(getIndex(value)); }

private:
    std::vector<Bucket<T>> buckets_{bucket_count};
};

} // namespace lbox::thread_safe

#endif // LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_SET_HPP_
