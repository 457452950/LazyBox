#pragma once
#ifndef LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_
#define LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_

#include "thread/Lock.hpp"
#include "NonCopyAble.hpp"

namespace lbox {

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
        if(instance_ == nullptr) {

            UniqueLock uni_lock(instance_lock);

            if(instance_ == nullptr) {
                instance_ = new ValueType(std::forward<Types>(Args)...);
            }
        }
    }

    static ValuePointer GetInstance() { return instance_; }
    static void         Destroy() {
        UniqueLock uni_lock(instance_lock);

        delete instance_;
        instance_ = nullptr;
    }


private:
    static FastLock     instance_lock;
    static ValuePointer instance_;

    Instance()  = default;
    ~Instance() = default;
};

template <class T>
inline Instance<T>::ValuePointer Instance<T>::instance_ = nullptr;
template <class T>
inline FastLock Instance<T>::instance_lock;


} // namespace lbox

#endif // LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_
