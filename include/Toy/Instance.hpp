#pragma once
#ifndef LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_
#define LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_

#include "thread/Lock.hpp"
#include "NonCopyAble.hpp"

namespace lbox {

namespace lazy {
/**
 * 单例模板，懒汉式
 * @tparam T 单例类
 */
template <class T>
class Instance final : public NonCopyAble {
public:
    using ValueType    = T;
    using ValuePointer = T *;

public:
    template <class... Types>
    static void Init(Types &&...Args) {
        Instance::instance_ = new ValueType(std::forward<Types>(Args)...);
    }

    static void Destroy() {
        delete Instance::instance_;
        Instance::instance_ = nullptr;
    }

    /**
     * thread safety
     * @return
     */
    static ValuePointer GetInstance() {
        assert(instance_ && "instance has not initialized");
        return Instance::instance_;
    }

private:
    static ValuePointer instance_;

    Instance()  = default;
    ~Instance() = default;
};

template <class T>
inline Instance<T>::ValuePointer Instance<T>::instance_ = nullptr;
} // namespace lazy


/**
 * 单例模板，懒汉式
 * @tparam T 单例类
 */
template <class T>
class Instance final : public NonCopyAble {
public:
    using ValueType    = T;
    using ValuePointer = T *;

public:
    /**
     * thread safety
     * @return
     */
    static ValuePointer GetInstance() {
        static auto instance = new ValueType{};
        return instance;
    }

private:
    Instance()  = default;
    ~Instance() = default;
};


} // namespace lbox

#endif // LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_
