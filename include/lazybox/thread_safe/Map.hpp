#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_MAP_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_MAP_HPP_

#include <unordered_map>
#include <utility>

#include "bucket/Bucket.hpp"

namespace lbox::thread_safe {

template <class K, class V, int bucket_count = 13, class Hasher = std::hash<K>>
class Map {
    using Pair = std::pair<K, V>;

public:
    Map()  = default;
    ~Map() = default;

    bool Contain(const K &key) { return getBucket(key).Contain(key); }

    template <class U>
    void Add(U &&value) {
        getBucket(value.first).Add(std::forward<U>(value));
    }

    void Remove(const K &value) { getBucket(value).Remove(value); }

    bool Get(const K &key, V &value) { return getBucket(key).Get(key, value); }

    std::unordered_map<K, V> Dump() {
        std::unordered_map<K, V> map;
        for(auto &bucket : this->buckets_) {
            auto d = bucket.Dump();
            map.insert(d.begin(), d.end());
        }
        return map;
    }
    
    template <class Fn>
    void Foreach(Fn func) {
        for(auto &it : buckets_) {
            it.Foreach(func);
        }
    }

private:
    auto          getIndex(const K &value) { return Hasher()(value) % bucket_count; }
    Bucket<Pair> &getBucket(const K &value) { return this->buckets_.at(getIndex(value)); }

private:
    std::vector<Bucket<Pair>> buckets_{bucket_count};
};


} // namespace lbox::thread_safe

#endif // LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_MAP_HPP_
