#include <benchmark/benchmark.h>

#include <utils/RandomValuePool.h>

#include <sofa/geometry/Tetrahedron.h>
#include <sofa/type/fixed_array.h>
#include <sofa/type/Vec.h>
#include <array>

template<typename Container>
static void BM_Tetrahedron_volume(benchmark::State& state);

using stdarray3f = std::array<float, 3>;
using sofatypefixedarray3f = sofa::type::fixed_array<float, 3>;

constexpr int64_t minSubIterations = 8 << 4;
constexpr int64_t maxSubIterations = 8 << 6;

BENCHMARK_TEMPLATE1(BM_Tetrahedron_volume, stdarray3f)->RangeMultiplier(2)->Ranges({ {minSubIterations, maxSubIterations } })->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE1(BM_Tetrahedron_volume, sofatypefixedarray3f)->RangeMultiplier(2)->Ranges({ {minSubIterations, maxSubIterations} })->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE1(BM_Tetrahedron_volume, sofa::type::Vec3f)->RangeMultiplier(2)->Ranges({ {minSubIterations, maxSubIterations} })->Unit(benchmark::kMicrosecond);

template<typename Container>
void BM_Tetrahedron_volume(benchmark::State& state)
{
    constexpr auto totalsize = maxSubIterations * 9;
    const std::array<float, totalsize>& values = RandomValuePool<float, totalsize>::get();

    for (auto _ : state)
    {
        for (unsigned int i = 0; i < state.range(0); ++i)
        {
            state.PauseTiming();
            state.ResumeTiming();
        }
    }
}
