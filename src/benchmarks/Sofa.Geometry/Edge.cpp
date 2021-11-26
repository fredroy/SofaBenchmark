#include <benchmark/benchmark.h>

#include <utils/RandomValuePool.h>

#include <sofa/geometry/Edge.h>
#include <sofa/type/fixed_array.h>
#include <sofa/type/Vec.h>
#include <array>

template<typename Container>
static void BM_Edge_length(benchmark::State& state);

static void BM_Edge_norm(benchmark::State& state);

using stdarray3f = std::array<float, 3>;
using sofatypefixedarray3f = sofa::type::fixed_array<float, 3>;

constexpr int64_t minSubIterations = 8 << 4;
constexpr int64_t maxSubIterations = 8 << 6;

BENCHMARK_TEMPLATE1(BM_Edge_length, stdarray3f)->RangeMultiplier(2)->Ranges({ {minSubIterations, maxSubIterations} })->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE1(BM_Edge_length, sofatypefixedarray3f)->RangeMultiplier(2)->Ranges({ {minSubIterations, maxSubIterations} })->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE1(BM_Edge_length, sofa::type::Vec3f)->RangeMultiplier(2)->Ranges({ {minSubIterations, maxSubIterations} })->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Edge_norm)->RangeMultiplier(2)->Ranges({ {minSubIterations, maxSubIterations} })->Unit(benchmark::kMicrosecond);

template<typename Container>
void BM_Edge_length(benchmark::State& state)
{
    constexpr auto totalsize = maxSubIterations * 6;
    const std::array<float, totalsize>& values = RandomValuePool<float, totalsize>::get();

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<Container> vc1(state.range(0));
        std::vector<Container> vc2(state.range(0));

        for (unsigned int i = 0; i < state.range(0); ++i)
        {
            vc1.emplace_back(Container{ values[i * 6 + 0], values[i * 6 + 1], values[i * 6 + 2] });
            vc2.emplace_back(Container{ values[i * 6 + 3], values[i * 6 + 4], values[i * 6 + 5] });
        }
        state.ResumeTiming();

        for (unsigned int i = 0; i < state.range(0); ++i)
        {
            benchmark::DoNotOptimize(sofa::geometry::Edge::length(vc1[i], vc2[i]));
        }
    }
}

void BM_Edge_norm(benchmark::State& state)
{
    using Container = sofa::type::Vec3f;
    constexpr auto totalsize = maxSubIterations * 6;
    const std::array<float, totalsize>& values = RandomValuePool<float, totalsize>::get();

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<Container> vc1(state.range(0));
        std::vector<Container> vc2(state.range(0));

        for (unsigned int i = 0; i < state.range(0); ++i)
        {
            vc1.emplace_back(Container{ values[i * 6 + 0], values[i * 6 + 1], values[i * 6 + 2] });
            vc2.emplace_back(Container{ values[i * 6 + 3], values[i * 6 + 4], values[i * 6 + 5] });
        }
        state.ResumeTiming();
        for (unsigned int i = 0; i < state.range(0); ++i)
        {
            benchmark::DoNotOptimize( (vc1[i] - vc2[i]).norm());
        }
    }
}
