#ifndef LAZYBOX__INCLUDE_RANDOM__RANDOM_H
#define LAZYBOX__INCLUDE_RANDOM__RANDOM_H

#include <cstdint>

#include <random>

namespace lbox
{

template<class Engine = std::mt19937>
class RandomBox
{
public:
    using result_type = typename Engine::result_type;

    RandomBox() : generator_() {}
    RandomBox(result_type seed) : generator_(seed) {}
    ~RandomBox() {}

    void Seed(result_type seed) { generator_.seed(seed); }

    result_type Get() {
        return generator_();
    }

    template<class Filter_>
    typename Filter_::result_type Filter(Filter_& filter) { return filter(generator_); }
    
    // template<class Filter_>
    // typename Filter_::result_type Filter(Filter_&& filter) { return filter(generator_); }

private:
    Engine generator_;
};

    
} // namespace lbox



#endif