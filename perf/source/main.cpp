#include <benchmark/benchmark.h>
#include <bungeegum/bungeegum.hpp>

int main(int argc, char** argv)
{
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    };
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
	return 0;
}