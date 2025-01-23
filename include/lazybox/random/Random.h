#ifndef LAZYBOX__INCLUDE_RANDOM__RANDOM_H
#define LAZYBOX__INCLUDE_RANDOM__RANDOM_H

#include <cstdint>

#include <random>
#include <unordered_set>
#include <vector>
#include <type_traits> // std::enable_if

namespace lbox
{

template<class Engine = std::default_random_engine>
class Random
{
public:
    using result_type = typename Engine::result_type;

    Random() : generator_() {}
    Random(result_type seed) : generator_(seed) {}
    Random(const Engine& engin) : generator_(engin) {}
    ~Random() {}

    void Seed(result_type seed) { generator_.seed(seed); }

    result_type Generate() {
        return generator_();
    }

    template<class Filter_>
    typename Filter_::result_type Filter(Filter_& filter) { return filter(generator_); }
    
    template<class Filter_>
    typename Filter_::result_type Filter(Filter_&& filter) { return filter(generator_); }


private:
    Engine generator_;
};

template <typename T, typename Enable = void> class RandomBox;

template<typename T>
class RandomBox<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
public:
    RandomBox() : generator_() {}
    RandomBox(unsigned int seed) : generator_(seed) {}
    RandomBox(const Random<std::mt19937>& generator) : generator_(generator) {}

    std::unordered_set<T> Generate(size_t count, T min_val, T max_val) {
        Assert(count > 0, "count must be big than 0");
        Assert(min_val < max_val, "min_val must be less than max_val");

        std::uniform_real_distribution<T> dist(min_val, max_val);
        std::unordered_set<T> result;

        while (result.size() < count) {
            result.insert(generator_.Filter(dist));
        }

        return result;
    }
    
private:
    Random<std::mt19937> generator_;
};

template<typename T>
class RandomBox<T, std::enable_if_t<std::is_integral_v<T>>>
{
public:
    RandomBox() : generator_() {}
    RandomBox(unsigned int seed) : generator_(seed) {}
    RandomBox(const Random<std::mt19937>& generator) : generator_(generator) {}

    std::unordered_set<T> Generate(size_t count, T min_val, T max_val) {
        Assert(count > 0, "count must be big than 0");
        Assert(min_val < max_val, "min_val must be less than max_val");
        Assert(count < (max_val - min_val), "count must be less than max_val - min_val");

        if (count == 1) {
            std::uniform_int_distribution<T> dist(min_val, max_val);
            return { generator_.Filter(dist) };
        }

        T range = max_val - min_val;
        std::vector<T> temp(range);
        std::unordered_set<T> result;

        if (range / count > 15) {
            std::uniform_int_distribution<T> dist(min_val, max_val);
            
            while (result.size() < count) {
                result.insert(generator_.Filter(dist));
            }
            
            return result;
        }
        
        for (size_t i = 0; i < range; ++i) {
            temp[i] = min_val + static_cast<T>(i);
        }

        for (size_t i = 0; i < count; ++i) {
            std::uniform_int_distribution<size_t> dist(i, range - 1);

            auto index = generator_.Filter(dist);
            std::swap(temp[i], temp[index]);
        }

        return {temp.begin(), temp.begin() + count};
    }

private:
    Random<std::mt19937> generator_;
};



    
} // namespace lbox



#endif